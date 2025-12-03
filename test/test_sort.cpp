//
// Created by seer on 2025/12/1.
//
#include <iostream>
#include <sort.h>
#include <Timer.h>
#include "gtest/gtest.h"

TEST(SortTest, BasicAssertions) {
    Timer timer;

    std::vector<int> vec = {5, 2, 9, 1, 5, 6};
    selectSort(vec.begin(), vec.end(), std::greater<int>());
    std::cout << timer.elapsed() << std::endl;
    std::cout << "Sorted result: ";
    for (int v: vec) std::cout << v << " ";
    std::cout << std::endl;
    timer.reset();
    bubbleSort(vec.begin(), vec.end(), std::less<int>());
    std::cout << timer.elapsed() << std::endl;
    std::cout << "Sorted result: ";
    for (int v: vec) std::cout << v << " ";
    std::cout << std::endl;
    timer.reset();
    std::ranges::sort(vec,std::less<int>());
    std::cout << timer.elapsed() << std::endl;
    std::cout << "Sorted result: ";
    for (int v: vec) std::cout << v << " ";
    std::cout << std::endl;

    // std::vector<int> expected = {1, 2, 5, 5, 6, 9};
    // EXPECT_EQ(vec, expected);
}
