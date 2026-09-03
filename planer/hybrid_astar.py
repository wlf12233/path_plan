import heapq
from dataclasses import dataclass
import numpy as np


@dataclass
class HybridAstarConfig:
    xy_resolution: float = 1.0  # 用于把连续状态 (x, y) 变成索引：
    yaw_resolution: float = np.deg2rad(10)
    step_size: float = 1.0  # 每次扩展前进多远。
    wheel_base: float = 1.0  # 轴距
    max_steer: float = np.deg2rad(30)  # 最大转向角。
    pos_goal_tol: float = 1.0  # 到目标的位置容差。
    yaw_goal_tol: float = np.deg2rad(10)  # 到目标的航向角容差。
    steer_samples: float = 10  # 转角采样个数。 3 对应 [-max, 0, max] 5 对应 [-max, -max/2, 0, max/2, max]
    w_steer: float = 0.1  # 转向惩罚。转得越大，代价越高。
    w_steer_change: float = 0.2  # 转向变化惩罚。如果当前转角和上一步差别很大，就额外惩罚。


@dataclass
class Node:
    x: float
    y: float
    yaw: float
    g: float
    h: float
    parent: object
    steer: float

    @property
    def f(self):
        return self.g + self.h

    def __lt__(self, other):
        return self.f < other.f


class HybridAstarPlanner:
    def __init__(self, grid_map, config=None):
        self.grid_map = grid_map
        self.config = config if config is not None else HybridAstarConfig()

    def plan(self, start, goal):
        start_x, start_y = start[1], start[0]
        start_yaw = start[2] if len(start) > 2 else 0.0
        goal_x, goal_y = goal[1], goal[0]
        goal_yaw = goal[2] if len(goal) > 2 else 0.0
        if not self.grid_map.is_free(int(round(start_y)), int(round(start_x))):
            return None, None
        if not self.grid_map.is_free(int(round(goal_y)), int(round(goal_x))):
            return None, None
        open_list = []
        closed_list = {}
        g_list = {}
        start_key = self.state_to_index(start_x, start_y, start_yaw)
        g_list[start_key] = 0.0
        start_node = Node(x=start_x, y=start_y, yaw=normalize_angle(start_yaw), g=0.0,
                          h=self.heuristic(start_x, start_y, goal_x, goal_y),
                          parent=None, steer=0.0)
        heapq.heappush(open_list, (start_node.f, start_node))
        while open_list:
            f, node = heapq.heappop(open_list)
            cur_key = self.state_to_index(node.x, node.y, node.yaw)
            if cur_key in closed_list:
                continue
            closed_list[cur_key] = node
            if self.is_goal(node, goal_x, goal_y, goal_yaw):
                path = self.reconstruct_path(node)
                return path, list(closed_list.keys())
            for steer in self.get_motions():
                new_x, new_y, new_yaw = self.simulate_motion(node.x, node.y, node.yaw, steer)
                if not self.is_valid(new_x, new_y):
                    continue
                step_cost = self.config.step_size
                steer_cost = self.config.w_steer * abs(steer)
                steer_change_cost = self.config.w_steer_change * abs(steer - node.steer)
                new_g = node.g + step_cost + steer_change_cost + steer_cost
                new_h = self.heuristic(new_x, new_y, goal_x, goal_y)
                child_key = self.state_to_index(new_x, new_y, new_yaw)
                child_node = Node(x=new_x, y=new_y, yaw=new_yaw, g=new_g, h=new_h, parent=node, steer=steer)
                if child_key in closed_list:
                    continue
                if child_key not in g_list or g_list[child_key] > new_g:
                    g_list[child_key] = new_g
                    heapq.heappush(open_list, (child_node.f, child_node))
        return None, list(closed_list.keys())

    def get_motions(self):
        c = self.config
        if c.steer_samples == 3:
            return [-c.max_steer, 0, c.max_steer]
        else:
            return np.linspace(-c.max_steer, c.max_steer, c.steer_samples)

    def simulate_motion(self, x, y, yaw, steer):
        c = self.config
        x = x + c.step_size * np.cos(yaw)
        y = y + c.step_size * np.sin(yaw)
        yaw = yaw + c.step_size / c.wheel_base * np.tan(steer)
        yaw = normalize_angle(yaw)
        return x, y, yaw

    def is_valid(self, x, y):
        return self.grid_map.is_free(int(round(y)), int(round(x)))

    def heuristic(self, start_x, start_y, goal_x, goal_y):
        return np.hypot(goal_x - start_x, goal_y - start_y)

    def is_goal(self, node, goal_x, goal_y, goal_yaw):
        dist_xy = np.hypot(goal_x - node.x, goal_y - node.y)
        dist_yaw = abs(normalize_angle(goal_yaw - node.yaw))
        return dist_xy <= self.config.pos_goal_tol and dist_yaw <= self.config.yaw_goal_tol

    def reconstruct_path(self, node):
        path = []
        current = node
        while current is not None:
            path.append((current.y, current.x, current.yaw))
            current = current.parent
        path.reverse()
        return path

    def state_to_index(self, x, y, yaw):
        ix = int(round(x / self.config.xy_resolution))
        iy = int(round(y / self.config.xy_resolution))
        iyaw = int(round(normalize_angle(yaw) / self.config.yaw_resolution))
        return ix, iy, iyaw


def normalize_angle(angle):
    while angle > np.pi:
        angle -= 2 * np.pi
    while angle < -np.pi:
        angle += 2 * np.pi
    return angle
