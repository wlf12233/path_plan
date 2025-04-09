//
// Created by seer on 2025/3/28.
//

#include "DirectedGraph.h"

void DirectedGraph::addNode(const NodeId &node) {
    if (!graph.contains(node)) {
        graph[node] = std::unordered_map<NodeId, Weight>();
    }
}

void DirectedGraph::addEdge(const NodeId &node1, const NodeId &node2, Weight weight) {
    addNode(node1);
    addNode(node2);
    graph[node1][node2] = weight;
}

double DirectedGraph::getWeight(const NodeId &node1, const NodeId &node2) {
    return graph[node1][node2];
}

auto DirectedGraph::getNeighbors(const NodeId &node) {
    return graph[node];
}

auto DirectedGraph::getGraph() {
    return graph;
}
