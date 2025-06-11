//
// Created by seer on 2025/6/5.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <ranges>
#include <set>
#include <unordered_set>

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
            auto [cur, step, c] = q.front();
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
            if (visited[i] || hasCycle) {
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

    int nearestExit(vector<vector<char> > &maze, vector<int> &entrance) {
        const vector<int> dx{1, -1, 0, 0};
        const vector<int> dy{0, 0, 1, -1};
        int m = maze.size(), n = maze[0].size();
        int res = 0;
        queue<pair<int, int> > q;
        q.push({entrance[0], entrance[1]});

        maze[entrance[0]][entrance[1]] = '+';

        while (!q.empty()) {
            int size = q.size();
            for (int j = 0; j < size; j++) {
                auto [x, y] = q.front();
                q.pop();

                if ((x != entrance[0] || y != entrance[1]) && (x == 0 || x == m - 1 || y == 0 || y == n - 1)) {
                    return res;
                }

                for (int i = 0; i < 4; i++) {
                    auto nx = x + dx[i];
                    auto ny = y + dy[i];
                    if (nx >= 0 && ny >= 0 && nx < m && ny < n && maze[nx][ny] == '.') {
                        maze[nx][ny] = '+'; // 标记访问
                        q.push({nx, ny});
                    }
                }
            }
            res++;
        }
        return -1;
    }

    int shortestBridge(vector<vector<int> > &grid) {
        const vector<int> dx{1, -1, 0, 0};
        const vector<int> dy{0, 0, 1, -1};
        vector<vector<int> > dirs{{1,  0},
                                  {-1, 0},
                                  {0,  1},
                                  {0,  -1}};
        int m = grid.size();
        int res = 0;
        queue<pair<int, int> > q;
        function<void(int x, int y)> dfs = [&](int x, int y) {
            if (x < 0 || y < 0 || x >= m || y >= m || grid[x][y] != 1) {
                return;
            }
            grid[x][y] = 2;
            q.push({x, y});
            for (auto dir: dirs) {
                dfs(x + dir[0], y + dir[1]);
            }
        };
        bool found = false;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1 && found == false) {
                    dfs(i, j);
                    found = true;
                }
            }
        }
        while (!q.empty()) {
            int size = q.size();

            for (int j = 0; j < size; ++j) {
                auto [x, y] = q.front();
                q.pop();
                for (int i = 0; i < 4; ++i) {
                    auto nx = x + dx[i];
                    auto ny = y + dy[i];
                    if (nx < 0 || nx >= m || ny < 0 || ny >= m) {
                        continue;
                    }
                    if (grid[nx][ny] == 1) {
                        return res;
                    }
                    if (grid[nx][ny] == 0) {
                        grid[nx][ny] = 2;
                        q.push({nx, ny});
                    }
                }
            }
            res++;
        }
        return -1;
    }

    int minMutation(string startGene, string endGene, vector<string> &bank) {
        if (startGene == endGene) {
            return 0;
        }
        if (ranges::find(bank, endGene) == bank.end()) {
            return -1;
        }
        vector<char> c{'A', 'C', 'G', 'T'};
        queue<string> q;
        int res = 0;
        q.push(startGene);
        unordered_set<string> visited;
        visited.emplace(startGene);
        while (!q.empty()) {
            const int size = q.size();
            for (int i = 0; i < size; i++) {
                string s = q.front();
                q.pop();
                for (int i = 0; i < s.length(); ++i) {
                    for (int j = 0; j < 4; ++j) {
                        if (s[i] != c[j]) {
                            string next = s;
                            next[i] = c[j];
                            if (!visited.contains(next) && ranges::find(bank, next) != bank.end()) {
                                if (next == endGene) {
                                    return res + 1;
                                }
                                q.push(next);
                                visited.emplace(next);
                            }
                        }
                    }
                }
            }
            res++;
        }
        return -1;
    }

    bool canReach(vector<int> &arr, int start) {
        if (arr[start] == 0) {
            return true;
        }
        queue<int> q;
        q.push(start);
        vector<bool> visited(arr.size(), false);
        visited[start] = true;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                int x = q.front();
                q.pop();
                int next = arr[x] + x;
                int prev = x - arr[x];
                if (next < arr.size() && !visited[next]) {
                    if (arr[next] == 0) {
                        return true;
                    }
                    q.push(next);
                    visited[next] = true;
                }
                if (prev >= 0 && !visited[prev]) {
                    if (arr[prev] == 0) {
                        return true;
                    }
                    q.push(prev);
                    visited[prev] = true;
                }
            }
        }
        return false;
    }

    vector<vector<int> > updateMatrix(vector<vector<int> > &mat) {
        int n = mat.size();
        int m = mat[0].size();
        vector<vector<int> > dirs(n, vector<int>(m, -1));
        vector<vector<int> > dist(n, vector<int>(m, -1));
        queue<pair<int, int> > q;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                    q.emplace(i, j);
                }
            }
        }
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            for (auto [dx, dy]: dirs) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && dist[nx][ny] == -1) {
                    dist[nx][ny] = dist[x][y] + 1;
                    q.emplace(nx, ny);
                }
            }
        }
        return dist;
    }

    struct TreeNode {
        int val;
        TreeNode *left;
        TreeNode *right;

        TreeNode(int x) : val(x), left(NULL), right(NULL) {
        }
    };

    vector<int> distanceK(TreeNode *root, TreeNode *target, int k) {
        if (!root || !target) return {};
        unordered_map<int, int> nodeToIndex;
        vector<TreeNode *> nodes;
        function<void(TreeNode *)> dfs = [&](TreeNode *node) {
            if (!node) {
                return;
            }
            nodes.push_back(node);
            dfs(node->left);
            dfs(node->right);
        };
        dfs(root);
        int n = nodes.size();

        for (int i = 0; i < n; ++i) {
            nodeToIndex[nodes[i]->val] = i;
        }

        vector<vector<int> > adj(n);
        for (const auto &node: nodes) {
            int u = nodeToIndex[node->val];
            if (node->left) {
                int v = nodeToIndex[node->left->val];
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            if (node->right) {
                int v = nodeToIndex[node->right->val];
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
        vector<bool> visited(n);
        queue<int> q;
        q.push(nodeToIndex[target->val]);
        visited[nodeToIndex[target->val]] = true;
        int cur = 0;
        while (!q.empty() && cur < k) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                int u = q.front();
                q.pop();
                for (const auto &edge: adj[u]) {
                    if (!visited[edge]) {
                        visited[edge] = true;
                        q.push(edge);
                    }
                }
            }
            cur++;
        }
        vector<int> res;
        while (!q.empty()) {
            res.push_back(nodes[q.front()]->val);
            q.pop();
        }
        return res;
    }
};
