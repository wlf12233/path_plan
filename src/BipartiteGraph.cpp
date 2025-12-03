#include <vector>
//
// Created by seer on 2025/6/13.
//
#include <common.h>

class BipartiteGraph {
public:
    std::vector<std::vector<int> > graph;
    std::vector<int> color;
    std::vector<bool> visited;

    bool checkBipartiteGraph(const std::vector<std::vector<int> > &graph) {
        int n = graph.size();
        color.assign(n, 0);
        visited.assign(n, false);
        for (int i = 0; i < graph.size(); ++i) {
            if (!visited[i]) {
                color[i] = 0;
                if (!dfs(i)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool dfs(const int cur) {
        visited[cur] = true;
        for (int edge: graph[cur]) {
            if (visited[edge]) {
                if (color[edge] == color[cur]) return false;
            } else {
                visited[edge] = true;
                color[edge] = color[cur] ^ 1;
                if (!dfs(edge)) {
                    return false;
                }
            }
        }
        return true;
    }
};
