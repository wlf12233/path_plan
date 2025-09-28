import numpy as np
import random


# 目标函数，假设我们要最小化 f(x) = x^2
def fitness(x):
    return x ** 2  # 返回 x 的平方，即我们的目标函数值


# 粒子类，用于表示每个粒子的位置和速度以及其个体最优
class Particle:
    def __init__(self, bounds):
        # 随机初始化粒子的位置在给定的边界内
        self.position = np.random.uniform(bounds[0], bounds[1])
        # 随机初始化粒子的速度
        self.velocity = np.random.uniform(-1, 1)
        # 粒子的历史最优位置设为其初始位置
        self.best_position = self.position
        # 粒子的历史最优值设为对应的适应度值
        self.best_value = fitness(self.position)

    # 粒子群优化算法主函数


def particle_swarm_optimization(num_particles, bounds, max_iter):
    # 初始化粒子群体，根据给定数量生成粒子
    particles = [Particle(bounds) for _ in range(num_particles)]
    # 找到粒子群中的全局最优位置与对应的适应度值
    global_best_position = min(particles, key=lambda p: p.best_value).best_position
    global_best_value = fitness(global_best_position)

    # 迭代优化过程
    for iteration in range(max_iter):
        for particle in particles:
            # 更新速度
            inertia_weight = 0.5  # 惯性权重，控制粒子上次速度对当前速度的影响
            cognitive_weight = 1.5  # 认知权重，粒子个人最优对速度的影响
            social_weight = 1.5  # 社会性权重，群体全局最优对速度的影响

            # 生成0到1之间的随机数
            r1 = np.random.rand()  # 第一随机数，用于个人最优影响
            r2 = np.random.rand()  # 第二随机数，用于全局最优影响

            # 更新粒子的速度
            particle.velocity = (inertia_weight * particle.velocity +  # 惯性部分
                                 cognitive_weight * r1 * (particle.best_position - particle.position) +  # 认知部分
                                 social_weight * r2 * (global_best_position - particle.position))  # 社会性部分

            # 更新粒子的位置
            particle.position += particle.velocity

            # 计算新位置的适应度值
            current_value = fitness(particle.position)

            # 如果新适应度值优于粒子历史最佳适应度值，则更新最佳位置和最佳值
            if current_value < particle.best_value:
                particle.best_value = current_value
                particle.best_position = particle.position

                # 更新全局最佳位置
        for particle in particles:
            if particle.best_value < global_best_value:  # 如果某个粒子的历史最佳适应度优于当前全局最佳
                global_best_value = particle.best_value  # 更新全局最佳适应度
                global_best_position = particle.best_position  # 更新全局最佳位置

        # 可选：输出当前代的全局最优位置和适应度值
        print(f"Iteration {iteration + 1}: Best Value = {global_best_value}, Best Position = {global_best_position}")

        # 返回全局最佳位置和适应度值
    return global_best_position, global_best_value


# 主程序部分，设置参数并运行算法
num_particles = 30  # 粒子数量
bounds = (-10, 10)  # 搜索空间的边界
max_iter = 100  # 最大迭代次数

# 运行粒子群优化算法
best_position, best_value = particle_swarm_optimization(num_particles, bounds, max_iter)
print(f"Global Best Position: {best_position}, Global Best Value: {best_value}")

if __name__ == '__main__':
    print(best_position, best_value)
