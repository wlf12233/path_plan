import matplotlib

matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import math
from matplotlib.patches import Polygon


def dot_product(v1, v2):
    return v1['x'] * v2['x'] + v1['y'] * v2['y']


def get_edge_dir(v1, v2):
    return {'x': v2['x'] - v1['x'], 'y': v2['y'] - v1['y']}


def project_vertices(vertices, axis):
    min_proj = float('inf')
    max_proj = float('-inf')
    for v in vertices:
        proj = dot_product(v, axis)
        min_proj = min(min_proj, proj)
        max_proj = max(max_proj, proj)
    return min_proj, max_proj


def overlap_on_projection(v1, v2, axis):
    min1, max1 = project_vertices(v1, axis)
    min2, max2 = project_vertices(v2, axis)
    return not (max1 < min2 or max2 < min1)


def check_collision(poly1, poly2):
    # 获取多边形的顶点
    vertices1 = [{'x': p['x'], 'y': p['y']} for p in poly1['vertices']]
    vertices2 = [{'x': p['x'], 'y': p['y']} for p in poly2['vertices']]

    # 获取多边形的边
    edges1 = [get_edge_dir(vertices1[i], vertices1[(i + 1) % len(vertices1)]) for i in range(len(vertices1))]
    edges2 = [get_edge_dir(vertices2[i], vertices2[(i + 1) % len(vertices2)]) for i in range(len(vertices2))]

    # 检查所有边的法线方向
    for edge in edges1 + edges2:
        axis = {'x': -edge['y'], 'y': edge['x']}
        if not overlap_on_projection(vertices1, vertices2, axis):
            return False, vertices1, vertices2  # 存在分离轴，无碰撞

    return True, vertices1, vertices2  # 所有轴上都有重叠，存在碰撞


# 可视化函数
# 可视化函数
def visualize_polygons(poly_a, poly_b):
    collision, vertices_a, vertices_b = check_collision(poly_a, poly_b)
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
    # 多边形A：三角形
    poly_a = {
        'vertices': [
            {'x': 0, 'y': 0},
            {'x': 2, 'y': 3},
            {'x': 4, 'y': 0}
        ]
    }

    # 多边形B：五边形
    poly_b = {
        'vertices': [
            {'x': 4, 'y': 1},
            {'x': 5, 'y': 2},
            {'x': 5, 'y': 4},
            {'x': 3, 'y': 4},
            {'x': 2, 'y': 2}
        ]
    }

    result = visualize_polygons(poly_a, poly_b)
    print(f"凸多边形是否发生碰撞：{result}")
