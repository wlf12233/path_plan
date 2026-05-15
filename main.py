import numpy as np
import matplotlib.pyplot as plt
from map.grid_map import GridMap
from smoother.path_utils import remove_duplicate_points, simplify_path, path_to_xy
from smoother.spline_smoother import smooth_path, resample_path, compute_yaw
from planer.astar import AstarPlanner


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


def main():
    grid_map = create_map()
    start = (0, 0)
    goal = (25, 20)
    planner = AstarPlanner(grid_map)
    path, visited = planner.plan(start, goal)
    if path is None or len(path) == 0:
        print("No path found")
        return
    print("节点数: ", len(path))
    # 去重
    raw_path = remove_duplicate_points(path)

    # 路径简化
    simple_path = simplify_path(raw_path)

    # 转为x，y
    x, y = path_to_xy(simple_path)
    # 样条平滑
    smooth_x, smooth_y = smooth_path(x, y, 300, 5.0)
    # 重采样
    smooth_x, smooth_y = resample_path(smooth_x, smooth_y)
    # 计算朝向
    yaw = compute_yaw(smooth_x, smooth_y)

    smooth_xy = [smooth_x, smooth_y]

    print("平滑后轨迹点数:", len(smooth_x))
    print("yaw 点数:", len(yaw))

    plot_result(grid_map, start, goal, path=path, smooth_path=smooth_xy, expanded=visited, frontier=None)


if __name__ == "__main__":
    main()
