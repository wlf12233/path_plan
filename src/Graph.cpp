//
// Created by seer on 2025/12/5.
//

#include "Graph.h"

#include "fstream"
#include "Johnson.h"

void Graph::addEdge(int u, int v) {
    graph[u].edges.emplace_back(u, v, 0.0);
}

void Graph::addWeightedEdge(int u, int v, double weight) {
    graph[u].edges.emplace_back(u, v, weight);
}

void Graph::removeEdge(int u, int v) {
    std::erase_if(graph[u].edges,
                  [v](Edge const &edge) { return edge.v == v; });
}

void Graph::changeWeight(int u, int v, double weight) {
    for (auto &edge: graph[u].edges) {
        if (edge.v == v) {
            edge.weight = weight;
            return;
        }
    }
}

const std::vector<Graph::Edge> &Graph::getNeighbors(int u) const {
    return graph[u].edges;
}

void Graph::printGraph() {
    int n = graph.size();
    std::vector mat(n, std::vector<double>(n, -1));
    for (auto [id,edge]: graph) {
        for (auto [u,v,w]: edge) {
            mat[u][v] = w;
        }
    }
    std::cout << "  ";
    for (int j = 0; j < n; ++j) std::cout << std::setw(6) << j;
    std::cout << '\n';

    for (int i = 0; i < n; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < n; ++j) {
            if (mat[i][j] == -1) {
                std::cout << std::setw(6) << "∞";
            } else {
                std::cout << std::setw(6) << mat[i][j];
            }
        }
        std::cout << '\n';
    }
}
