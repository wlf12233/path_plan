import numpy as np


def hungarian_algorithm(cost_matrix):
    # 获取行数（工人数量）和列数（任务数量）
    n, m = len(cost_matrix), len(cost_matrix[0])

    # 初始化每个任务的分配状态，-1表示尚未分配
    match_job = [-1] * m  # match_job[j] = i 表示任务 j 被工人 i 分配

    # 初始化每个工人的匹配状态，-1表示尚未匹配
    match_worker = [-1] * n  # match_worker[i] = j 表示工人 i 分配给任务 j

    # 标记每个工人的访问状态
    visited = [False] * n

    # 实现匈牙利算法的 DFS（深度优先搜索）函数
    def dfs(worker):
        for task in range(m):  # 遍历所有任务
            if cost_matrix[worker][task] == 0 and not visited[task]:
                # 如果工人与任务之间的成本为零且任务未被访问过
                visited[task] = True  # 标记任务为已访问
                if match_job[task] == -1 or dfs(match_job[task]):
                    # 如果任务未分配或工人可以找到其他匹配，则实现匹配
                    match_job[task] = worker  # 为任务分配工人
                    match_worker[worker] = task  # 更新工人的分配状态
                    return True
        return False  # 无法找到匹配返回 False

    # 主循环，尝试为每个工人找到匹配
    for worker in range(n):
        # 重置访问状态
        visited = [False] * m
        dfs(worker)  # 尝试为工人进行匹配

    # 返回最终的匹配情况
    return match_worker, match_job  # 返回工人与任务的匹配结果


# 示例成本矩阵
cost_matrix = [
    [4, 2, 8],
    [2, 4, 6],
    [8, 6, 2]
]

matching, cost = hungarian_algorithm(cost_matrix)
print(f"最优匹配: {matching}")
print(f"最小总成本: {cost}")
