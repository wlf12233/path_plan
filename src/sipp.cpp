//
// Created by seer on 2025/4/1.
//

#include "sipp.h"

SIPP::SIPP(int width, int height): width(width), height(height) {
    grid.resize(width, vector<vector<SafeInterval> >(height));
    moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
}

void SIPP::addSafeInterval(int x, int y, double start, double end) {
    grid[x][y].push_back({start, end});
}

std::vector<tuple<int, int, double> > SIPP::findPath(std::pair<int, int> start, std::pair<int, int> goal) {
    priority_queue<State, vector<State>, greater<State> > open_list;
    unordered_map<int, State> came_from;
    unordered_map<int, double> g_score;

    for (int i = 0; i < grid[start.first][start.second].size(); ++i) {
        auto safe_interval = grid[start.first][start.second][i];
        State init{start.first, start.second, safe_interval.start, i, 0, heuristic(start, goal)};
        open_list.push(init);
        g_score[hashState(init.x, init.y, init.intervalIdx)] = 0;
        came_from[hashState(init.x, init.y, init.intervalIdx)] = init;
    }

    State goal_state;

    while (!open_list.empty()) {
        State current = open_list.top();
        open_list.pop();
        if (current.x == goal.first && current.y == goal.second) {
            goal_state = current;
            break;
        }

        for (auto move: moves) {
            int nx = current.x + move.first, ny = current.y + move.second;
            if (!isValid(nx, ny)) {
                continue;
            }
            for (int i = 0; i < grid[nx][ny].size(); ++i) {
                SafeInterval safe_interval = grid[nx][ny][i];

                double arrival_time = max(current.t + 1, safe_interval.start);
                // double arrival_time = current.t + 1;

                if (arrival_time <= safe_interval.end) {
                    double new_g = current.g + 1;
                    int key = hashState(nx, ny, i);

                    if (g_score.find(key) == g_score.end() || new_g < g_score[key]) {
                        g_score[key] = new_g;
                        State next{nx, ny, arrival_time, i, new_g, heuristic({nx, ny}, goal)};
                        open_list.push(next);
                        came_from[key] = current;
                    }
                }
            }
        }
    }
    vector<tuple<int, int, double> > path;
    State cur = goal_state;
    while (true) {
        path.push_back({cur.x, cur.y, cur.t});
        int key = hashState(cur.x, cur.y, cur.intervalIdx);
        if (came_from.find(key) == came_from.end() || came_from[key].x == cur.x && came_from[key].y == cur.y) {
            break;
        }
        State prev = came_from[key];

        // vector<tuple<int, int, double> > wait_steps;
        // int wait_x = prev.x, wait_y = prev.y;
        // double wait_time = prev.t;
        // while (wait_time + 1 < cur.t) {
        //     wait_time += 1;
        //     wait_steps.push_back({wait_x, wait_y, wait_time});
        // }
        // for (auto wait_step : wait_steps) {
        //     path.push_back(wait_step);
        // }

        cur = prev;
    }
    ranges::reverse(path);
    return path;
}

bool SIPP::isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

double SIPP::heuristic(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<pair<int, int> > SIPP::reconstructPath(State goal) {
    return {{goal.x, goal.y}};
}

int SIPP::hashState(int x, int y, int idx) {
    return x * 10000 + y * 100 + idx;
}
