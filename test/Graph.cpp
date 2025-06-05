//
// Created by seer on 2025/6/5.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
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
};
