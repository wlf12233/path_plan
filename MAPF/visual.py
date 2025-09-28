import matplotlib.pyplot as plt


def plot_robot_paths(robots, conflicts):
    """
    绘制多个机器人的运动路径。

    参数:
    robot_paths (dict): 一个字典，键为机器人名称，值为路径列表（元组或列表），每个路径由 (x, y) 坐标点组成。
    """

    # 定义不同机器人的颜色
    colors = {
        "Robot 1": 'b',  # 蓝色
        "Robot 2": 'g',  # 绿色
        "Robot 3": 'r',  # 红色
        "Robot 4": 'm'  # 紫色
    }

    # 创建图像和轴
    plt.figure(figsize=(10, 8))  # 设置窗口大小

    # 绘制每个机器人的路径
    for agent in robots:
        x_coords = [point[0] for point in agent.path]  # 提取X坐标
        y_coords = [point[1] for point in agent.path]  # 提取Y坐标
        plt.plot(x_coords, y_coords, marker='o', color=colors.get(agent.id, 'k'), label=agent.id)  # 绘制路径
        plt.text(x_coords[-1], y_coords[-1], agent.id, fontsize=12, color=colors.get(agent.id, 'k'))  # 在路线末尾标记机器人名称

        # for t, (x, y) in enumerate(zip(x_coords, y_coords)):
        #     plt.text(x, y, str(t), fontsize=12, color='red', ha='right')
    if conflicts:
        for conflict in conflicts:
            time = conflict['time']
            x, y = conflict['position']
            agents = conflict['agents']
            plt.plot(x, y, marker='o', color='red', markersize=10, label='Conflict')
            # plt.text(x, y, f'Conflict at t={time}\nAgents: {", ".join(map(str, agents))}', fontsize=10, color='red',
            #          ha='left')  # 设置图形属性
    plt.title("Multiple Robot Paths")  # 设置标题
    plt.xlim(-1, 5)  # 设置X轴范围
    plt.ylim(-1, 5)  # 设置Y轴范围
    plt.xticks(range(5))  # 设置X轴刻度
    plt.yticks(range(5))  # 设置Y轴刻度
    plt.grid(True)  # 显示网格线
    plt.legend()  # 显示图例

    # 显示图像
    plt.gca().set_aspect('equal', adjustable='box')  # 设置坐标轴比例相等
    plt.xlabel("X Coordinate")  # 设置X轴标签
    plt.ylabel("Y Coordinate")  # 设置Y轴标签
    plt.show()  # 显示绘制的图形


def plot_path(paths):
    colors = {
        "A": 'b',  # 蓝色
        "B": 'g',  # 绿色
        "C": 'r',  # 红色
        "D": 'm'  # 紫色
    }
    plt.figure(figsize=(10, 8))
    for robot, path in paths.items():
        x = [point[0] for point in path]
        y = [point[1] for point in path]
        plt.plot(x, y, marker='o', color=colors.get(robot, 'k'), label=robot)
        # plt.text(x, y, robot, fontsize=12, color=colors.get(robot, 'k'))  # 在路线末尾标记机器人名称
    plt.plot(2, 2, marker='o', color='black', markersize=10, label='conflict')
    plt.title("Multiple Robot Paths")  # 设置标题
    plt.xlim(-1, 5)  # 设置X轴范围
    plt.ylim(-1, 5)  # 设置Y轴范围
    plt.xticks(range(5))  # 设置X轴刻度
    plt.yticks(range(5))  # 设置Y轴刻度
    plt.grid(True)  # 显示网格线
    plt.legend()  # 显示图例

    # 显示图像
    plt.gca().set_aspect('equal', adjustable='box')  # 设置坐标轴比例相等
    plt.xlabel("X Coordinate")  # 设置X轴标签
    plt.ylabel("Y Coordinate")  # 设置Y轴标签
    plt.show()  # 显示绘制的图形
    plt.show()


# 示例路径数据
robot_paths_example = {
    "A": [(1, 1), (2, 2), (3, 3), (4, 4)],
    "B": [(1, 2), (2, 2), (3, 3), (4, 3)],
    "C": [(1, 0), (2, 0), (2, 1), (2, 2), (2, 3), (2, 4)],
}

if __name__ == '__main__':
    plot_path(robot_paths_example)
