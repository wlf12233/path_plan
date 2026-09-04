from dataclasses import dataclass

import numpy as np


# RRT* 在邻域内选择代价更低的父节点，重新连接已有节点（Rewire）
# RRT 找到一条可行路线后即可返回；RRT* 在给定迭代次数内持续优化，并返回当前找到的代价最小路线。
@dataclass
class RRTConfig:
    max_iter: int = 10000
    step_size: float = 1.0
    goal_bias: float = 0.10
    neighborhood_radius: float = 5.0  # 邻域阈值


@dataclass
class Node:
    # Coordinates use the GridMap convention: row, column.
    x: int
    y: int
    cost: float = 0.0
    parent: object = None


# 快速拓展随机数
class RRTStarPlaner:
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
        if self.config.neighborhood_radius <= 0.0:
            raise ValueError("neighborhood_radius must be positive")

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

    # 树刚开始节点少，需要看较大范围
    # 节点变多后，局部节点密集，只需要看附近节点
    def get_neighbor_hood(self):
        return min(self.config.neighborhood_radius,
                   2.0 * self.config.step_size * np.sqrt(np.log(len(self.nodes) + 1 / len(self.nodes))))

    def get_neighbor(self, point):
        neighbors = [node for node in self.nodes if self.distance(node, point) <= self.get_neighbor_hood()]
        return neighbors

    def update_descendant_costs(self, parent):
        """Propagate a changed cost through the current tree."""
        for child in self.nodes:
            if child.parent is parent:
                child.cost = parent.cost + self.distance(parent, child)
                self.update_descendant_costs(child)

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

    def rrt_star(self, start, goal):
        start_node = Node(start[0], start[1], 0)
        goal_node = Node(goal[0], goal[1], 0)
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
            parent = nearest_point
            neighbors = self.get_neighbor(new_point)
            for neighbor in neighbors:
                candidate_cost = neighbor.cost + self.distance(neighbor, new_point)
                if (
                        self.edge_is_free(neighbor, new_point)
                        and candidate_cost < parent.cost + self.distance(parent, new_point)
                ):
                    parent = neighbor
            new_point.parent = parent
            new_point.cost = self.distance(new_point, parent) + parent.cost
            self.nodes.append(new_point)

            for neighbor in neighbors:
                new_cost = new_point.cost + self.distance(neighbor, new_point)
                if (
                        neighbor is not parent
                        and self.edge_is_free(new_point, neighbor)
                        and neighbor.cost > new_cost
                ):
                    neighbor.parent = new_point
                    neighbor.cost = new_cost
                    self.update_descendant_costs(neighbor)

        goal_candidates = [
            node for node in self.nodes
            if self.distance(node, goal_node) <= self.config.step_size
               and self.edge_is_free(node, goal_node)
        ]
        if goal_candidates:
            best_node = min(
                goal_candidates,
                key=lambda node: node.cost + self.distance(node, goal_node),
            )
            goal_node.parent = best_node
            goal_node.cost = best_node.cost + self.distance(best_node, goal_node)
            self.nodes.append(goal_node)
            print(f"cost:{goal_node.cost}")
            return self.extract(goal_node), [(node.x, node.y) for node in self.nodes]
        return None, [(node.x, node.y) for node in self.nodes]
