def detect_conflicts(paths):
    """
    检测路径冲突
    :param paths: 所有智能体的路径列表
    :return: 冲突列表
    """
    conflicts = []

    position_dict = {}
    # 检测逻辑
    for one in paths:
        for agent, path in one.items():
            for time_stap, position in enumerate(path):
                if time_stap not in position_dict:
                    position_dict[time_stap] = {}
                if position not in position_dict[time_stap]:
                    position_dict[time_stap][position] = []
                position_dict[time_stap][position].append(agent)

        for time_stap, positions in position_dict.items():
            for position, agents in positions.items():
                if len(agents) > 1:
                    conflicts.append({
                        "time": time_stap,
                        "position": position,
                        "agents": agents
                    })

    return conflicts


def solve_conflicts(conflicts, robots):
    """
    解决路径上的冲突
    :param conflicts: 冲突列表
    :param robots: 智能体列表
    """
    # 冲突解决策略
    for conflict in conflicts:
        t = conflict["time"]
        position = conflict["position"]
        agents = conflict["agents"]
        for robot in robots:
            if robot.id == agents[1] and t < len(robot.path):
                current_position = robot.path[t]
                neighbors = [(0, 1), (1, 0), (0, -1), (-1, 0)]
                for neighbor in neighbors:
                    new_position = (current_position[0] + neighbor[0], current_position[1] + neighbor[1])
                    if new_position not in [r.path[t] for r in robots if r.id in agents]:
                        robot.path[t] = new_position
                        break

    return robots


def RHCR(environment, time_horizon=3):
    # 简单实现
    for t in range(10):  # 模拟10个时间步
        for agent in environment.agent:
            if agent.replan_counter == 0:
                agent.agent.find_path(environment)
                agent.replan_counter = time_horizon
            else:
                agent.replan_counter -= 1

        # 检测冲突
        paths = [{agent.id: agent.path} for agent in environment.agents]
        conflicts = detect_conflicts(paths)

        # 解决冲突
        if conflicts:
            solve_conflicts(conflicts, environment.agents)

        environment.move()  # 移动机器人


def run_mapf(environment):
    for agent in environment.agents:
        agent.find_path(environment)

    for _ in range(10):
        paths = [{agent.id: agent.path} for agent in environment.agents]
        conflicts = detect_conflicts(paths)
        if not conflicts:
            break
        solve_conflicts(conflicts, environment.agents)

    paths = [{agent.id: agent.path} for agent in environment.agents]
    conflicts = detect_conflicts(paths)
    return conflicts
