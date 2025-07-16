//
// Created by seer on 2025/7/7.
//
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

// int main() {
//     using namespace std::chrono;
//     auto start = std::chrono::high_resolution_clock::now();
//     std::thread t1 = std::thread([&]() {
//         std::cout << std::this_thread::get_id() << std::endl;
//     });
//     auto end = std::chrono::high_resolution_clock::now();
//     std::this_thread::sleep_for(1000ms);
//     auto du = std::chrono::duration_cast<std::chrono::seconds>(end - start);
// }
