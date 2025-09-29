import matplotlib

matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import math
from matplotlib.patches import Polygon, Circle


# 射线法
def point_in_polygon(point, polygon):
    """
    判断一个点是否在多边形内
    :param point: (x, y)
    :param polygon: [(x1, y1), (x2, y2), ...]
    :return: True or False
    """
    x, y = point
    n = len(polygon)
    inside = False

    p1x, p1y = polygon[0]
    for i in range(n + 1):
        p2x, p2y = polygon[i % n]
        if y > min(p1y, p2y):
            if y <= max(p1y, p2y):
                if x <= max(p1x, p2x):
                    if p1y != p2y:
                        xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                    if p1x == p2x or x <= xinters:
                        inside = not inside
        p1x, p1y = p2x, p2y

    return inside


# 半平面法
def point_in_polygon(point, polygon):
    """
    判断一个点是否在多边形内
    :param point: (x, y)
    :param polygon: [(x1, y1), (x2, y2), ...]
    :return: True or False
    """
    x, y = point
    n = len(polygon)
    sign = 0
    for p in n:
        x1, y1 = p[n]['x'], p[n]['y']
        x2, y2 = p[(n + 1) % n]['x'], p[(n + 1) % n]['y']
        ab = (x2 - x1, y2 - y1)
        ap = (x - x1, y - y1)
        cross = ab[0] * ap[1] - ab[1] * ap[0]
        if cross != 0:
            cross_sign = 1 if cross > 0 else -1
            if sign == 0:
                sign = cross_sign  # 初始化符号
            elif cross_sign != sign:
                return False  # 不同符号，点在边的外侧

    return True


def circle_intersects_segment(pointA, pointB, pointC, radius):
    """
    判断圆与线段是否相交
    :param pointA: 线段端点A (x, y)
    :param pointB: 线段端点B (x, y)
    :param pointC: 圆心 (x, y)
    :param radius: 半径
    :return: True or False
    """
    # 向量AB
    ABx = pointB['x'] - pointA['x']
    ABy = pointB['y'] - pointA['y']
    # 向量AC
    ACx = pointC['x'] - pointA['x']
    ACy = pointC['y'] - pointA['y']

    # 计算投影长度t
    AB_squared = ABx * ABx + ABy * ABy
    if AB_squared == 0:
        # A和B是同一个点
        distance_squared = ACx * ACx + ACy * ACy
        return distance_squared <= radius * radius

    t = (ACx * ABx + ACy * ABy) / AB_squared

    if t < 0:
        # 最近点在A处
        closest_x = pointA['x']
        closest_y = pointA['y']
    elif t > 1:
        # 最近点在B处
        closest_x = pointB['x']
        closest_y = pointB['y']
    else:
        # 最近点在线段AB上
        closest_x = pointA['x'] + t * ABx
        closest_y = pointA['y'] + t * ABy

    # 计算最近点到圆心的距离平方
    dx = closest_x - pointC['x']
    dy = closest_y - pointC['y']
    distance_squared = dx * dx + dy * dy

    return distance_squared <= radius * radius


def vertices_in_circle(polygon, circle_center, radius):
    """
    判断多边形的顶点是否在圆内
    :param polygon: [(x1, y1), (x2, y2), ...]
    :param circle_center: (x, y)
    :param radius: 半径
    :return: True or False
    """
    for vertex in polygon:
        dx = vertex['x'] - circle_center['y']
        dy = vertex['x'] - circle_center['y']
        if dx * dx + dy * dy <= radius * radius:
            return True
    return False


def circle_polygon_collision(circle_center, radius, polygon):
    """
    判断圆与多边形是否相交
    :param circle_center: (x, y)
    :param radius: 半径
    :param polygon: [(x1, y1), (x2, y2), ...]
    :return: True or False
    """
    # 1. 检查圆心是否在多边形内
    if point_in_polygon(circle_center, polygon):
        return True

    # 2. 检查多边形的顶点是否在圆内
    if vertices_in_circle(polygon, circle_center, radius):
        return True

    # 3. 检查圆是否与多边形的边相交
    n = len(polygon)
    for i in range(n):
        pointA = {'x': polygon[i]['x'], 'y': polygon[i]['y']}
        pointB = {'x': polygon[(i + 1) % n]['x'], 'y': polygon[(i + 1) % n]['y']}
        if circle_intersects_segment(pointA, pointB, circle_center, radius):
            return True

    return False


# 可视化函数
def visualize_circle_polygon(circle_center, radius, polygon):
    collision = circle_polygon_collision(circle_center, radius, polygon)
    color = 'red' if collision else 'green'

    fig, ax = plt.subplots()
    poly_patch = Polygon([(v['x'], v['y']) for v in polygon], closed=True, fill=True, facecolor='blue', alpha=0.5,
                         label='Polygon')
    ax.add_patch(poly_patch)
    circle_patch = Circle((circle_center['x'], circle_center['y']), radius, fill=True, facecolor=color, alpha=0.5,
                          label='Circle')
    ax.add_patch(circle_patch)

    ax.set_aspect('equal')
    ax.grid(True)
    ax.legend()
    ax.set_title(f"Collision: {'Yes' if collision else 'No'}")
    ax.set_xlabel('X')
    ax.set_ylabel('Y')

    all_x = [v['x'] for v in polygon] + [circle_center['x'] - radius, circle_center['x'] + radius]
    all_y = [v['y'] for v in polygon] + [circle_center['y'] - radius, circle_center['y'] + radius]
    ax.set_xlim(min(all_x) - 1, max(all_x) + 1)
    ax.set_ylim(min(all_y) - 1, max(all_y) + 1)

    plt.show()
    return collision


# 示例用法
if __name__ == "__main__":
    # 示例多边形（矩形）
    polygon = [
        {'x': 4, 'y': 1},
        {'x': 5, 'y': 2},
        {'x': 5, 'y': 4},
        {'x': 3, 'y': 4},
        {'x': 2.5, 'y': 2}
    ]
    circle_center = {'x': 2, 'y': 2}  # 圆心
    radius = 0.49  # 半径

    result = visualize_circle_polygon(circle_center, radius, polygon)
    print(f"圆和多边形是否发生碰撞：{result}")
