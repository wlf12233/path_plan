import matplotlib.pyplot as plt
import numpy as np


class GridMap:
    def __init__(self, width, height):
        self.width = width  # 列数
        self.height = height  # 行数
        self.grid = np.zeros((self.height, self.width), dtype=int)

    def in_bounds(self, row, col):
        return 0 <= row < self.height and 0 <= col < self.width

    def is_obstacle(self, row, col):
        return self.grid[row, col] == 1

    def is_free(self, row, col):
        return self.in_bounds(row, col) and not self.is_obstacle(row, col)

    def add_rect_obstacle(self, row_min, row_max, col_min, col_max):
        self.grid[row_min:row_max, col_min:col_max] = 1

    def add_obstacle(self, row, col):
        self.grid[row, col] = 1
