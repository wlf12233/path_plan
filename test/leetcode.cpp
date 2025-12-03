//
// Created by seer on 2025/12/2.
//
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> col, vis;

    bool isBipartite(vector<vector<int> > &graph) {
        const int n = graph.size();
        vis.assign(n+1, 0);
        col.assign(n+1, -1);
        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) {
                vis[i] = true;
                col[i] = 0;
                if (!dfs(i, graph)) return false;
            }
        }
        return true;
    }

    bool dfs(int cur, vector<vector<int> > &graph) {
        vis[cur] = 1;
        for (int v: graph[cur]) {
            if (vis[v]) {
                if (col[v] == col[cur]) return false;
            } else {
                col[v] = col[cur] ^ 1;
                if (!dfs(v, graph)) return false;
            }
        }
        return true;
    }
};
