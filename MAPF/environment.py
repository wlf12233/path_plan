class Environment:
    def __init__(self, grid):
        """
        初始化环境
        :param grid: 2D列表表示环境地图（0为可行走，1为障碍物）
        """
        self.grid = grid
        self.rows = len(grid)
        self.cols = len(grid[0])
        self.agents = []

    def add_agent(self, agent):
        """
        添加智能体到环境
        :param agent: Agent对象
        """
        self.agents.append(agent)

    def is_valid_move(self, position):
        """
        检查是否可以移动到指定位置
        :param position: 元组（行，列）
        :return: bool
        """
        row, col = position
        if 0 <= row < self.rows and 0 <= col < self.cols:
            return self.grid[row][col] == 0  # 0表示可走
        return False

    def move(self, agents):
        """
        :param agents: Agent对象
        :return:
        """
        for agent in agents:
            if agent.path:
                agent.position = agent.path.pop(0)
