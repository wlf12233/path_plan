//
// Created by seer on 2025/3/26.
//

#include "AStar.h"
#include "Grid.h"
#include "DirectedGraph.h"


Node::Node(int _x, int _y, int _gcost, int _hcost, std::shared_ptr<Node> parent)
    : x(_x), y(_y), g_cost(_gcost), h_cost(_hcost), parent(std::move(parent)) {
}

AStar::AStar(int rows, int cols, const Heuristic heuristic) : rows(rows), cols(cols), heuristic(heuristic) {
}

std::optional<std::vector<std::pair<int, int> > > AStar::find_path_1(
    const std::vector<std::vector<int> > &grid,
    std::pair<int, int> start,
    std::pair<int, int> goal) {
    constexpr int dx[] = {-1, 1, 0, 0};
    constexpr int dy[] = {0, 0, 1, -1};

    // 访问标记矩阵
    std::vector visited(rows, std::vector(cols, false));

    // 使用智能指针管理节点
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, CompareNode> open_list;

    std::unordered_map<int, std::shared_ptr<Node> > all_nodes; // 记录所有创建的节点

    // 计算哈希键
    auto get_hash = [this](int x, int y) { return x * cols + y; };

    // 创建起点
    const auto start_node = std::make_shared<Node>(start.first, start.second, 0,
                                                   heuristic(start.first, start.second, goal.first, goal.second));
    open_list.push(start_node);

    // all_nodes[get_hash(start.first, start.second)] = start_node;

    while (!open_list.empty()) {
        auto cur_node = open_list.top();
        open_list.pop();

        if (visited[cur_node->x][cur_node->y]) continue;
        visited[cur_node->x][cur_node->y] = true;

        // 找到目标点
        if (cur_node->x == goal.first && cur_node->y == goal.second) {
            std::vector<std::pair<int, int> > path;
            while (cur_node) {
                path.emplace_back(cur_node->x, cur_node->y);
                cur_node = cur_node->parent;
            }
            std::ranges::reverse(path);
            return path;
        }

        // 遍历 4 个方向
        for (int i = 0; i < 4; i++) {
            int next_x = cur_node->x + dx[i];
            int next_y = cur_node->y + dy[i];


            if (next_x < 0 || next_y < 0 || next_x >= rows || next_y >= cols || visited[next_x][next_y] || grid[next_x][next_y] == 1)
                continue;

            auto hash = get_hash(next_x, next_y);
            if (all_nodes.contains(hash)) {
                continue;
            }

            int g_new_cost = cur_node->g_cost + 1;
            int h_new_cost = heuristic(next_x, next_y, goal.first, goal.second);


            auto neighbor = std::make_shared<Node>(next_x, next_y, g_new_cost, h_new_cost, cur_node);
            open_list.push(neighbor);
            all_nodes.emplace(hash, neighbor);
        }
    }

    return std::nullopt;
}

std::optional<std::vector<std::pair<int, int> > > AStar::find_path_2(const std::vector<std::vector<int> > &grid, std::pair<int, int> start,
                                                                     std::pair<int, int> goal) {
    constexpr int dx[] = {-1, 1, 0, 0};
    constexpr int dy[] = {0, 0, 1, -1};

    // 访问标记矩阵
    std::vector visited(rows, std::vector(cols, false));

    // 使用智能指针管理节点
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, CompareNode> open_list;

    // std::unordered_map<int, std::shared_ptr<Node> > all_nodes; // 记录所有创建的节点
    //
    // // 计算哈希键
    // auto get_hash = [this](int x, int y) { return x * cols + y; };

    // 创建起点
    const auto start_node = std::make_shared<Node>(start.first, start.second, 0,
                                                   heuristic(start.first, start.second, goal.first, goal.second));
    open_list.push(start_node);

    // all_nodes[get_hash(start.first, start.second)] = start_node;

    while (!open_list.empty()) {
        auto cur_node = open_list.top();
        open_list.pop();

        if (visited[cur_node->x][cur_node->y]) continue;
        visited[cur_node->x][cur_node->y] = true;

        // 找到目标点
        if (cur_node->x == goal.first && cur_node->y == goal.second) {
            std::vector<std::pair<int, int> > path;
            while (cur_node) {
                path.emplace_back(cur_node->x, cur_node->y);
                cur_node = cur_node->parent;
            }
            std::ranges::reverse(path);
            return path;
        }

        // 遍历 4 个方向
        for (int i = 0; i < 4; i++) {
            int next_x = cur_node->x + dx[i];
            int next_y = cur_node->y + dy[i];


            if (next_x < 0 || next_y < 0 || next_x >= rows || next_y >= cols || visited[next_x][next_y] || grid[next_x][next_y] == 1)
                continue;

            // auto hash = get_hash(next_x, next_y);
            // if (all_nodes.contains(hash)) {
            //     continue;
            // }

            int g_new_cost = cur_node->g_cost + 1;
            int h_new_cost = heuristic(next_x, next_y, goal.first, goal.second);


            auto neighbor = std::make_shared<Node>(next_x, next_y, g_new_cost, h_new_cost, cur_node);
            open_list.push(neighbor);
            // all_nodes.emplace(hash, neighbor);
        }
    }

    return std::nullopt;
}

std::vector<DirectedGraph::NodeId> find_path_3(
    const std::string &start, const std::string &goal) {
    auto heuristic = [&](const std::string &a, const std::string &b) {
        return std::abs(a[0] - goal[0]) + std::abs(a[1] - goal[1]);
    }; // 启发函数

    DirectedGraph graph = DirectedGraph();
    std::unordered_map<DirectedGraph::NodeId, DirectedGraph::NodeId> came_from; // 路径
    std::unordered_map<DirectedGraph::NodeId, double> g_score; // 从起点到某点的实际代价
    std::unordered_map<DirectedGraph::NodeId, double> f_score; // 估计值

    for (const auto &[node,_]: graph.getGraph()) {
        g_score[node] = std::numeric_limits<double>::infinity();
        f_score[node] = std::numeric_limits<double>::infinity();
    }
    g_score[start] = 0.0;
    f_score[start] = heuristic(start, goal);

    using PQElement = std::pair<double, DirectedGraph::NodeId>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<> > open_list;
    open_list.emplace(f_score[start], start);

    while (!open_list.empty()) {
        auto cur_node = open_list.top().second;
        open_list.pop();
        if (cur_node == goal) {
            return graph.reconstruct_path(came_from, goal);
        }
        for (const auto &[neighbor,weight]: graph.getNeighbors(cur_node)) {
            double tentative_g_score = g_score[cur_node] + weight;
            if (tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = cur_node;
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = g_score[cur_node] + heuristic(neighbor, goal);
                open_list.emplace(f_score[neighbor], neighbor);
            }
        }
    }
    return {};
}

std::optional<std::vector<std::pair<int, int> > > AStar::find_path_with_constrains(const int agent,
                                                                                   const std::vector<std::vector<int> > &map,
                                                                                   const std::pair<int, int> &start,
                                                                                   const std::pair<int, int> &goal,
                                                                                   const std::vector<Constraint> &constraints) {
    constexpr int dx[] = {-1, 1, 0, 0};
    constexpr int dy[] = {0, 0, 1, -1};

    // std::vector visited(rows, std::vector(cols, false));

    std::set<std::tuple<int, int, int> > visited;

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, CompareNode> open_list;
    const auto start_node = std::make_shared<Node>(start.first, start.second, 0,
                                                   heuristic(start.first, start.second, goal.first, goal.second));
    open_list.push(start_node);
    while (!open_list.empty()) {
        auto cur_node = open_list.top();
        open_list.pop();
        const int cur_time = cur_node->g_cost;
        if (visited.count({cur_node->x, cur_node->y, cur_node->g_cost})) {
            continue;
        }
        visited.insert({cur_node->x, cur_node->y, cur_node->g_cost});

        if (goal.first == cur_node->x && goal.second == cur_node->y) {
            std::vector<std::pair<int, int> > path;
            while (cur_node) {
                path.emplace_back(cur_node->x, cur_node->y);
                cur_node = cur_node->parent;
            }
            std::ranges::reverse(path);
            return path;
        }
        for (int i = 0; i < 4; i++) {
            int next_x = cur_node->x + dx[i];
            int next_y = cur_node->y + dy[i];
            int next_tim = cur_time + 1;
            if (next_x < 0 || next_y < 0 || next_x >= rows || next_y >= cols || map[next_x][next_y] == 1) {
                continue;
            }
            bool constraint_found = false;
            if (!constraints.empty()) {
                for (auto [robot, x, y, time]: constraints) {
                    if (agent == robot && next_x == x && next_y == y && next_tim == time) {
                        constraint_found = true;
                        break;
                    }
                }
            }
            if (constraint_found) {
                continue;
            }
            int g_new_cost = cur_node->g_cost + 1;
            auto neighbor = std::make_shared<Node>(next_x, next_y, g_new_cost, heuristic(next_x, next_y, goal.first, goal.second),
                                                   cur_node);
            open_list.push(neighbor);
        }
    }
    return std::nullopt;
}
