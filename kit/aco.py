import numpy as np
import random
import matplotlib.pyplot as plt


class AntColony:
    def __init__(self, num_ants, n_iterations, alpha, beta, evaporation_rate, q0, city_distances):
        """
        初始化蚁群算法的参数。

        :param num_ants: 蚂蚁数量
        :param n_iterations: 迭代次数
        :param alpha: 信息素重要程度
        :param beta: 启发函数重要程度
        :param evaporation_rate: 信息素挥发率
        :param q0: 贪婪因子
        :param city_distances: 城市间距离矩阵
        """
        self.num_ants = num_ants  # 蚂蚁数量
        self.n_iterations = n_iterations  # 迭代次数
        self.alpha = alpha  # 信息素的重要性（越大表示越看重信息素）
        self.beta = beta  # 启发函数的重要性（越大表示越看重启发函数）
        self.evaporation_rate = evaporation_rate  # 信息素挥发率，表示信息素减少的速度
        self.q0 = q0  # 贪婪因子，决定蚂蚁选择下一城市时的随机性
        self.city_distances = city_distances  # 城市间的距离矩阵
        self.num_cities = len(city_distances)  # 城市数量

        # 初始化信息素矩阵，初始值为城市数量的倒数
        self.pheromone = np.ones((self.num_cities, self.num_cities)) / self.num_cities

    def calculate_visibility(self):
        """计算城市间的启发函数（距离的倒数），使近的城市更容易被选择。"""
        return 1 / self.city_distances  # 启发函数为距离的倒数

    def select_next_city(self, current_city, visited):
        """
        从当前城市选择下一个城市。

        :param current_city: 当前城市索引
        :param visited: 访问状态数组，已访问的城市会被排除
        :return: 下一个城市的索引
        """
        visibility = self.visibility[current_city]  # 当前城市的可见性（启发度）
        pheromone = self.pheromone[current_city]  # 当前城市的信息素

        # 计算选择每个城市的概率
        probabilities = (
                pheromone ** self.alpha * visibility ** self.beta  # 选择概率的计算
        )
        probabilities[visited] = 0  # 排除已访问的城市
        probabilities /= np.sum(probabilities)  # 归一化选择概率

        # 根据贪婪因子选择下一个城市
        if random.random() < self.q0:
            next_city = np.argmax(probabilities)  # 选择概率最大的城市
        else:
            next_city = np.random.choice(range(self.num_cities), p=probabilities)  # 随机选择城市

        return next_city

    def update_pheromone(self, all_routes):
        """
        根据所有蚂蚁的路径更新信息素。

        :param all_routes: 所有蚂蚁路径的列表
        """
        # 信息素挥发
        self.pheromone *= (1 - self.evaporation_rate)

        # 对每条路径更新信息素
        for route in all_routes:
            route_length = self.calculate_route_length(route)  # 计算路径长度
            pheromone_contribution = 1.0 / route_length  # 信息素增加的贡献
            for i in range(len(route) - 1):
                # 在路径的每一对城市上增加信息素
                self.pheromone[route[i], route[i + 1]] += pheromone_contribution

    def calculate_route_length(self, route):
        """
        计算一条路径的总长度。

        :param route: 城市路径
        :return: 路径的总长度
        """
        length = 0
        for i in range(len(route) - 1):
            length += self.city_distances[route[i], route[i + 1]]  # 累加城市间的距离
        length += self.city_distances[route[-1], route[0]]  # 回到起点的距离
        return length

    def optimize(self):
        """执行优化过程，返回最佳路径和最佳长度。"""
        best_route = None  # 最佳路径
        best_length = float('inf')  # 最佳路径长度初始为无穷大

        self.visibility = self.calculate_visibility()  # 计算可见性（启发函数）

        # 开始迭代
        for iteration in range(self.n_iterations):
            all_routes = []
            for _ in range(self.num_ants):
                visited = [False] * self.num_cities  # 访问状态初始化
                current_city = random.randint(0, self.num_cities - 1)  # 随机选择起始城市
                route = [current_city]  # 初始化路径
                visited[current_city] = True  # 标记为已访问

                # 构建一条完整路径
                for _ in range(self.num_cities - 1):
                    next_city = self.select_next_city(current_city, visited)  # 选择下一个城市
                    route.append(next_city)  # 添加到路径
                    visited[next_city] = True  # 标记为已访问
                    current_city = next_city  # 更新当前城市

                all_routes.append(route)  # 保存蚂蚁的路径
                route_length = self.calculate_route_length(route)  # 计算路径长度

                # 更新全局最优路径
                if route_length < best_length:
                    best_length = route_length  # 更新最佳长度
                    best_route = route  # 更新最佳路径

            print(f"Iteration {iteration + 1}, Best Length: {best_length}")  # 输出当前迭代的最佳长度
            self.update_pheromone(all_routes)  # 更新信息素

        return best_route, best_length  # 返回结果


# 示例数据：城市距离矩阵
city_distances = np.array([
    [0, 2, 9, 10],  # 城市 0 到其他城市的距离
    [3, 0, 6, 4],  # 城市 1 到其他城市的距离
    [15, 7, 0, 8],  # 城市 2 到其他城市的距离
    [6, 3, 12, 0]  # 城市 3 到其他城市的距离
])

# 参数设置
num_ants = 10  # 蚂蚁数量
n_iterations = 100  # 迭代次数
alpha = 1  # 信息素的重要性
beta = 2  # 启发函数的重要性
evaporation_rate = 0.5  # 信息素挥发率
q0 = 0.9  # 贪婪因子

# 创建蚁群算法对象并执行优化
aco = AntColony(num_ants, n_iterations, alpha, beta, evaporation_rate, q0, city_distances)
best_route, best_length = aco.optimize()  # 开始优化

# 输出最佳路径和最佳长度
print(f"Best Route: {best_route}, Best Length: {best_length}")

# 可视化结果
plt.plot(best_route + [best_route[0]], 'bo-')  # 绘制最佳路径（回到起点）
plt.title('Best Route Found')  # 图表标题
plt.xlabel('City Index')  # x 轴标签
plt.ylabel('City Index')  # y 轴标签
plt.grid()  # 显示网格
plt.show()  # 展示图表

# import java.util.Arrays;
#
# public class TravelingSalesman {
#     private static final int INF = Integer.MAX_VALUE; // 定义无穷大
#
#     public static void main(String[] args) {
#         // 示例距离矩阵，表示城市之间的距离
#         int[][] distance = {
#             {0, 10, 15, 20},
#             {10, 0, 35, 25},
#             {15, 35, 0, 30},
#             {20, 25, 30, 0}
#         };
#
#         int n = distance.length; // 城市数量
#         int shortestPathLength = tsp(distance, n);
#         System.out.println("最短路径长度: " + shortestPathLength);
#     }
#
#     public static int tsp(int[][] distance, int n) {
#         // dp[mask][i]: 到达状态mask（已访问的城市）并且最后访问城市i的最短路径
#         int[][] dp = new int[1 << n][n];
#
#         // 初始化 dp 数组为无穷大
#         for (int[] row : dp) {
#             Arrays.fill(row, INF);
#         }
#
#         // 初始状态：只访问起点城市0
#         dp[1][0] = 0;
#
#         // 循环所有可能的状态
#         for (int mask = 1; mask < (1 << n); mask++) {
#             for (int u = 0; u < n; u++) {
#                 // 如果城市u不在mask中，跳过
#                 if ((mask & (1 << u)) == 0) continue;
#
#                 // 遍历所有城市尝试到达新城市v
#                 for (int v = 0; v < n; v++) {
#                     // 如果城市v已经在mask中或距离为无穷大，跳过
#                     if ((mask & (1 << v)) != 0 || distance[u][v] == INF) continue;
#
#                     // 更新状态
#                     int newMask = mask | (1 << v); // 更新mask，表示v城市已被访问
#                     dp[newMask][v] = Math.min(dp[newMask][v], dp[mask][u] + distance[u][v]);
#                 }
#             }
#         }
#
#         // 找出从所有城市返回到起始城市的最小路径
#         int finalMask = (1 << n) - 1; // 所有城市都已访问
#         int minTourCost = INF;
#
#         for (int u = 1; u < n; u++) {
#             // 返回到起始城市0的路径成本
#             minTourCost = Math.min(minTourCost, dp[finalMask][u] + distance[u][0]);
#         }
#
#         return minTourCost;
#     }
# }