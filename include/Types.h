//
// Created by seer on 2025/4/14.
//

#ifndef TYPES_H
#define TYPES_H
#include <common.h>

/**
 * 表示约束（某个 agent 不能在某时间到达某位置）
 */
struct Constraint {
    int agent;
    int x, y;
    int time;

    bool operator ==(const Constraint &other) const {
        return agent == other.agent && x == other.x && y == other.y && time == other.time;
    }
};

using Path = std::vector<std::pair<int, int> >; //路径
/**
 * 冲突
 */
struct Conflict {
    int agent1, agent2;
    int x, y;
    int time;

    Conflict(int agent1, int agent2, int x, int y, int time) : agent1(agent1), agent2(agent2), x(x), y(y), time(time) {
    }
};

namespace std {
    template<>
    struct hash<Constraint> {
        std::size_t operator()(const Constraint &c) const noexcept {
            return ((std::hash<int>()(c.agent) ^ (std::hash<int>()(c.x) << 1)) >> 1) ^
                   (std::hash<int>()(c.y) << 1) ^ (std::hash<int>()(c.time) << 2);
        }
    };
}

#endif //TYPES_H
