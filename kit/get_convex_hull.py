import math
import matplotlib

matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches


def cross_product(p0, p1, p2):
    """
    计算向量p0->p1和p1->p2的叉积
    叉积为正表示为逆时针方向
    叉积为负表示为顺时针方向
    叉积为零表示共线
    其中O、A、B为二维点，表示为(x, y)元组
    参考：https://en.wikipedia.org/wiki/Cross_product#Computing_the_cross_product
    参考：https://en.wikipedia.org/wiki/Convex_hull#Monotone_chain
    参考：https://cp-algorithms.com/geometry/convex_hull.html#implementation
    :param o:
    :param a:
    :param b:
    :return:
    """
    v1 = {'x': p1['x'] - p0['x'], 'y': p1['y'] - p0['y']}
    v2 = {'x': p2['x'] - p1['x'], 'y': p2['y'] - p1['y']}
    return v1['x'] * v2['y'] - v1['y'] * v2['x']


def convex_hull_andrew(points):
    """
    计算二维点集的凸包
    使用Andrew's monotone chain算法，时间复杂度O(n log n)
    :param points: 二维点集，列表形式，每个点为(x, y)元组
    :return: 凸包顶点列表，按逆时针顺序排列
    """
    # 按x坐标排序，x相同时按y坐标排序
    points = sorted([{'x': p['x'], 'y': p['y']} for p in points], key=lambda p: (p['x'], p['y']))
    if len(points) <= 1:
        return points

    # 构建下凸包
    lower = []
    for p in points:
        while len(lower) >= 2 and cross_product(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)

    # 构建上凸包
    upper = []
    for p in reversed(points):
        while len(upper) >= 2 and cross_product(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)

    # 去掉最后一个点，因为它与第一个点重复
    del lower[-1]
    del upper[-1]

    # 合并上下凸包
    convex_hull = lower + upper
    # return [(p['x'], p['y']) for p in convex_hull]
    return convex_hull


def convex_hull_graham(points):
    """
    计算二维点集的凸包
    使用Graham scan算法，时间复杂度O(n log n)
    :param points: 二维点集，列表形式，每个点为(x, y)元组
    :return: 凸包顶点列表，按逆时针顺序排列
    """
    # 按y坐标排序，y相同时按x坐标排序，找到最低点
    points = sorted([{'x': p['x'], 'y': p['y']} for p in points], key=lambda p: (p['y'], p['x']))
    if len(points) <= 1:
        return points

    # 以最低点为基准点，按极角排序
    base = points[0]

    def polar_angle(p):
        return math.atan2(p['y'] - base['y'], p['x'] - base['x'])

    sorted_points = sorted(points[1:], key=polar_angle)

    # 构建凸包
    hull = [base, sorted_points[0]]
    for p in sorted_points[1:]:
        while len(hull) >= 2 and cross_product(hull[-2], hull[-1], p) <= 0:
            hull.pop()
        hull.append(p)

    # return [(p['x'], p['y']) for p in hull]
    return hull

# 可视化点集和凸包
def visualize_convex_hull(points):
    # try:
        # 计算凸包
        hull = convex_hull_graham(points)

        # 创建画布
        fig, ax = plt.subplots()

        # 绘制原始点
        xs = [p['x'] for p in points]
        ys = [p['y'] for p in points]
        ax.scatter(xs, ys, color='blue', label='Points', zorder=2)

        # 绘制凸包
        if len(hull) >= 3:
            hull_patch = patches.Polygon([(v['x'], v['y']) for v in hull], closed=True, fill=True, facecolor='red', alpha=0.3,
                                         label='Convex Hull')
            ax.add_patch(hull_patch)

        # 设置画布属性
        ax.set_aspect('equal')
        ax.grid(True)
        ax.legend()
        ax.set_title('Convex Hull')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')

        # 自动调整坐标轴范围
        all_x = [p['x'] for p in points]
        all_y = [p['y'] for p in points]
        ax.set_xlim(min(all_x) - 2, max(all_x) + 2)
        ax.set_ylim(min(all_y) - 2, max(all_y) + 2)

        plt.show()
        return hull
    # except Exception as e:
    #     print(f"Visualization error: {e}")
    #     return None


# 示例用法
if __name__ == "__main__":
    # 示例点集（随机点，包含可能的非凸多边形顶点）
    points = [
        {'x': 0, 'y': 0},
        {'x': 1, 'y': 2},
        {'x': 2, 'y': 1},
        {'x': 3, 'y': 3},
        {'x': 4, 'y': 0},
        {'x': 2, 'y': -1},
        {'x': 1, 'y': 0.5}
    ]

    hull = visualize_convex_hull(points)
    print("凸包顶点：", hull)
