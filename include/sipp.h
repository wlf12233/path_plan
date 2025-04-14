
#ifndef SIPP_H
#define SIPP_H

#include "common.h"

using namespace std;

struct SafeInterval {
    double start, end; // 安全时间区间：[start, end]
};

struct State {
    int x, y; // 位置坐标
    double t; // 当前到达时间（在 safe interval 内）
    int intervalIdx; // 当前所在 safe interval 的索引
    double g, h; // 从起点到当前状态的实际代价（时间） 启发式估计，从当前到目标的时间

    bool operator > (const State &s) const {
        return (g + h) > (s.g + s.h);
    }
};

class SIPP {
public:
    SIPP(int width, int height);

    void addSafeInterval(int x, int y, double start, double end);

    std::vector<tuple<int,int,double>> findPath(std::pair<int, int> start, std::pair<int, int> goal);

private:
    int width, height;
    vector<vector<vector<SafeInterval> > > grid;
    vector<pair<int, int> > moves;

    bool isValid(int x, int y);

    int hashState(int x, int y, int idx);

    double heuristic(pair<int, int> a, pair<int, int> b);

    vector<pair<int, int> > reconstructPath(State goal);
};


#endif //SIPP_H
