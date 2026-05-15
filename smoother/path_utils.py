import numpy as np


def remove_duplicate_points(path):
    if path is None or len(path) == 0:
        return None
    new_path = [path[0]]
    for p in path[1:]:
        if p != new_path[-1]:
            new_path.append(p)
    return new_path


# 将 (row, col) 转换为控制常用的 (x, y)
def path_to_xy(path):
    if path is None or len(path) == 0:
        return np.array([]), np.array([])
    x = np.array([p[1] for p in path], dtype=float)
    y = np.array([p[0] for p in path], dtype=float)
    return x, y


def simplify_path(path):
    """
    简单路径简化：
    去掉连续共线的中间点
    输入: [(row, col), ...]
    输出: 简化后的路径
    """
    if path is None or len(path) == 0:
        return None
    new_path = [path[0]]
    for i in range(1, len(path) - 1):
        prev = np.array(path[i - 1])
        curr = np.array(path[i])
        p_next = np.array(path[i + 1])
        v1 = curr - prev
        v2 = p_next - curr
        # 判断方向是否一致（共线且同向）保留首位就够了
        if np.array_equal(v1, v2):
            continue
        new_path.append(path[i])
    new_path.append(path[-1])
    return new_path
