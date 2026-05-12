import heapq
import math

import numpy as np
import matplotlib.pyplot as plt


def create_map():
    grid = np.zeros((50, 50))
    # 添加障碍物
    grid[10:20, 15:18] = 1
    return grid


def heuristic(a, b):
    # 曼哈顿距离
    return abs(a[0] - b[0]) + abs(a[1] - b[1])
    # 欧式距离
    # return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)


def get_path(came_from, start, goal):
    if start == goal:
        return [start]
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = came_from[cur]
    path.append(start)
    return path[::-1]


def astar(grid, start, goal):
    rows, cols = grid.shape
    motions = [(-1, 0, 1.0),  # 上
               (1, 0, 1.0),  # 下
               (0, -1, 1.0),  # 左
               (0, 1, 1.0),  # 右
               (-1, -1, math.sqrt(2)),
               (-1, 1, math.sqrt(2)),
               (1, -1, math.sqrt(2)),
               (1, 1, math.sqrt(2))]

    open_list = []
    g_cost = {start: 0.0}
    f_cost = {start: g_cost[start] + heuristic(start, goal)}
    heapq.heappush(open_list, (f_cost[start], start))
    close_set = set()
    came_from = {}

    while len(open_list) > 0:
        _, current = heapq.heappop(open_list)
        if current in close_set:
            continue

        if current == goal:
            path = [current]
            while current in came_from:
                current = came_from[current]
                path.append(current)
            path.reverse()
            return path

        close_set.add(current)

        for dx, dy, cost in motions:
            new_x = current[0] + dx
            new_y = current[1] + dy
            neighbor = (new_x, new_y)
            if not (0 <= new_x < rows and 0 <= new_y < cols):
                continue
            if grid[new_x, new_y] == 1:
                continue
            if g_cost.get(neighbor, float('inf')) > g_cost[current] + cost:
                g_cost[neighbor] = g_cost[current] + cost
                came_from[neighbor] = current
                f_cost[neighbor] = g_cost[neighbor] + heuristic(neighbor, goal)
                heapq.heappush(open_list, (f_cost[neighbor], neighbor))

    return None


def plot_result(grid, start, goal, path=None):
    plt.figure(figsize=(8, 8))
    plt.imshow(grid, cmap='gray_r', origin='lower')
    plt.plot(start[1], start[0], 'go', markersize=8, label='Start')
    plt.plot(goal[1], goal[0], 'ro', markersize=8, label='Goal')
    if path:
        path = np.array(path)
        plt.plot(path[:, 1], path[:, 0], 'b-', linewidth=2, label='A* Path')
    plt.legend()
    plt.title("A* Path Planning on Grid Map")
    plt.grid(True)
    plt.show()


def main():
    grid = create_map()
    start = (0, 0)
    goal = (45, 35)
    path = astar(grid, start, goal)
    if path is None:
        print("未找到路径")
    else:
        print("找到路径，路径长度（节点数）:", len(path))
    plot_result(grid, start, goal, path)


if __name__ == "__main__":
    main()
