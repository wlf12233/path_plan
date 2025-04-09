//
// Created by seer on 2025/3/26.
//

#ifndef GRID_H
#define GRID_H
#include "common.h"


class Grid {
public:
    Grid(int width, int height);

    // 设置障碍物，1表示障碍物
    void setObstacle(int x, int y);

    // 移除障碍物
    void removeObstacle(int x, int y);

    // 检查某个位置是否是障碍物
    bool isObstacle(int x, int y) const;

    // 检查位置是否有效（在地图内且不是障碍物）
    bool isValid(int x, int y) const;

    // 显示网格地图
    void display() const;

    // 获取网格地图的宽度和高度
    int getWidth() const;
    int getHeight() const;

    std::vector<std::vector<int> > getGrid() const;

private:
    int width, height;
    std::vector<std::vector<int> > grid;
};


#endif //GRID_H
