//
// Created by seer on 2025/8/6.
//

#ifndef SIPP2_H
#define SIPP2_H
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <ratio>

using namespace std;

struct Interval {
    int start, end;

    Interval(int start, int end) : start(start), end(end) {
    }

    bool operator==(const Interval &other) const {
        return start == other.start && end == other.end;
    }
};

struct Node {
    int x, y;
    Interval interval;
    int g;
    int h;
    int f;
    Node *parent;
    int arrival_time; //到达时间
    Node(int x, int y, Interval interval, int g, int h, Node *parent, int arrival): x(x), y(y), interval(interval), g(g), h(h),
                                                                                    parent(parent), arrival_time(arrival) {
    }

    bool operator<(const Node &other) const {
        return f > other.f;
    }
};

// 定义网格环境
struct Grid {
    int width;
    int height;
    vector<vector<bool> > static_obstacles; // 静态障碍物地图

    Grid(int w, int h) : width(w), height(h) {
        static_obstacles.resize(w, vector<bool>(h, false));
    }

    bool is_valid(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height && !static_obstacles[x][y];
    }
};

// 定义动态障碍物
struct DynamicObstacle {
    int x, y; // 障碍物位置
    int start_time; // 开始时间
    int end_time; // 结束时间

    DynamicObstacle(int x, int y, int start, int end)
        : x(x), y(y), start_time(start), end_time(end) {
    }
};

// 计算曼哈顿距离作为启发式函数
int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 计算所有网格单元的安全区间
unordered_map<int, unordered_map<int, vector<Interval> > > compute_safe_intervals(const Grid &grid,
                                                                                  const vector<DynamicObstacle> &obstacles,
                                                                                  int max_time) {
    unordered_map<int, unordered_map<int, vector<Interval> > > safe_intervals;
    for (int i = 0; i < grid.width; ++i) {
        for (int j = 0; j < grid.height; ++j) {
            if (grid.is_valid(i, j)) {
                safe_intervals[i][j].emplace_back(0, max_time);
            }
        }
    }
    for (auto &obstacle: obstacles) {
        int x = obstacle.x, y = obstacle.y;
        if (!grid.is_valid(x, y)) {
            continue;
        }
        vector<Interval> new_intervals;
        for (auto interval: safe_intervals[x][y]) {
            // 如果障碍物时间与当前区间无重叠
            if (obstacle.end_time < interval.start || obstacle.start_time > interval.end) {
                new_intervals.push_back(interval);
            }
            // 如果障碍物完全覆盖当前区间
            else if (obs.start_time <= interval.start && obs.end_time >= interval.end) {
                continue; // 跳过这个区间
            }
            // 如果障碍物部分覆盖当前区间
            else {
                if (obstacle.start_time > interval.start) {
                    new_intervals.push_back({interval.start, obstacle.start_time - 1});
                }
                // 区间在障碍物之后的部分
                if (obstacle.end_time < interval.end) {
                    new_intervals.push_back({obstacle.end_time + 1, interval.end});
                }
            }
        }
        safe_intervals[x][y] = new_intervals;
    }
    return safe_intervals;
}

// 检查时间点是否在安全区间内
bool is_in_safe_interval(int time, const vector<Interval> &intervals) {
    for (const auto &interval: intervals) {
        if (time >= interval.start && time <= interval.end) {
            return true;
        }
    }
    return false;
}

// 查找可以到达目标区间的到达时间
pair<bool, int> find_arrival_time(int start_time, int move_duration, const vector<Interval> &target_intervals) {
    for (const auto &interval: target_intervals) {
        int earliest_arrival = max(start_time + move_duration, interval.start);
        if (earliest_arrival <= interval.end) {
            return {true, earliest_arrival};
        }
    }
    return {false, -1};
}

vector<pair<int, int> > sipp(const Grid &grid, pair<int, int> start,
                             pair<int, int> goal,
                             const vector<DynamicObstacle> &obstacles,
                             int max_time) {
    // 计算安全区间
    auto safe_intervals = compute_safe_intervals(grid, obstacles, max_time);

    // 优先队列（开放列表）
    priority_queue<Node> open_list;
    // 关闭列表（记录已扩展的节点）
    unordered_map<int, unordered_map<int, unordered_map<Interval, bool> > > closed_list;
    // 初始化起点
    if (safe_intervals[start.first][start.second].empty()) {
        return {}; // 起点没有安全区间，无解
    }
    // 使用第一个安全区间作为起点的安全区间
    Interval start_interval = safe_intervals[start.first][start.second][0];
    int h = manhattan_distance(start.first, start.second, goal.first, goal.second);
    open_list.emplace(start.first, start.second, start_interval, 0, h, nullptr, 0);
    // 定义移动方向（上、下、左、右、等待）
    vector<pair<int, int> > directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {0, 0}};
    while (!open_list.empty()) {
        Node current = open_list.top();
        open_list.pop();

        //是否到达目标
        if (current.x == goal.first && current.y == goal.second) {
            vector<pair<int, int> > path;
            Node *node = &current;
            while (node != nullptr) {
                path.push_back({
                    node->x, node->y
                });
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        // 检查是否已经在关闭列表中
        if (closed_list[current.x][current.y][current.interval]) {
            continue;
        }
        closed_list[current.x][current.y][current.interval] = true;
        for (auto direction: directions) {
            int new_x = current.x + direction.first;
            int new_y = current.y + direction.second;
            if (!grid.is_valid(new_x, new_y)) {
                continue;
            }
            // 计算移动时间（等待为1，移动为1）
            int move_duration = (direction.first == 0 && direction.second == 0) ? 1 : 1;
            for (auto &target_interval: safe_intervals[new_x][new_y]) {
                auto [found,arrival_time] = find_arrival_time(current.arrival_time, move_duration, {target_interval});
                if (found) {
                    // 检查移动过程中是否会与动态障碍物碰撞
                    bool collision = false;
                    for (auto obstacle: obstacles) {
                        if (obstacle.x == new_x && obstacle.y == new_y) {
                            if (arrival_time >= obstacle.start_time && arrival_time <= obstacle.end_time) {
                                collision = true;
                                break;
                            }
                        }
                    }
                    if (!collision) {
                        int new_g = current.g + move_duration;
                        int new_h = manhattan_distance(new_x, new_y, goal.first, goal.second);
                        Node* parent = new Node(current);
                        open_list.emplace(new_x, new_y, target_interval, new_g, new_h, parent, arrival_time);
                    }
                }
            }
        }
    }
    return {};
}
#endif //SIPP2_H
