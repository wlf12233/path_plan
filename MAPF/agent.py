from kit import a_star


class Agent:
    def __init__(self, Id, position, start, goal):
        """
        :param Id:
        :param start: 起始位置
        :param goal: 目标位置
        """
        self.position = position
        self.path = []  # 拥有的路径
        self.start = start
        self.goal = goal
        self.id = Id

    def find_path(self, environment):
        """
        使用A*算法查找路径
        :param environment: Environment对象
        """
        self.path = a_star.a_star(self.start, self.goal, environment)
