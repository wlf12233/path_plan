//
// Created by seer on 2025/4/14.
//

#ifndef CBS_H
#define CBS_H

#include "common.h"
#include "Types.h"

class CBS {
public:
    CBS(const std::vector<std::pair<int, int> > &starts,
        const std::vector<std::pair<int, int> > &goals,
        const std::vector<std::vector<int> > &map);

    std::optional<std::vector<Path> > solve();

private:
    struct CBSNode {
        std::vector<Path> paths;
        std::vector<Constraint> constraints;
        int cost;

        bool operator>(const CBSNode &other) const {
            return cost > other.cost;
        }
    };

    std::optional<Conflict> findConflict(const std::vector<Path> &paths);

     Path aStarWithConstrains(int agent, const std::pair<int, int> &start, const std::pair<int, int> &goal,
                             std::vector<Constraint> &constraints);

    int computeTotalCost(const std::vector<Path> &paths);

    std::vector<std::pair<int, int> > starts;
    std::vector<std::pair<int, int> > goals;
    std::vector<std::vector<int> > map; // 0 = free, 1 = obstacle
};


#endif //CBS_H
