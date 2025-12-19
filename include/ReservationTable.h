//
// Created by seer on 2025/12/16.
//

#ifndef PATH_PLAN_RESERVATIONTABLE_H
#define PATH_PLAN_RESERVATIONTABLE_H
#include <common.h>
#include <unordered_set>

class ReservationTable {
public:
    std::unordered_set<long long> vertex;
    std::unordered_set<long long> edge;

    long long vkey(int v, int t) const {
        return static_cast<long long>(v) << 32 | t;
    }

    long long ekey(int u, int v, int t) const {
        return static_cast<long long>(u) << 40 |
               static_cast<long long>(v) << 20 |
               t;
    }

    bool isReserved(int v, int t) const {
        return vertex.contains(vkey(v, t));
    }

    bool isEdgeReserved(int u, int v, int t) const {
        return edge.contains(ekey(v, u, t)); //反向
    }
};


#endif //PATH_PLAN_RESERVATIONTABLE_H
