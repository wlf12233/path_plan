import random

# 指定一些参数
POPULATION_SIZE = 100  # 种群大小，指的是遗传算法中个体的数量
GENERATIONS = 50  # 迭代代数，指定算法运行的总代数
MUTATION_RATE = 0.01  # 变异概率，每个个体在变异时有这个概率发生变异


# 适应度函数：计算个体的适应度（在这里是求平方）
def fitness(x):
    return x ** 2  # 返回输入x的平方


# 二进制字符串转十进制整数
def binary_to_decimal(binary_str):
    return int(binary_str, 2)  # 将二进制字符串转换为十进制数字


# 初始化种群：创建指定大小的随机种群
def initialize_population(size, gene_length):
    # 创建一个由随机二进制字符串组成的列表，长度为gene_length
    return [''.join(random.choice('01') for _ in range(gene_length)) for _ in range(size)]


# 选择操作：依据适应度选择个体
def selection(population):
    total_fitness = sum(fitness(binary_to_decimal(individual)) for individual in population)  # 计算总适应度
    # 根据适应度计算每个个体的选择概率
    probabilities = [fitness(binary_to_decimal(individual)) / total_fitness for individual in population]
    # 根据概率选择个体，生成新的种群
    return random.choices(population, weights=probabilities, k=len(population))


# 交叉操作：进行基因交叉以生成新的个体
def crossover(parent1, parent2):
    crossover_point = random.randint(1, len(parent1) - 1)  # 随机选择交叉点
    # 生成两个子个体，使用父母的基因进行交叉
    child1 = parent1[:crossover_point] + parent2[crossover_point:]
    child2 = parent2[:crossover_point] + parent1[crossover_point:]
    return child1, child2  # 返回生成的两个子个体


# 变异操作：随机变更个体的基因
def mutate(individual):
    # 使用随机数判断是否发生变异
    if random.random() < MUTATION_RATE:
        mutation_point = random.randint(0, len(individual) - 1)  # 随机选择变异点
        mutated_individual = list(individual)  # 将字符串转为列表以便修改
        # 变异，翻转选定的基因位（0变1，1变0）
        mutated_individual[mutation_point] = '1' if mutated_individual[mutation_point] == '0' else '0'
        return ''.join(mutated_individual)  # 返回变异后的个体
    return individual  # 如果没有发生变异，返回原个体


# 遗传算法主函数
def genetic_algorithm():
    gene_length = 5  # 设置基因长度为5位（对应范围0到31）
    population = initialize_population(POPULATION_SIZE, gene_length)  # 初始化种群

    # 进行多代迭代
    for generation in range(GENERATIONS):
        # 评估适应度，选择父代
        population = selection(population)

        new_population = []  # 准备存储新一代种群
        # 以成对的方式进行交叉和变异产生新个体
        for i in range(0, POPULATION_SIZE, 2):
            parent1 = population[i]  # 选择当前个体作为父母1
            # 确保不会越界；若没有足够的个体，则选择最后一个个体作为父母2
            parent2 = population[i + 1] if i + 1 < POPULATION_SIZE else population[-1]
            child1, child2 = crossover(parent1, parent2)  # 交叉生成两个子代
            # 将子代进行变异
            new_population.extend([mutate(child1), mutate(child2)])

        population = new_population  # 更新为新种群

        # 输出当前代的最佳个体和适应度
        best_individual = max(population, key=lambda ind: fitness(binary_to_decimal(ind)))
        print(
            f"Generation {generation}: Best Individual: {best_individual} (Decimal: {binary_to_decimal(best_individual)}, Fitness: {fitness(binary_to_decimal(best_individual))})")

    return best_individual  # 返回找到的最佳个体


# 运行遗传算法
best_solution = genetic_algorithm()
print(
    f"Best Solution: {best_solution} (Decimal: {binary_to_decimal(best_solution)}, Fitness: {fitness(binary_to_decimal(best_solution))})")

if __name__ == '__main__':
    print(best_solution)
