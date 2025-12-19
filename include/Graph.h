//
// Created by seer on 2025/12/5.
//

#ifndef PATH_PLAN_GRAPH_H
#define PATH_PLAN_GRAPH_H
#include <common.h>

struct Edge {
    int to;
    double weight;
};

struct Point {
    int id;
    std::vector<Edge> edges;
};
class Graph {
    std::vector<Point> graph;

public:
    void addEdge(int u, int v);

    void addWeightedEdge(int u, int v, double weight);

    void removeEdge(int u, int v);

    void changeWeight(int u, int v, double weight);

    const std::vector<Edge> &getNeighbors(int u) const;

    [[nodiscard]] const std::vector<Point> &getGraph() const {
        return graph;
    }

    void printGraph();
};


#endif //PATH_PLAN_GRAPH_H
