import numpy as np
from scipy.interpolate import splprep, splev


# 计算折线长度
def compute_path_length(x, y):
    if len(x) < 2:
        return 0.0
    dx = np.diff(x)
    dy = np.diff(y)
    return np.sum(np.sqrt(dx ** 2 + dy ** 2))


def smooth_path(x, y, num_points, s=0.0):
    """
    对路径进行平滑
    :param x: x
    :param y: y
    :param num_points: 平滑后的采样点数
    :param s: 平滑系数 s=0 穿过所有点，s>0 允许一定的平滑
    :return: s_x,x_y
    """
    if len(x) < 2:
        return x, y
    k = min(3, len(x) - 1)  # k 是样条阶数
    tck, u = splprep([x, y], s=s, k=k)  # spline preparation t:knot vector（节点向量） c:控制点/系数 k:样条次数 u:对应每个输入点的参数位置。
    u_fine = np.linspace(0, 1, num_points)  # 在 0，1 之间均匀采样。
    s_x, s_y = splev(u_fine, tck)  # spline evaluation
    return np.array(s_x), np.array(s_y)


# 按固定距离 ds 对路径重采样
def resample_path(x, y, ds=0.5):
    if len(x) < 2:
        return x, y
    dx = np.diff(x)
    dy = np.diff(y)
    dist = np.hypot(dx, dy)
    s = np.concatenate(([0.0], np.cumsum(dist)))
    total_len = s[-1]
    if total_len == 0:
        return x, y
    s_new = np.arange(0.0, total_len, ds)
    if len(s_new) == 0 or s_new[-1] < total_len:
        s_new = np.append(s_new, total_len)
    x_new = np.interp(s_new, s, x)
    y_new = np.interp(s_new, s, y)
    return x_new, y_new


# 根据路径点，计算每个角的航向角
def compute_yaw(x, y):
    # 路径方向本质是曲线切线方向
    # x = x(s)
    # y = y(s)
    # 则切线方向：arctan(dy/dx)
    if len(x) < 2:
        return np.array([0.0])
    dx = np.gradient(x)  # 计算数值导数，即相邻点变化率 y=[0,1,1] dy=[1,0.5,0] 使用中心差分，长度不变；diff 会输入N-1个。
    dy = np.gradient(y)
    yaw = np.arctan2(dy, dx)
    return yaw


# 计算曲率
def compute_curvature(x, y):
    dx = np.gradient(x)
    dy = np.gradient(y)
    ddx = np.gradient(dx)
    ddy = np.gradient(dy)
    denominator = (dx ** 2 + dy ** 2) ** 1.5
    denominator[denominator < 1e-6] = 1e-6
    curvature = (dx * ddy - dy * ddx) / denominator
    return curvature
