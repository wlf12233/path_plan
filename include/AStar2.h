//
// Created by seer on 2025/12/16.
//

#ifndef PATH_PLAN_ASTAR2_H
#define PATH_PLAN_ASTAR2_H
#include <common.h>
#include <Graph.h>
#include <dijkstra.h>

class AStar2 {
    struct Node {
        int id;
        int t;
        double g, h;

        int get_f() const {
            return g + h;
        }

        std::shared_ptr<Node> parent;

        Node(int id, double g, double h, std::shared_ptr<Node> parent = nullptr);
    };

    struct CompareNode {
        bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b) const {
            return a->get_f() > b->get_f();
        }
    };

    std::vector<std::vector<double> > dis;

public:
    AStar2(const std::vector<Point> &graph) {
        dijkstra dijkstra;
        dijkstra.computeAllPaths(graph);
        dis = dijkstra.dis;
    }

    std::vector<int> findPath(int source, int target, std::vector<Point> &graph);
};


#endif //PATH_PLAN_ASTAR2_H
