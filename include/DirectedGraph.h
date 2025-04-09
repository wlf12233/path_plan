//
// Created by seer on 2025/3/28.
//

#ifndef DirectedGraph_H
#define DirectedGraph_H

#include "common.h"

class DirectedGraph {
public:
    using NodeId = std::string;
    using Weight = double;
    using Graph = std::unordered_map<NodeId, std::unordered_map<NodeId, Weight> >;

    void addNode(const NodeId &node);

    void addEdge(const NodeId &node1, const NodeId &node2, Weight weight);

    double getWeight(const NodeId &node1, const NodeId &node2);

    auto getNeighbors(const NodeId &node);

    auto getGraph();

    void printGraph() {
        for (const auto &[fst, snd]: graph) {
            auto node = fst;
            auto neighbors = snd;
            std::cout << node << "->";
            for (const auto &[fst, snd]: neighbors) {
                std::cout << fst << "(" << snd << ")";
            }
            std::cout << std::endl;
        }
    }

    std::vector<NodeId> reconstruct_path(std::unordered_map<NodeId, NodeId> &came_from, NodeId current) {
        std::vector<NodeId> path;
        while (came_from.contains(current)) {
            path.push_back(current);
            current = came_from[current];
        }
        path.push_back(current);
        std::ranges::reverse(path);
        return path;
    }

private:
    Graph graph;
};


#endif //DirectedGraph_H
