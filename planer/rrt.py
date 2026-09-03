from dataclasses import dataclass

import numpy as np

# A*、Dijkstra 通常需要先把环境离散成网格或图，而 RRT 可以直接在连续状态空间中采样
# RRT 最重要的优势之一，是它比较适合高维路径规划。例如六自由度机械臂的状态为：q=[q_1,q_2,q_3,q_4,q_5,q_6]^T
# 如果每个关节角离散为 100 个值，网格规模为：100^6=10^{12}，网格搜索很难直接处理这种规模，而 RRT 只采样实际需要探索的状态，不需要枚举整个空间。
# 容易加入运动学或动力学约束
# 空间探索速度快
# 劣势
# 生成的路径通常曲折、不平滑
# 随机性导致结果不稳定
# 基本 RRT 不保证路径最优
@dataclass
class RRTConfig:
    max_iter: int = 1000
    step_size: float = 1.0
    goal_bias: float = 0.10


@dataclass
class Node:
    # Coordinates use the GridMap convention: row, column.
    x: int
    y: int
    parent: object = None


class RRTPlaner:
    def __init__(self, grid_map, config=None, rng=None):
        self.map = grid_map
        self.config = config if config is not None else RRTConfig()
        self.rng = rng if rng is not None else np.random.default_rng()
        self.nodes = []

        if self.config.max_iter <= 0:
            raise ValueError("max_iter must be positive")
        if self.config.step_size < 1.0:
            raise ValueError("step_size must be at least one grid cell")
        if not 0.0 <= self.config.goal_bias <= 1.0:
            raise ValueError("goal_bias must be between 0 and 1")

    @staticmethod
    def distance(p1, p2):
        return float(np.hypot(p1.x - p2.x, p1.y - p2.y))

    def get_nearest_neighbor(self, point):
        return min(self.nodes, key=lambda node: self.distance(node, point))

    def sample_fn(self, goal):
        if self.rng.random() < self.config.goal_bias:
            return Node(goal.x, goal.y)
        return Node(
            int(self.rng.integers(0, self.map.height)),
            int(self.rng.integers(0, self.map.width)),
        )

    def steer(self, nearest, target):
        distance = self.distance(nearest, target)
        if distance == 0.0:
            return None

        scale = min(self.config.step_size / distance, 1.0)
        point = Node(
            int(round(nearest.x + (target.x - nearest.x) * scale)),
            int(round(nearest.y + (target.y - nearest.y) * scale)),
        )
        return None if point.x == nearest.x and point.y == nearest.y else point

    def edge_is_free(self, start, end):
        """Check every grid cell crossed by a straight tree edge."""
        steps = max(abs(end.x - start.x), abs(end.y - start.y))
        previous = start
        for step in range(1, steps + 1):
            point = Node(
                int(round(start.x + (end.x - start.x) * step / steps)),
                int(round(start.y + (end.y - start.y) * step / steps)),
            )
            if point.x != previous.x and point.y != previous.y:
                if not self.map.is_free(previous.x, point.y):
                    return False
                if not self.map.is_free(point.x, previous.y):
                    return False
            if not self.map.is_free(point.x, point.y):
                return False
            previous = point
        return True

    @staticmethod
    def extract(node):
        path = []
        while node is not None:
            path.append((node.x, node.y))
            node = node.parent
        path.reverse()
        return path

    def rrt(self, start, goal):
        start_node = Node(start[0], start[1])
        goal_node = Node(goal[0], goal[1])
        if not self.map.is_free(start_node.x, start_node.y):
            return None, []
        if not self.map.is_free(goal_node.x, goal_node.y):
            return None, []

        self.nodes = [start_node]
        if start == goal:
            return [(start_node.x, start_node.y)], [(start_node.x, start_node.y)]

        for _ in range(self.config.max_iter):
            sampled_point = self.sample_fn(goal_node)
            nearest_point = self.get_nearest_neighbor(sampled_point)
            new_point = self.steer(nearest_point, sampled_point)
            if new_point is None or not self.edge_is_free(nearest_point, new_point):
                continue

            new_point.parent = nearest_point
            self.nodes.append(new_point)
            if self.distance(new_point, goal_node) <= self.config.step_size:
                if self.edge_is_free(new_point, goal_node):
                    goal_node.parent = new_point
                    self.nodes.append(goal_node)
                    return self.extract(goal_node), [(node.x, node.y) for node in self.nodes]

        return None, [(node.x, node.y) for node in self.nodes]
