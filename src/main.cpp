#include "common.h"
#include "SIPP.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
// int main() {
//     // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
//     // <b>lang</b> variable name to see how CLion can help you rename it.
//     Grid grid(10, 10);
//     AStar astar(10, 10, manhattan);
//
//     grid.print();
//
//     auto start_time = std::chrono::high_resolution_clock::now();
//
//     if (auto path = astar.find_path_1(grid.getGrid(), {0, 0}, {4, 4})) {
//         std::cout << "路径找到: ";
//         for (const auto &[x, y]: path.value()) {
//             std::cout << "(" << x << "," << y << ") -> ";
//         }
//         std::cout << "终点\n";
//     } else {
//         std::cout << "未找到路径\n";
//     }
//     auto end_time = std::chrono::high_resolution_clock::now();
//
//     std::chrono::duration<double> diff = end_time - start_time;
//     std::cout << diff.count() << " seconds\n";
//
//     return 0;
// }
int main() {
    SIPP planner(10, 10);
    planner.addSafeInterval(0, 0, 0, 100);
    planner.addSafeInterval(1, 0, 1, 100);
    planner.addSafeInterval(2, 0, 2, 100);
    planner.addSafeInterval(3, 0, 3, 100);
    planner.addSafeInterval(4, 0, 4, 100);
    planner.addSafeInterval(5, 0, 5, 100);

    auto path = planner.findPath({0, 0}, {5, 0});
    for (auto [x, y] : path) {
        std::cout << "(" << x << ", " << y << ") -> ";
    }
    std::cout << "Goal reached!" << std::endl;
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
