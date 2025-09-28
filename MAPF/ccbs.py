class Node:
    """约束树节点"""

    def __init__(self, constraints, plans, cost):
        self.constraints = constraints  # 约束集合
        self.plans = plans  # 各智能体路径计划
        self.cost = cost  # 总代价（sum-of-costs）


class Action:
    """动作定义"""

    def __init__(self, from_vertex, to_vertex, duration, motion_func, action_type):
        self.from_vertex = from_vertex  # 起始顶点
        self.to_vertex = to_vertex  # 目标顶点
        self.duration = duration  # 持续时间
        self.motion_func = motion_func  # 运动函数：t -> 位置
        self.type = action_type  # "move" 或 "wait"


class Plan:
    """路径计划"""

    def __init__(self, timed_actions):
        self.timed_actions = timed_actions  # 时序动作列表 (action, start_time)
        self.duration = sum(a.duration for (a, t) in timed_actions)  # 总持续时间


def CSIPP(G, start, goal, constraints):
    """
    带约束的安全区间路径规划（基于SIPP）
    constraints: 字典 {(agent, action): (unsafe_start, unsafe_end)}
    返回: 满足约束的最优路径（时间连续）
    """
    # 1. 为每个顶点计算安全区间（考虑等待动作约束）
    safe_intervals = {}
    for v in G.V:
        intervals = [(0.0, float('inf'))]  # 初始安全区间
        # 应用等待动作约束
        for (a, (t_start, t_end)) in constraints.items():
            if a.type == "wait" and a.from_vertex == v:
                # 分割区间：移除[ t_start, t_end )
                new_intervals = []
                for (s, e) in intervals:
                    if e <= t_start or s >= t_end:
                        new_intervals.append((s, e))
                    else:
                        if s < t_start:
                            new_intervals.append((s, t_start))
                        if e > t_end:
                            new_intervals.append((t_end, e))
                intervals = new_intervals
        safe_intervals[v] = intervals

    # 2. A*搜索：状态为 (顶点, 安全区间索引)，代价为到达时间
    # （简化实现，实际需考虑移动动作约束和安全区间衔接）
    # ...（具体实现类似SIPP，结合约束处理移动动作）
    return optimal_path


def compute_unsafe_interval(action_a, time_a, action_b, time_b):
    """
    计算动作a相对于动作b的不安全区间：[start, end)
    在该区间内执行a会与b冲突
    """
    start = time_a
    # 找到最小的t使得a在t开始时与b无冲突
    low = start
    high = time_b + action_b.duration  # 上限：b结束后
    # 二分查找不安全区间终点
    epsilon = 1e-6  # 精度阈值
    while high - low > epsilon:
        mid = (low + high) / 2
        if in_conflict(action_a, mid, action_b, time_b):
            low = mid  # 仍冲突，需延后
        else:
            high = mid  # 无冲突，尝试提前
    return (start, low)


def find_conflict(plans):
    """
    检查多智能体路径是否存在冲突
    plans: 字典 {agent: 路径计划}，路径计划包含时序动作列表 (action, start_time)
    返回: 冲突详情 (agent_i, agent_j, action_i, time_i, action_j, time_j) 或 None
    """
    agents = list(plans.keys())
    for i in range(len(agents)):
        agent_i = agents[i]
        plan_i = plans[agent_i]
        for j in range(i + 1, len(agents)):
            agent_j = agents[j]
            plan_j = plans[agent_j]
            # 检查所有时序动作对的冲突
            for (a_i, t_i) in plan_i.timed_actions:
                for (a_j, t_j) in plan_j.timed_actions:
                    # 检查动作时间是否重叠
                    a_i_end = t_i + a_i.duration
                    a_j_end = t_j + a_j.duration
                    if not (a_i_end <= t_j or a_j_end <= t_i):
                        # 检查碰撞（基于几何形状和运动函数）
                        if in_conflict(a_i, t_i, a_j, t_j):
                            return (agent_i, agent_j, a_i, t_i, a_j, t_j)
    return None


def in_conflict(a_i, t_i, a_j, t_j):
    """检查两个时序动作是否碰撞"""
    # 时间重叠区间
    overlap_start = max(t_i, t_j)
    overlap_end = min(t_i + a_i.duration, t_j + a_j.duration)
    # 采样重叠区间内的时间点检测碰撞（实际可优化为解析计算）
    sample_times = [overlap_start, (overlap_start + overlap_end) / 2, overlap_end]
    for t in sample_times:
        pos_i = a_i.motion_func(t - t_i)  # 相对时间计算位置
        pos_j = a_j.motion_func(t - t_j)
        if is_collision(pos_i, pos_j):  # 基于几何形状的碰撞检测
            return True
    return False


def CCBS(G, S, G_goals):
    """
    G: 环境图 (V, E)
    S: 各智能体起点 (agent -> start_vertex)
    G_goals: 各智能体目标 (agent -> goal_vertex)
    返回: 无冲突的最优路径集合 (sum-of-costs 最小)
    """
    # 1. 初始化：为每个智能体生成初始路径（不考虑其他智能体）
    initial_plans = {}
    for agent in all_agents:
        # 低层搜索：使用A*生成单智能体最短路径（仅移动动作）
        initial_plans[agent] = A_star(G, S[agent], G_goals[agent])

    # 2. 构建约束树（CT）根节点
    root = Node(constraints={}, plans=initial_plans)
    root.cost = sum(plan.duration for plan in initial_plans.values())  # 总代价（sum-of-costs）
    open_list = PriorityQueue()
    open_list.put(root, root.cost)  # 按总代价优先扩展

    # 3. 迭代搜索最优解
    while not open_list.empty():
        current_node = open_list.get()  # 取出代价最小的节点

        # 检查当前解是否无冲突
        conflict = find_conflict(current_node.plans)
        if not conflict:
            return current_node.plans  # 找到最优解

        # 4. 处理冲突：生成子节点
        agent_i, agent_j, action_i, time_i, action_j, time_j = conflict
        # 计算不安全区间
        unsafe_i = compute_unsafe_interval(action_i, time_i, action_j, time_j)
        unsafe_j = compute_unsafe_interval(action_j, time_j, action_i, time_i)

        # 生成子节点i：约束agent_i不执行action_i在[unsafe_i.start, unsafe_i.end)
        constraints_i = current_node.constraints.copy()
        constraints_i[(agent_i, action_i)] = unsafe_i
        new_plan_i = CSIPP(G, S[agent_i], G_goals[agent_i], constraints_i)
        if new_plan_i is not None:
            new_plans_i = current_node.plans.copy()
            new_plans_i[agent_i] = new_plan_i
            new_cost_i = sum(plan.duration for plan in new_plans_i.values())
            child_i = Node(constraints=constraints_i, plans=new_plans_i, cost=new_cost_i)
            open_list.put(child_i, new_cost_i)

        # 生成子节点j：约束agent_j不执行action_j在[unsafe_j.start, unsafe_j.end)
        constraints_j = current_node.constraints.copy()
        constraints_j[(agent_j, action_j)] = unsafe_j
        new_plan_j = CSIPP(G, S[agent_j], G_goals[agent_j], constraints_j)
        if new_plan_j is not None:
            new_plans_j = current_node.plans.copy()
            new_plans_j[agent_j] = new_plan_j
            new_cost_j = sum(plan.duration for plan in new_plans_j.values())
            child_j = Node(constraints=constraints_j, plans=new_plans_j, cost=new_cost_j)
            open_list.put(child_j, new_cost_j)

    return None  # 无解（理论上不会触发，因算法是解完备的）
