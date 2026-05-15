import numpy as np
import matplotlib.pyplot as plt


# x = v * cos(yaw)
# y = v * sin(yaw)
# yaw = v/L * tan(delta)
# v = a
# 曲率 = tan(delta)/L

# x：车辆位置的 x 坐标
# y：车辆位置的 y 坐标
# theta：车辆朝向角（yaw）
# v：车辆速度
# L：车辆轴距
# delta：前轮转角
# a：加速度

# 表示车辆的一个状态
class State:
    def __init__(self, x, y, yaw, v):
        self.x = x
        self.y = y
        self.yaw = yaw  # 航向角
        self.v = v


# 新状态 = 旧状态 + 这一个时间步内的变化量
def update(state, accel, delta, dt, L):
    # 显式欧拉：用旧状态算下一步
    # 隐式欧拉：用下一步状态反过来算
    # 半隐式 / 辛欧拉：有些量先更新，再拿新值更新别的量

    x = state.x
    y = state.y
    yaw = state.yaw
    v = state.v
    new_x = x + v * np.cos(yaw) * dt
    new_y = y + v * np.sin(yaw) * dt
    new_yaw = yaw + v / L * np.tan(delta) * dt
    new_v = v + accel * dt
    state.x = new_x
    state.y = new_y
    state.yaw = new_yaw
    state.v = new_v
    # state.x += state.v * np.cos(state.yaw) * dt
    # state.y += state.v * np.sin(state.yaw) * dt
    # state.yaw += state.v / L * np.tan(delta) * dt
    # state.v += accel * dt
    return state


def control_input(time):
    if time < 3.0:
        accel = 2
        delta = 0
    elif time < 6.0:
        accel = 0
        delta = np.deg2rad(30.0)
    elif time < 8.0:
        accel = -2
        delta = np.deg2rad(-50.0)
    else:
        accel = 0
        delta = 0
    return accel, delta


def get_history(state, dt, L, times):
    x_history = [state.x]
    y_history = [state.y]
    yaw_history = [state.yaw]
    v_history = [state.v]
    time_history = [0]
    time = 0.0
    delta = 0
    accel = 0
    while time <= times:
        accel_cmd, delta_cmd = control_input(time)
        # 转角速率限制 每秒30度
        max_steer_rate = np.deg2rad(30.0)
        delta += np.clip(delta_cmd - delta, -max_steer_rate * dt, max_steer_rate * dt)
        # 加速度变化限制 每秒1.0
        max_accel_rate = 1.0
        accel += np.clip(accel_cmd - accel, -max_accel_rate * dt, max_accel_rate * dt)

        state = update(state, accel, delta, dt, L)
        time = time + dt
        x_history.append(state.x)
        y_history.append(state.y)
        yaw_history.append(state.yaw)
        v_history.append(state.v)
        time_history.append(time)
    return x_history, y_history, yaw_history, v_history, time_history


def plot_trace(x_history, y_history, yaw_history, v_history, time_history):
    plt.figure(figsize=(8, 6))
    # plt.subplot(1, 3, 1)
    plt.plot(x_history, y_history)
    plt.scatter(x_history[0], y_history[0], c='red')
    plt.scatter(x_history[-1], y_history[-1], c='blue')

    arrow_step = max(1, len(x_history) // 20)
    xs = np.array(x_history[::arrow_step])
    ys = np.array(y_history[::arrow_step])
    yaws = np.array(yaw_history[::arrow_step])
    arrow_len = 0.05 * max(np.ptp(x_history), np.ptp(y_history))
    plt.quiver(xs, ys, np.cos(yaws), np.sin(yaws),
               angles='xy', scale_units='xy', scale=1 / arrow_len,
               color='green', width=0.005)

    plt.axis('equal')
    plt.grid(True)
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Straight Motion of Bicycle Model')

    # plt.subplot(1, 3, 2)
    # plt.plot(time_history, np.rad2deg(yaw_history))
    # plt.grid(True)
    # plt.xlabel('time')
    # plt.ylabel('Yaw')
    # plt.title('YAM')
    #
    # plt.subplot(1, 3, 3)
    # plt.plot(time_history, v_history)
    # plt.scatter(time_history[0], v_history[0], c='red')
    # plt.scatter(time_history[-1], v_history[-1], c='blue')
    # plt.axis('equal')
    # plt.grid(True)
    # plt.xlabel('time')
    # plt.ylabel('V')
    # plt.title('speed')

    plt.tight_layout()
    plt.show()


def main():
    state = State(0, 0, 0, 1.0)
    x_history, y_history, yaw_history, v_history, time_history = get_history(state, 0.1, 2, 20)
    plot_trace(x_history, y_history, yaw_history, v_history, time_history)


if __name__ == '__main__':
    main()
