//
// Created by seer on 2025/6/9.
//

#ifndef FLOYD_H
#define FLOYD_H

#include "common.h"

void Floyd(std::vector<std::vector<int> > &graph) {
    int V = graph.size();
    std::vector<std::vector<long> > dist(V);
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            dist[i][j] = graph[i][j];
        }
    }
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    for (int i = 0; i < V; ++i) {
        if (dist[i][i] < 0) {
            //存在负权回路
            return;
        }
    }
}

#endif //FLOYD_H
