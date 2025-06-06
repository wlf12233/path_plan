//
// Created by seer on 2025/6/5.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <ranges>
using namespace std;

class Graph {
public:
    bool canVisitAllRooms(vector<vector<int> > &rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);
        function<void(int)> dfs = [&](int i) {
            if (visited[i]) {
                return;
            }
            visited[i] = true;
            for (auto j: rooms[i]) {
                dfs(j);
            }
        };
        dfs(0);
        for (int i = 1; i < n; i++) {
            if (!visited[i]) {
                return false;
            }
        }
        return true;
    }

    vector<int> shortestAlternatingPaths(int n, vector<vector<int> > &redEdges, vector<vector<int> > &blueEdges) {
        vector<int> res(n, -1);
        res[0] = 0;
        vector<vector<int> > redAdj(n);
        vector<vector<int> > blueAdj(n);
        vector<vector<pair<int, int> > > adj(n);
        for (auto red_edge: redEdges) {
            redAdj[red_edge[0]].push_back(red_edge[1]);
        }
        for (auto blue_edge: blueEdges) {
            blueAdj[blue_edge[0]].push_back(blue_edge[1]);
        }
        queue<tuple<int, int, int> > q;
        vector<vector<bool> > visited(n, vector<bool>(2, false));
        q.push({0, 0, 1});
        q.push({0, 0, 0});
        visited[0][0] = true;
        visited[0][1] = true;
        while (!q.empty()) {
            auto [cur,step,c] = q.front();
            q.pop();

            auto &next_edg = c == 0 ? blueAdj : redAdj;
            auto next_c = c == 0 ? 1 : 0;

            for (int value: next_edg[cur]) {
                if (!visited[value][next_c]) {
                    visited[value][next_c] = true;
                    q.push({value, step + 1, next_c});
                    if (res[value] == -1 || res[value] > step + 1) {
                        res[value] = step + 1;
                    }
                }
            }
        }
        return res;
    }

    bool canFinish(int numCourses, vector<vector<int> > &prerequisites) {
        vector<int> indegree(numCourses, 0);
        vector<vector<int> > adj(numCourses);
        for (auto prerequisite: prerequisites) {
            adj[prerequisite[1]].push_back(prerequisite[0]);
            indegree[prerequisite[0]]++;
        }
        queue<int> q;
        for (int i = 0; i < indegree.size(); i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        vector<int> ans;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            ans.push_back(cur);
            for (auto neighbour: adj[cur]) {
                indegree[neighbour]--;
                if (indegree[neighbour] == 0) {
                    q.push(neighbour);
                }
            }
        }
        if (ans.size() != numCourses) {
            return false;
        }
        return true;
    }

    vector<int> findOrder(int numCourses, vector<vector<int> > &prerequisites) {
        vector<int> indegree(numCourses, 0);
        vector<vector<int> > adj(numCourses);
        for (auto prerequisite: prerequisites) {
            adj[prerequisite[1]].push_back(prerequisite[0]);
            indegree[prerequisite[0]]++;
        }
        vector<bool> visited(numCourses, false);
        vector<bool> onPath(numCourses, false); // 标记当前路径
        vector<int> ans;
        bool hasCycle = false;

        function<void(int)> dfs = [&](int i) {
            if (onPath[i]) {
                hasCycle = true;
                return;
            }
            if (visited[i]||hasCycle) {
                return;
            }
            visited[i] = true;
            onPath[i] = true;
            for (auto neighbour: adj[i]) {
                dfs(neighbour);
            }
            onPath[i] = false;
            ans.push_back(i);
        };
        for (int i = 0; i < numCourses; i++) {
            if (!visited[i]) {
                dfs(i);
            }
        }
        if (hasCycle) {
            return {};
        }
        ranges::reverse(ans);

        return ans;
    }
};
