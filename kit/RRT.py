import matplotlib

matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import math
from matplotlib.patches import Polygon
import random

X_LIMIT = (0, 100)
Y_LIMIT = (0, 100)
# 起点和目标
START = (5, 5)
GOAL = (90, 90)
GOAL_RADIUS = 10.0

# 障碍物列表，每个障碍物用矩形(x, y, w, h)表示
OBSTACLES = [
    (20, 20, 20, 10),
    (50, 50, 10, 30),
    (70, 20, 20, 20)
]


class Node:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.parent = None


def distance(p1, p2):
    return math.hypot(p1.x - p2.x, p1.y - p2.y)


def random_point():
    return Node(random.uniform(*X_LIMIT), random.uniform(*Y_LIMIT))


def nearest(nodes, node):
    return min(nodes, key=lambda n: distance(n, node))


def steer(from_node, to_node, step_size):
    d = distance(from_node, to_node)
    if d < step_size:
        return Node(to_node.x, to_node.y)
    else:
        theta = math.atan2(to_node.y - from_node.y, to_node.x - from_node.x)
        return Node(from_node.x + step_size * math.cos(theta), from_node.y + step_size * math.sin(theta))


def collision():
    return False


def extract_path(node):
    path = [(node.x, node.y)]
    while node.parent:
        node = node.parent
        path.append((node.x, node.y))
    return path[:-1]


def RRT(start, goal, obstacles, max_iter, step_size):
    nodes = [Node(*start)]

    for _ in range(max_iter):
        rand_node = random_point()
        nearest_node = nearest(nodes, rand_node)
        new_node = steer(nearest_node, rand_node, step_size)

        if not collision():
            new_node.parent = nearest_node
            nodes.append(new_node)
            # RRT 是在 连续空间（continuous space） 中采样的，比如：(34.2618, 72.9453) 随机采样点 几乎不可能精确等于目标坐标，因为这概率趋近于 0。所以判断在一定的误差内
            if distance(new_node, Node(*goal)) < GOAL_RADIUS:
                goal_node = Node(*goal)
                goal_node.parent = new_node
                nodes.append(goal_node)
                return extract_path(goal_node), nodes
    return None, nodes


def draw_path(path, nodes, obstacles):
    plt.figure(figsize=(8, 8))
    # 绘制障碍物
    for (ox, oy, w, h) in obstacles:
        plt.fill([ox, ox + w, ox + w, ox], [oy, oy, oy + h, oy + h], 'k')
    # 绘制RRT树
    for node in nodes:
        if node.parent:
            plt.plot([node.x, node.parent.x], [node.y, node.parent.y], "-g")
    # 绘制路径
    if path:
        px, py = zip(*path)
        plt.plot(px, py, '-r', linewidth=2)
    # 绘制起点和终点
    plt.plot(START[0], START[1], "bo", markersize=8)
    plt.plot(GOAL[0], GOAL[1], "ro", markersize=8)
    plt.xlim(X_LIMIT)
    plt.ylim(Y_LIMIT)
    plt.title("RRT Path Planning")
    plt.grid(True)
    plt.show()


# -----------------------------
# 运行示例
# -----------------------------
if __name__ == "__main__":
    path, nodes = RRT(START, GOAL, None, 200, 10)
    if path:
        print("找到路径，长度:", len(path))
    else:
        print("未找到路径")
    draw_path(path, nodes, OBSTACLES)
