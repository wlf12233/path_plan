import numpy as np
import environment
import visual
import agent
import mapf
from kit import a_star


def main():
    # 定义一个网格，其中 0 为通行区域，1 为障碍物
    grip = np.array([[0, 0, 0, 1, 0],
                     [0, 1, 0, 1, 0],
                     [0, 0, 0, 0, 0],
                     [0, 1, 1, 1, 0],
                     [0, 0, 0, 0, 0]])

    en = environment.Environment(grip)
    en.add_agent(agent.Agent("Robot 1", (0, 0), (0, 0), (4, 3)))
    en.add_agent(agent.Agent("Robot 2", (0, 4), (0, 4), (4, 0)))
    en.add_agent(agent.Agent("Robot 3", (0, 4), (0, 4), (4, 0)))
    conflicts = mapf.run_mapf(en)

    # start = (0, 0)  # 起点
    # goal = (4, 3)  # 终点
    # path1 = a_star.a_star(start, goal, en)
    # agent1 = agent.Agent(1, start, goal)
    # agent1.path = path1
    #
    # start = (1, 0)  # 起点
    # goal = (3, 4)  # 终点
    # agent2 = agent.Agent(2, start, goal)
    # path2 = a_star.a_star(start, goal, en)
    # agent2.path = path2

    visual.plot_robot_paths(en.agents, conflicts)


if __name__ == "__main__":
    main()
