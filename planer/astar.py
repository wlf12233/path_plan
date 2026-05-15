import heapq
import math


class AstarPlanner:

    def __init__(self, grid):
        self.grid = grid

    def heuristic(self, a, b):
        # 曼哈顿距离
        # return abs(a[0] - b[0]) + abs(a[1] - b[1])
        # 欧式距离
        # return math.sqrt((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2)
        # 对角距离（Octile distance）
        dx = abs(a[0] - b[0])
        dy = abs(a[1] - b[1])
        return max(dx, dy) + (math.sqrt(2) - 1) * min(dx, dy)

    def get_motions(self):
        motions = [(-1, 0, 1.0),  # 上
                   (1, 0, 1.0),  # 下
                   (0, -1, 1.0),  # 左
                   (0, 1, 1.0),  # 右
                   (-1, -1, math.sqrt(2)),
                   (-1, 1, math.sqrt(2)),
                   (1, -1, math.sqrt(2)),
                   (1, 1, math.sqrt(2))]
        return motions

    def can_move(self, neighbor, current, dx, dy):
        nr, nc = neighbor
        if not self.grid.in_bounds(nr, nc):
            return False
        if self.grid.is_obstacle(nr, nc):
            return False
        # 防止斜向穿角
        adj1 = current[0] + dx, current[1]
        adj2 = current[0], current[1] + dy
        if not self.grid.is_free(adj1[0], adj1[1]) or not self.grid.is_free(adj2[0], adj2[1]):
            return False
        return True

    def reconstruct_path(self, current, came_from):
        path = [current]
        while current in came_from:
            current = came_from[current]
            path.append(current)
        return path[::-1]

    def plan(self, start, goal):
        if not self.grid.is_free(start[0], start[1]):
            return None, set()
        if not self.grid.is_free(goal[0], goal[1]):
            return None, set()
        if start == goal:
            return [start], {start}
        open_list = []
        came_from = {}
        close_set = set()
        g_cost = {start: 0.0}
        f_cost = {start: g_cost[start] + self.heuristic(start, goal)}
        heapq.heappush(open_list, (f_cost[start], start))
        motions = self.get_motions()

        while len(open_list) > 0:
            _, current = heapq.heappop(open_list)
            if current in close_set:
                continue
            close_set.add(current)
            if current == goal:
                return self.reconstruct_path(current, came_from), set(g_cost.keys())
            for dx, dy, cost in motions:
                new_x = current[0] + dx
                new_y = current[1] + dy
                neighbor = (new_x, new_y)
                if not self.can_move(neighbor, current, dx, dy):
                    continue
                if g_cost.get(neighbor, float('inf')) > g_cost[current] + cost:
                    g_cost[neighbor] = g_cost[current] + cost
                    f_cost[neighbor] = g_cost[neighbor] + self.heuristic(neighbor, goal)
                    heapq.heappush(open_list, (f_cost[neighbor], neighbor))
                    came_from[neighbor] = current
        return None, set(g_cost.keys())
