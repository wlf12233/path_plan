//
// Created by seer on 2025/6/4.
//
#include "Timer.h"
#include <iostream>
#include <string>
#include <utility> // for std::move
#include <vector>

int main() {
    std::vector<std::string> v;

    std::string str{"knock"};

    std::cout << "Copying str\n";
    Timer timer;

    v.push_back(str);
    auto s = timer.elapsed();
    std::cout << std::fixed << std::setprecision(6) << timer.elapsed() << "\n";

    std::cout << "str: " << str << '\n';
    std::cout << "vector: " << v[0] << '\n';

    std::cout << "\nMoving str\n";
    timer.reset();
    v.push_back(std::move(str)); // 调用右值版本的 push_back，它会将str移动到数组元素
    std::cout << std::fixed << std::setprecision(6) << timer.elapsed() << "\n";

    std::cout << "str: " << str << '\n'; // 打印中奖结果
    std::cout << "vector:" << v[0] << ' ' << v[1] << '\n';

    return 0;
}
