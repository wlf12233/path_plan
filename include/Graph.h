//
// Created by seer on 2025/12/5.
//

#ifndef PATH_PLAN_GRAPH_H
#define PATH_PLAN_GRAPH_H
#include <common.h>

class Graph {
    struct Edge {
        int from;
        int to;
        double weight;
    };

    struct Node {
        int id;
        std::vector<Edge> edges;
    };

    std::vector<Node> graph;

public:
    void addEdge(int u, int v);

    void addWeightedEdge(int u, int v, double weight);

    void removeEdge(int u, int v);

    void changeWeight(int u, int v, double weight);

    const std::vector<Edge> &getNeighbors(int u) const;

    void printGraph();
};


#endif //PATH_PLAN_GRAPH_H
