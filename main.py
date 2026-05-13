import numpy as np
import matplotlib.pyplot as plt
from map.grid_map import GridMap
from planer.astar import AstarPlanner


def create_map():
    grid_map = GridMap(30, 30)
    grid_map.add_rect_obstacle(10, 20, 5, 15)
    grid_map.add_obstacle(4, 5)
    grid_map.add_obstacle(5, 4)
    return grid_map


def plot_result(grid, start, goal, path=None, expanded=None, frontier=None):
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
    plot_result(grid_map, start, goal, path=path, expanded=visited, frontier=None)


if __name__ == "__main__":
    main()
