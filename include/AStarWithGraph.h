//
// Created by seer on 2025/6/3.
//

#ifndef ASTARWITHGRAPH_H
#define ASTARWITHGRAPH_H
#include <common.h>

class AStarWithGraph {
    using Heuristic = std::function<double(int, int)>;

public:
    bool aStar(int V, int start, int goal, std::vector<std::vector<std::pair<int, double> > > adj, std::vector<double> &out_list,
               std::vector<int> &out_pred, Heuristic heuristic);
};


#endif //ASTARWITHGRAPH_H
