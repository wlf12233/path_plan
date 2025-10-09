import heapq
import math

# ========== 基本配置 ==========
INF = float('inf')
DIRS = [(1,0), (-1,0), (0,1), (0,-1)]  # 四方向移动

# ========== 工具函数 ==========
def heuristic(a, b):
    """启发函数：曼哈顿距离"""
    return abs(a[0]-b[0]) + abs(a[1]-b[1])

# ========== 优先队列封装 ==========
class PriorityQueue:
    def __init__(self):
        self.elements = []
    def empty(self):
        return not self.elements
    def put(self, item, priority):
        heapq.heappush(self.elements, (priority, item))
    def get(self):
        return heapq.heappop(self.elements)
    def top_key(self):
        if self.elements:
            return self.elements[0][0]
        return (INF, INF)
    def remove(self, item):
        for i, (_, it) in enumerate(self.elements):
            if it == item:
                self.elements.pop(i)
                heapq.heapify(self.elements)
                return

# ========== D* Lite 核心类 ==========
class DStarLite:
    def __init__(self, grid, start, goal):
        self.grid = grid
        self.start = start
        self.goal = goal
        self.k_m = 0
        self.g = {}
        self.rhs = {}
        self.U = PriorityQueue()
        self.width = len(grid[0])
        self.height = len(grid)
        for y in range(self.height):
            for x in range(self.width):
                self.g[(x,y)] = INF
                self.rhs[(x,y)] = INF
        self.rhs[self.goal] = 0
        self.U.put(self.goal, self.calculate_key(self.goal))

    # ----------- 辅助函数 -----------
    def neighbors(self, s):
        x, y = s
        for dx, dy in DIRS:
            nx, ny = x + dx, y + dy
            if 0 <= nx < self.width and 0 <= ny < self.height:
                if self.grid[ny][nx] == 0:  # 可行走
                    yield (nx, ny)

    def cost(self, a, b):
        """如果是障碍，代价无限"""
        if self.grid[b[1]][b[0]] == 1:
            return INF
        return 1

    def calculate_key(self, s):
        val = min(self.g[s], self.rhs[s])
        return (val + heuristic(self.start, s) + self.k_m, val)

    # ----------- 核心更新函数 -----------
    def update_vertex(self, u):
        if u != self.goal:
            self.rhs[u] = min(
                [self.cost(u, s2) + self.g[s2] for s2 in self.neighbors(u)]
                or [INF]
            )
        self.U.remove(u)
        if self.g[u] != self.rhs[u]:
            self.U.put(u, self.calculate_key(u))

    # ----------- 计算最短路径 -----------
    def compute_shortest_path(self):
        while (self.U.top_key() < self.calculate_key(self.start)) or (self.rhs[self.start] != self.g[self.start]):
            k_old, u = self.U.get()
            if k_old < self.calculate_key(u):
                self.U.put(u, self.calculate_key(u))
            elif self.g[u] > self.rhs[u]:
                self.g[u] = self.rhs[u]
                for s in self.neighbors(u):
                    self.update_vertex(s)
            else:
                g_old = self.g[u]
                self.g[u] = INF
                for s in self.neighbors(u) | {u}:
                    self.update_vertex(s)

    # ----------- 主执行函数 -----------
    def plan(self):
        self.compute_shortest_path()
        path = [self.start]
        s = self.start
        while s != self.goal:
            if self.g[s] == INF:
                print("No path found!")
                return path
            # 选取代价最小的邻居
            s = min(
                self.neighbors(s),
                key=lambda n: self.cost(s, n) + self.g[n]
            )
            path.append(s)
        return path

    def move_and_rescan(self, new_start, changed_edges):
        """机器人移动并重新规划"""
        self.k_m += heuristic(self.start, new_start)
        self.start = new_start
        for (u, v, new_cost) in changed_edges:
            if new_cost == INF:
                self.grid[v[1]][v[0]] = 1  # 添加障碍
            else:
                self.grid[v[1]][v[0]] = 0  # 移除障碍
            self.update_vertex(u)
        self.compute_shortest_path()

# ========== 示例测试 ==========
if __name__ == "__main__":
    # 0 = 可通行, 1 = 障碍
    grid = [
        [0,0,0,0,0,0],
        [0,1,1,1,0,0],
        [0,0,0,1,0,0],
        [0,1,0,0,0,0],
        [0,0,0,0,0,0],
    ]
    start = (0, 4)
    goal = (5, 0)
    planner = DStarLite(grid, start, goal)
    path = planner.plan()
    print("Initial path:", path)

    # 模拟动态障碍出现
    print("\nAdding obstacle at (2,2)...\n")
    planner.move_and_rescan(new_start=(1,4), changed_edges=[((2,2),(2,2),INF)])
    new_path = planner.plan()
    print("Replanned path:", new_path)
