import matplotlib

matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import math
from matplotlib.patches import Polygon
import random

X_LIMIT = (0, 100)
Y_LIMIT = (0, 100)
STEP_SIZE = 25
MAX_ITER = 200
NEIGHBOR_RADIUS = 10.0
GOAL_RADIUS = 3.0

OBSTACLES = [
    (20, 20, 20, 10),
    (50, 50, 10, 30),
    (70, 20, 20, 20)
]

START = (5, 5)
GOAL = (90, 90)


class Node:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.parent = None
        self.cost = 0.0  # 从起点到该节点的累计代价


def distance(p1, p2):
    return math.hypot(p1.x - p2.x, p1.y - p2.y)


def random_point():
    return Node(random.uniform(*Y_LIMIT), random.uniform(*Y_LIMIT))


def nearest(nodes, node):
    return min(nodes, key=lambda n: distance(n, node))


def steer(from_node, to_node, step_size):
    d = distance(from_node, to_node)
    if d < step_size:
        return Node(to_node.x, to_node.y)
    else:
        theta = math.atan2(to_node.y - from_node.y, to_node.x - from_node.x)
        return Node(from_node.x + step_size * math.cos(theta), from_node.y + step_size * math.sin(theta))


# 线性插值
def collision(node1, node2, obstacles):
    for (ox, oy, w, h) in obstacles:
        # 沿着线段 node1→node2 每隔一点取样坐标。
        for t in [i / 10.0 for i in range(11)]:
            x = node1.x + t * (node2.x - node1.x)
            y = node1.y + t * (node2.y - node1.y)
            if ox <= x <= ox + w and oy <= y <= oy + h:
                return True
    return False


def find_neighbors(nodes, new_node, radius):
    return [n for n in nodes if distance(n, new_node) <= radius]


def extract_path(node):
    path = [(node.x, node.y)]
    while node.parent:
        node = node.parent
        path.append((node.x, node.y))
    return path[::-1]


def rrt_star(start, goal, obstacles, max_iter=MAX_ITER, step_size=STEP_SIZE, radius=NEIGHBOR_RADIUS):
    start_node = Node(*start)
    nodes = [start_node]

    for i in range(max_iter):
        x_rand = random_point()
        x_nearest = nearest(nodes, x_rand)
        x_new = steer(x_nearest, x_rand, step_size)

        if collision(x_nearest, x_new, obstacles):
            continue

        neighbors = find_neighbors(nodes, x_new, radius)

        # 选择代价最小的父节点
        min_parent = x_nearest
        min_cost = x_nearest.cost + distance(x_nearest, x_new)
        for nb in neighbors:
            cost_through_nb = nb.cost + distance(nb, x_new)
            if cost_through_nb < min_cost and not collision(nb, x_new, obstacles):
                min_cost = cost_through_nb
                min_parent = nb

        x_new.parent = min_parent
        x_new.cost = min_cost
        nodes.append(x_new)

        # 重连邻居
        for nb in neighbors:
            new_cost = x_new.cost + distance(x_new, nb)
            if new_cost < nb.cost and not collision(x_new, nb, obstacles):
                nb.cost = new_cost
                nb.parent = x_new

        if distance(x_new, Node(*goal)) < GOAL_RADIUS:
            goal_node = Node(*goal)
            goal_node.parent = x_new
            goal_node.cost = x_new.cost + distance(x_new, goal_node)
            nodes.append(goal_node)
            return extract_path(goal_node), nodes
    return None, nodes


# -----------------------------
# 可视化
# -----------------------------
def draw_path(path, nodes, obstacles, title="RRT* Path Planning"):
    plt.figure(figsize=(8, 8))
    for (ox, oy, w, h) in obstacles:
        plt.fill([ox, ox + w, ox + w, ox], [oy, oy, oy + h, oy + h], 'k')
    for node in nodes:
        if node.parent:
            plt.plot([node.x, node.parent.x], [node.y, node.parent.y], "-g", linewidth=0.6)
    if path:
        px, py = zip(*path)
        plt.plot(px, py, '-r', linewidth=2)
    plt.plot(START[0], START[1], "bo", markersize=8, label="Start")
    plt.plot(GOAL[0], GOAL[1], "ro", markersize=8, label="Goal")
    plt.xlim(X_LIMIT)
    plt.ylim(Y_LIMIT)
    plt.grid(True)
    plt.title(title)
    plt.legend()
    plt.show()



if __name__ == "__main__":
    path, nodes = rrt_star(START, GOAL, OBSTACLES)
    if path:
        print("找到路径，节点数:", len(nodes))
        print("路径长度:", sum(math.hypot(path[i + 1][0] - path[i][0], path[i + 1][1] - path[i][1])
                               for i in range(len(path) - 1)))
    else:
        print("未找到路径")
    draw_path(path, nodes, OBSTACLES)
