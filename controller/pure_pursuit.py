import numpy as np

L = 2.5
dt = 0.1
max_steer = np.radians(30)


def find_nearest_point(x, y, path_x, path_y, last_idx=0, window=30):
    px = np.asarray(path_x[last_idx:window + last_idx])
    py = np.asarray(path_y[last_idx:window + last_idx])
    return last_idx + int(np.argmin(np.hypot(px - x, py - y)))


def find_lookahead_point(nearest_idx, path_x, path_y, x, y, Ld):
    c_index = nearest_idx
    while c_index < len(path_x) - 1:
        if np.hypot(path_x[c_index] - x, path_y[c_index] - y) >= Ld:
            break
        c_index += 1
    return c_index


def pure_pursuit_control(x, y, yaw, v, path_x, path_y, L, k, Ld0, last_idx=0):
    """
    delta = arctan(L kappa) 曲率 * 车辆轴距
    :param L:
    :param k:速度相关前视增益
    :param Ld0:最小前视距离
    :param last_idx:上一次最近点索引，避免回跳
    :return: delta, target_idx, nearest_idx
    """
    # 1. 计算预瞄距离
    Ld = k * v + Ld0
    # 2. 找最近路径点（在 last_idx 附近窗口内搜索）
    nearest_idx = find_nearest_point(x, y, path_x, path_y, last_idx=last_idx)
    # 3. 从最近点往前找预瞄点
    target_idx = find_lookahead_point(nearest_idx, path_x, path_y, x, y, Ld)
    tx = path_x[target_idx]
    ty = path_y[target_idx]
    # 4. 转换到车体坐标系
    dx = tx - x
    dy = ty - y
    y_r = -np.sin(yaw) * dx + np.cos(yaw) * dy
    # 5. 计算转角（用实际预瞄距离，不是期望值）
    ld_actual = np.hypot(dx, dy)
    if ld_actual < 1e-6:
        return 0.0, target_idx, nearest_idx
    delta = np.arctan2(2.0 * L * y_r, ld_actual ** 2)
    delta = float(np.clip(delta, -max_steer, max_steer))
    return delta, target_idx, nearest_idx

