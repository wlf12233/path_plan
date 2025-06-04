//
// Created by w on 2025/6/4.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Graph {
public:
    vector<int> eventualSafeNodes(vector<vector<int>> &graph) {
        auto n = graph.size();
        vector<int> col(n);
        function<bool(int)> safe = [&](int x) {
            if (col[x] > 0) {
                return col[x] == 2;
            }
            col[x] = 1;
            for (const auto &item: graph[x]) {
                if (!safe(item)) {
                    return false;
                }
            }
            col[x] = 2;
            return true;
        };
        vector<int> res;
        for (int i = 0; i < n; ++i) {
            if (safe(i)){
                res.push_back(i);
            }
        }
        return res;
    }


    int findCircleNum(vector<vector<int>> &isConnected) {
        int n = isConnected.size();
        std::vector<vector<int>> adj(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (isConnected[i][j]) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
        int res = 0;
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                res++;
                dfs(i, visited, adj);
            }
        }
        return res;
    }

    void dfs(int v, vector<bool> &visited, const vector<vector<int>> &adj) {
        if (visited[v]) {
            return;
        }
        visited[v] = true;
        for (const auto &item: adj[v]) {
            dfs(item, visited, adj);
        }
    }
};