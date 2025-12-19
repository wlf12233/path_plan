//
// Created by seer on 2025/12/16.
//

#ifndef PATH_PLAN_CASTAR_H
#define PATH_PLAN_CASTAR_H
#include <common.h>
#include <graph.h>
#include <dijkstra.h>
#include <ReservationTable.h>

class CAStar {
    struct TimeNode {
        int v;
        int t;
    };
    struct STATE {
        int v;
        int t;
        double g, f;
    };
    struct Cmp {
        bool operator()(const STATE &s1, const STATE &s2) const {
            return s1.f > s2.f; //小f优先
        }
    };

    std::vector<std::vector<double> > dis;

    std::vector<int> findPath(int source, int target, std::vector<Point> &graph);

    std::vector<TimeNode> findPathWithRT(int source, int target, std::vector<Point> &graph, ReservationTable &rt);

    void reserverPath(std::vector<TimeNode> &paths, ReservationTable &rt);
};

#endif //PATH_PLAN_CASTAR_H
