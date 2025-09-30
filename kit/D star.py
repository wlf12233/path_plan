import heapq

# 网格地图示例：0=空地, 1=障碍
grid = [
    [0, 0, 0, 0, 0],
    [0, 1, 1, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 1, 1, 0],
    [0, 0, 0, 0, 0]
]

rows, cols = len(grid), len(grid[0])

# 起点和目标
start = (0, 0)
goal = (4, 4)


# 邻居函数
def neighbors(node):
    x, y = node
    dirs = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # 上下左右
    result = []
    for dx, dy in dirs:
        nx, ny = x + dx, y + dy
        if 0 <= nx < rows and 0 <= ny < cols and grid[nx][ny] == 0:
            result.append((nx, ny))
    return result


# 启发式函数：曼哈顿距离
def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


# D* Lite 简化实现
def d_star_lite(start, goal):
    g = {(i, j): float('inf') for i in range(rows) for j in range(cols)}
    rhs = {(i, j): float('inf') for i in range(rows) for j in range(cols)}

    rhs[goal] = 0
    open_list = []
    heapq.heappush(open_list, (heuristic(start, goal), goal))  # (key, node)

    while open_list:
        key, u = heapq.heappop(open_list)

        # 计算最小 rhs
        min_rhs = min([g[n] + 1 for n in neighbors(u)] + [0 if u == goal else float('inf')])
        if g[u] > min_rhs:
            g[u] = min_rhs
        elif g[u] < min_rhs:
            g[u] = float('inf')

        # 更新邻居
        for n in neighbors(u):
            n_rhs = min([g[nb] + 1 for nb in neighbors(n)])
            if rhs[n] != n_rhs:
                rhs[n] = n_rhs
                heapq.heappush(open_list, (heuristic(start, n), n))

    # 从 start 提取路径
    path = [start]
    current = start
    while current != goal:
        next_nodes = neighbors(current)
        if not next_nodes:
            print("无法到达目标")
            return path
        current = min(next_nodes, key=lambda n: g[n] + 1)
        path.append(current)
    return path


# 测试
path = d_star_lite(start, goal)
print("路径:", path)

# 动态添加障碍
grid[2][2] = 1  # 新障碍
print("添加障碍后重新规划:")
path2 = d_star_lite(start, goal)
print("路径:", path2)
