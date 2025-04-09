//
// Created by seer on 2025/3/26.
//

#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <optional>

#include <algorithm>
#include <functional>

struct Node {
    int x, y; // 位置
    int g_cost; //从起点到当前节点的代价
    int h_cost; //启发式代价
    int f_cost() const { return h_cost + g_cost; };
    std::shared_ptr<Node> parent;

    Node(int _x, int _y, int _gcost, int _hcost, std::shared_ptr<Node> parent = nullptr);
};

struct CompareNode {
    bool operator()(const std::shared_ptr<Node> &p1, const std::shared_ptr<Node> &p2) const {
        return p1->f_cost() > p2->f_cost();
    }
};

class AStar {
public:
    using Heuristic = std::function<int(int, int, int, int)>;

    AStar(int rows, int cols, Heuristic heuristic);

    // int operator+(const AStar &other) const {
    //     return other.cols;
    // }

    std::optional<std::vector<std::pair<int, int> > > find_path_1(
        const std::vector<std::vector<int> > &grid,
        std::pair<int, int> start,
        std::pair<int, int> goal);

    std::optional<std::vector<std::pair<int, int> > > find_path_2(
        const std::vector<std::vector<int> > &grid,
        std::pair<int, int> start,
        std::pair<int, int> goal);

    std::optional<std::vector<std::pair<int, int> > > find_path_3(
        const std::string &start, const std::string &goal);

private:
    int rows, cols;

    Heuristic heuristic;

    // int heuristic(int x1, int y1, int x2, int y2);
};

// 曼哈顿距离
inline int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 欧式距离
inline int euclidean(int x1, int y1, int x2, int y2) {
    return static_cast<int>(std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

// 对角距离
inline int diagonal(int x1, int y1, int x2, int y2) {
    return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
}


#endif //ASTAR_H
