//
// Created by seer on 2025/6/4.
//
#include <stack>

#include "vector"
#include <iostream>
#include <queue>
class topoSort {
public:
    std::vector<int> topologicalSort(int V, const std::vector<std::vector<int> > &adj) {
        std::vector<int> indegree(V, 0);
        for (int i = 0; i < V; i++) {
            for (auto v: adj[i]) {
                indegree[v]++;
            }
        }
        std::queue<int> q;
        for (int i = 0; i < V; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        std::vector<int> topo;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);
            for (auto v: adj[u]) {
                indegree[v]--;
                if (indegree[v] == 0) {
                    q.push(v);
                }
            }
        }
        if (topo.size() != V) {
            throw std::runtime_error("topoSort: topological sort error");
        }
        return topo;
    }

    void dfs(int u, std::vector<std::vector<int> > &adj, std::vector<bool> &visited, std::stack<int> &stk) {
        visited[u] = true;
        for (auto v: adj[u]) {
            if (visited[v] == false) {
                dfs(v,adj,visited,stk);
            }
        }
        stk.push(u);

    }

    std::vector<int> topologicalSortWithDFS(int V, std::vector<std::vector<int> > &adj) {
        std::vector<bool> visited(V, false);
        std::stack<int> st;

        for (int i = 0; i < V; i++) {
            if (visited[i] == false) {
                dfs(i,adj,visited,st);
            }
        }
        std::vector<int> res;
        while (st.size() != 0) {
            res.push_back(st.top());
            st.pop();
        }
        return res;
    }
};
