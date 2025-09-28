import numpy as np
import heapq
from MAPF.environment import Environment
from MAPF.visual import *


class Node:
    def __init__(self, position, parent=None):
        self.position = position  # 节点的坐标
        self.parent = parent  # 父节点
        self.g = 0  # 从起点到该节点的成本
        self.h = 0  # 从该节点到目标节点的启发式估计成本
        self.f = 0  # 总成本

    def __lt__(self, other):
        return self.f < other.f  # 用于优先队列比较, 依据 f 值排序


def heuristic(current, goal):
    """
    计算曼哈顿距离作为启发函数
    :param current: 当前位置(row,col)
    :param goal: 目标位置
    :return:
    """
    return abs(current[0] - goal[0]) + abs(current[1] - goal[1])


def a_star(start, goal, environment):
    open_list = []
    closed_list = set()

    start_node = Node(start)
    goal_node = Node(goal)

    heapq.heappush(open_list, start_node)  # 将起点加入优先队列

    while open_list:
        current_node = heapq.heappop(open_list)  # 取出 f 值最低的节点
        closed_list.add(current_node.position)  # 将当前节点加入闭合列表

        # 如果到达目标节点
        if current_node.position == goal_node.position:
            return reconstruct_path(current_node)

        # 生成邻接节点
        neighbors = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # 上、右、下、左四个方向
        for new_position in neighbors:
            node_position = (current_node.position[0] + new_position[0], current_node.position[1] + new_position[1])

            # 检查节点是否在网格内、是否可穿越，且未在闭合列表中
            if environment.is_valid_move(node_position) and node_position not in closed_list:

                neighbor_node = Node(node_position, current_node)
                neighbor_node.g = current_node.g + 1  # g值加1（移动到相邻节点）
                neighbor_node.h = heuristic(current_node.position, neighbor_node.position)
                neighbor_node.f = neighbor_node.g + neighbor_node.h  # 计算 f 值

                # 检查是否在开放列表中，比较 f 值
                if add_to_open(open_list, neighbor_node):
                    heapq.heappush(open_list, neighbor_node)

    return None  # 如果没有找到路径


def reconstruct_path(current_node):
    path = []
    while current_node:
        path.append(current_node.position)
        current_node = current_node.parent
    return path[::-1]


def add_to_open(open_list, neighbor_node):
    for node in open_list:
        if neighbor_node.position == node.position and neighbor_node.g >= node.g:
            return False  # 如果相同节点 g 值大于已在开放列表节点，不加入
    return True  # 否则加入开放列表
