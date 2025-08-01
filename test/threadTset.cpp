//
// Created by seer on 2025/8/1.
//
#include "future"
#include "thread"
#include "iostream"

int main() {
    std::promise<int> p;
    auto f = p.get_future();
    std::thread t(
        [](std::promise<int> pr) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            pr.set_value(10);
        }, std::move(p));
    std::cout << f.get() << std::endl;
    if (t.joinable()) {
        t.join();
    }
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
    });
    auto f1 = task.get_future();
    std::thread t1(std::move(task), 1, 2);
    std::cout << f1.get() << std::endl;
    if (t1.joinable()) {
        t1.join();
    }
    auto f2 = std::async(std::launch::async, [](int a, int b) {
        return a + b;
    }, 1, 2);
    std::cout << f2.get() << std::endl;
}
