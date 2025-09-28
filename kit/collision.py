import math
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('TkAgg')  # 设置为 TkAgg 后端

from matplotlib.patches import Polygon



# 向量点积
def dot_product(v1, v2):
    return v1['x'] * v2['x'] + v1['y'] * v2['y']


def calculate_vertices(center, width, height, angle):
    # 计算矩形的四个顶点
    hw, hh = width / 2, height / 2
    corners = [
        {'x': -hw, 'y': -hh},
        {'x': hw, 'y': -hh},
        {'x': hw, 'y': hh},
        {'x': -hw, 'y': hh}
    ]
    cos_a = math.cos(angle)
    sin_a = math.sin(angle)
    rotated_corners = []
    for corner in corners:
        x_rot = corner['x'] * cos_a - corner['y'] * sin_a + center['x']
        y_rot = corner['x'] * sin_a + corner['y'] * cos_a + center['y']
        rotated_corners.append({'x': x_rot, 'y': y_rot})
    return rotated_corners


# 获取边方向作为分离轴
def get_edge_direction(vertex1, vertex2):
    edge = {'x': vertex2['x'] - vertex1['x'], 'y': vertex2['y'] - vertex1['y']}
    return edge


# 将顶点投影到轴上，返回投影范围
def project_vertices(vertices, axis):
    min_proj = float('inf')
    max_proj = float('-inf')
    for vertex in vertices:
        proj = dot_product(vertex, axis)
        min_proj = min(min_proj, proj)
        max_proj = max(max_proj, proj)
    return min_proj, max_proj


def overlap_on_axis(vertices1, vertices2, axis):
    min1, max1 = project_vertices(vertices1, axis)
    min2, max2 = project_vertices(vertices2, axis)
    return not (max1 < min2 or max2 < min1)


def check_collision(rect1, rect2):
    vertices1 = calculate_vertices(rect1['center'], rect1['width'], rect1['height'], rect1['rotation'])
    vertices2 = calculate_vertices(rect2['center'], rect2['width'], rect2['height'], rect2['rotation'])

    axes = [
        get_edge_direction(vertices1[0], vertices1[1]),
        get_edge_direction(vertices1[0], vertices1[3]),
        get_edge_direction(vertices2[0], vertices2[1]),
        get_edge_direction(vertices2[0], vertices2[3])
    ]
    for axis in axes:
        if not overlap_on_axis(vertices1, vertices2, axis):
            return False, vertices1, vertices2  # 在某个轴上没有重叠，说明没有碰撞

    return True, vertices1, vertices2  # 所有轴上都有重叠，说明碰撞


# 可视化函数
def visualize_obb(obb_a, obb_b):
    # 检查碰撞并获取顶点
    collision, vertices_a, vertices_b = check_collision(obb_a, obb_b)

    # 设置颜色：碰撞为红色，未碰撞为绿色
    color = 'red' if collision else 'green'

    # 创建画布
    fig, ax = plt.subplots()

    # 绘制OBB A
    poly_a = Polygon([(v['x'], v['y']) for v in vertices_a], closed=True, fill=True, facecolor=color, alpha=0.5,
                     label='OBB A')
    ax.add_patch(poly_a)

    # 绘制OBB B
    poly_b = Polygon([(v['x'], v['y']) for v in vertices_b], closed=True, fill=True, facecolor=color, alpha=0.5,
                     label='OBB B')
    ax.add_patch(poly_b)

    # 设置画布属性
    ax.set_aspect('equal')  # 保持x和y轴比例相等
    ax.grid(True)
    ax.legend()
    ax.set_title(f"OBB Collision: {'Colliding' if collision else 'Not Colliding'}")
    ax.set_xlabel('X')
    ax.set_ylabel('Y')

    # 自动调整坐标轴范围
    all_x = [v['x'] for v in vertices_a + vertices_b]
    all_y = [v['y'] for v in vertices_a + vertices_b]
    ax.set_xlim(min(all_x) - 2, max(all_x) + 2)
    ax.set_ylim(min(all_y) - 2, max(all_y) + 2)

    plt.show()
    return collision


if __name__ == "__main__":
    obb_a = {
        'center': {'x': 0, 'y': 0},
        'width': 4,
        'height': 1.5,
        'rotation': math.pi / 3  # 45度
    }

    obb_b = {
        'center': {'x': 3, 'y': 0},
        'width': 4,
        'height': 2,
        'rotation': 0  # 0度
    }

    result = visualize_obb(obb_a, obb_b)
    print(f"OBB矩形是否发生碰撞：{result}")
