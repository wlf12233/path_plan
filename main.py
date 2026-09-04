import numpy as np
import matplotlib.pyplot as plt
from map.grid_map import GridMap
from controller.pure_pursuit import pure_pursuit_control
from model.bicycle_model import State, update
from planer.rrt import RRTPlaner
from planer.rrtStar import RRTStarPlaner
from smoother.path_utils import remove_duplicate_points, simplify_path, path_to_xy
from smoother.spline_smoother import smooth_path, resample_path, compute_yaw
from planer.astar import AstarPlanner
from planer.hybrid_astar import HybridAstarPlanner


def create_map():
    grid_map = GridMap(30, 30)
    grid_map.add_rect_obstacle(10, 20, 5, 15)
    grid_map.add_obstacle(4, 5)
    grid_map.add_obstacle(5, 4)
    return grid_map


def plot_result(grid, start, goal, path=None, smooth_path=None, expanded=None, frontier=None):
    if isinstance(grid, GridMap):
        grid = grid.grid
    plt.figure(figsize=(8, 8))
    plt.imshow(grid, cmap='gray_r', origin='lower')
    if frontier:
        f = np.array(list(frontier))
        plt.scatter(f[:, 1], f[:, 0], c='lightyellow', edgecolors='gold',
                    s=40, label='Frontier (in open list)')
    if expanded:
        e = np.array(list(expanded))
        plt.scatter(e[:, 1], e[:, 0], c='lightblue', edgecolors='steelblue',
                    s=40, label='Expanded')
    plt.plot(start[1], start[0], 'go', markersize=10, label='Start')
    plt.plot(goal[1], goal[0], 'ro', markersize=10, label='Goal')
    if path:
        path = np.array(path)
        plt.plot(path[:, 1], path[:, 0], 'b-', linewidth=2, label='A* Path')
    if smooth_path is not None and len(smooth_path) > 0:
        plt.plot(smooth_path[0], smooth_path[1], 'red', linewidth=2, label='Smooth Path')
    plt.legend(loc='upper left', bbox_to_anchor=(1.02, 1.0))
    plt.title("A* Path Planning on Grid Map")
    plt.grid(True)
    plt.tight_layout()
    plt.show()


def plot_tracking(grid_map, smooth_x, smooth_y, smooth_yaw, xs, ys, yaws, vs, deltas, ts):
    fig = plt.figure(figsize=(13, 8))

    ax1 = plt.subplot2grid((3, 3), (0, 0), rowspan=3, colspan=2)
    ax1.imshow(grid_map.grid, cmap='gray_r', origin='lower')
    ax1.plot(smooth_x, smooth_y, 'r-', lw=2, label='Reference')
    ax1.plot(xs, ys, 'b-', lw=1.5, label='Tracked')
    ax1.scatter(xs[0], ys[0], c='green', s=60, zorder=5, label='Start')
    ax1.scatter(xs[-1], ys[-1], c='black', s=60, marker='x', zorder=5, label='End')

    step = max(1, len(xs) // 25)
    ax1.quiver(xs[::step], ys[::step], np.cos(yaws[::step]), np.sin(yaws[::step]),
               color='blue', scale=25, width=0.004, alpha=0.6)
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_title('Pure Pursuit Path Tracking')
    ax1.legend(loc='upper left')
    ax1.grid(True)
    ax1.set_aspect('equal')

    # 横向误差（带符号）+ 最近参考点索引
    ref = np.column_stack([smooth_x, smooth_y])
    actual = np.column_stack([xs, ys])
    ref_idx = np.array([int(np.argmin(np.hypot(ref[:, 0] - p[0], ref[:, 1] - p[1]))) for p in actual])
    dx = actual[:, 0] - ref[ref_idx, 0]
    dy = actual[:, 1] - ref[ref_idx, 1]
    theta_ref = smooth_yaw[ref_idx]
    err = -np.sin(theta_ref) * dx + np.cos(theta_ref) * dy  # +左 / -右

    ax2 = plt.subplot2grid((3, 3), (0, 2))
    ax2.plot(ts, err, 'm-')
    ax2.axhline(0.0, color='k', lw=0.5)
    ax2.set_xlabel('t [s]')
    ax2.set_ylabel('lateral err [m]  (+left / -right)')
    ax2.set_title(f'Lateral Error (max|e|={np.abs(err).max():.2f}, '
                  f'mean|e|={np.abs(err).mean():.2f})')
    ax2.grid(True)

    # 航向角误差，wrap 到 [-pi, pi]
    yaw_err = np.arctan2(np.sin(yaws - smooth_yaw[ref_idx]),
                         np.cos(yaws - smooth_yaw[ref_idx]))
    yaw_err_deg = np.rad2deg(yaw_err)
    ax_yaw = plt.subplot2grid((3, 3), (1, 2))
    ax_yaw.plot(ts, yaw_err_deg, 'c-')
    ax_yaw.set_xlabel('t [s]')
    ax_yaw.set_ylabel('yaw err [deg]')
    ax_yaw.set_title(f'Yaw Error (max={np.abs(yaw_err_deg).max():.2f}, '
                     f'mean={np.abs(yaw_err_deg).mean():.2f})')
    ax_yaw.grid(True)

    ax3 = plt.subplot2grid((3, 3), (2, 2))
    ax3.plot(ts, vs, 'b-', label='v [m/s]')
    ax3.plot(ts, np.rad2deg(deltas), 'r-', label='delta [deg]')
    ax3.set_xlabel('t [s]')
    ax3.legend()
    ax3.grid(True)
    ax3.set_title('Speed & Steering')

    plt.tight_layout()
    plt.show()


def simulate(path_x, path_y, path_yaw, L=2.5, dt=0.1, target_v=1.0, k=0.3, Ld0=2.0,
             goal_tol=1.0, max_time=120.0):
    state = State(x=path_x[0], y=path_y[0], yaw=path_yaw[0], v=0.0)
    xs, ys, yaws, vs, deltas, ts = [state.x], [state.y], [state.yaw], [state.v], [0.0], [0.0]
    last_idx = 0
    t = 0.0
    while t < max_time:
        accel = float(np.clip(1.0 * (target_v - state.v), -2.0, 2.0))
        delta, _, last_idx = pure_pursuit_control(state.x, state.y, state.yaw,
                                                  max(state.v, 0.1), path_x, path_y, L, k, Ld0, last_idx=last_idx)
        state = update(state, accel, delta, dt, L)
        t += dt
        xs.append(state.x)
        ys.append(state.y)
        yaws.append(state.yaw)
        vs.append(state.v)
        deltas.append(delta)
        ts.append(t)

        if np.hypot(state.x - path_x[-1], state.y - path_y[-1]) <= goal_tol:
            break
    return np.array(xs), np.array(ys), np.array(yaws), np.array(vs), np.array(deltas), np.array(ts)


def main():
    grid_map = create_map()
    start = (0, 0)
    goal = (25, 20)
    # planner = AstarPlanner(grid_map)
    # path,vis = planner.plan(start, goal)
    # planner = HybridAstarPlanner(grid_map)
    planner = RRTStarPlaner(grid_map)
    path, visited = planner.rrt_star(start, goal)
    # planner = RRTPlaner(grid_map)
    # path,vis = planner.rrt(start, goal)
    if path is None or len(path) == 0:
        print("No path found")
        return
    print("节点数: ", len(path))
    visited = None
    plot_result(grid_map, start, goal, path=path, smooth_path=None, expanded=visited, frontier=None)

    # 去重
    # raw_path = remove_duplicate_points(path)
    #
    # # 路径简化
    # simple_path = simplify_path(raw_path)
    #
    # # 转为x，y
    # x, y = path_to_xy(simple_path)
    # # 样条平滑
    # smooth_x, smooth_y = smooth_path(x, y, 300, 2.0)
    # # 重采样
    # smooth_x, smooth_y = resample_path(smooth_x, smooth_y, 0.01)
    # # 计算朝向
    # yaw = compute_yaw(smooth_x, smooth_y)
    #
    # smooth_xy = [smooth_x, smooth_y]
    #
    # print("平滑后轨迹点数:", len(smooth_x))
    # print("yaw 点数:", len(yaw))
    #
    # xs, ys, yaws, vs, deltas, ts = simulate(smooth_x, smooth_y, yaw)
    #
    # print(f"Sim time: {ts[-1]:.2f}s, end pos=({xs[-1]:.2f},{ys[-1]:.2f}), "
    #       f"goal=({smooth_x[-1]:.2f},{smooth_y[-1]:.2f})")
    # plot_tracking(grid_map, smooth_x, smooth_y, yaw, xs, ys, yaws, vs, deltas, ts)
    # plot_result(grid_map, start, goal, path=path, smooth_path=smooth_xy, expanded=visited, frontier=None)


if __name__ == "__main__":
    main()
