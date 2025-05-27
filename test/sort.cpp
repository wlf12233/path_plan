//
// Created by seer on 2025/5/8.
//
#include <common.h>
#include <filesystem>
#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <random>

// class Timer {
// private:
//     using Clock = std::chrono::steady_clock;
//     using Second = std::chrono::duration<double>;
//
//     std::chrono::time_point<Clock> start{Clock::now()};
//
// public:
//     void reset() {
//         start = Clock::now();
//     }
//
//     double elapsed() const {
//         return std::chrono::duration_cast<Second>(Clock::now() - start).count();
//     }
// };

template<typename Iterator, typename Compare>
void selectSort(Iterator begin, Iterator end, Compare compare) {
    for (auto startElement{begin}; startElement != end; ++startElement) {
        auto smallest{startElement};
        for (auto currentElement = std::next(startElement); currentElement != end; ++currentElement) {
            if (compare(*currentElement, *smallest)) {
                smallest = currentElement;
            }
        }
        std::swap(*startElement, *smallest);
    }
}

template<typename Iterator, typename Compare>
void bubbleSort(Iterator begin, Iterator end, Compare compare) {
    for (auto startElement{end}; startElement != begin; --startElement) {
        bool sorted{false};
        for (auto currentElement = begin + 1; currentElement != startElement; ++currentElement) {
            if (compare(*currentElement, *(currentElement - 1))) {
                std::swap(*currentElement, *(currentElement - 1));
                sorted = true;
            }
        }
        if (!sorted) {
            break;
        }
    }
}

// int main() {
//     int arr[]{93, 41, 65, 62, 7, 8, 19, 10};
//     constexpr int length = std::size(arr);
//     Timer timer;
//     bubbleSort(std::begin(arr), std::end(arr), std::less<int>{});
//     std::cout << "bubbleSort " << std::fixed << std::setprecision(6) << timer.elapsed() << "\n";
//     std::ranges::shuffle(arr, std::mt19937{});
//     timer.reset();
//     selectSort(std::begin(arr), std::end(arr), std::less<int>{});
//     std::cout << "selectSort " << std::fixed << std::setprecision(6) << timer.elapsed() << "\n";
//     std::ranges::shuffle(arr, std::mt19937{});
//     timer.reset();
//     std::ranges::sort(arr, std::greater<int>{});
//     std::cout << std::fixed << std::setprecision(6) << timer.elapsed() << "\n";
//     // for (int arr1: arr) {
//     //     std::cout << arr1 << ' ';
//     // }
//     // std::cout << std::endl;
//     return 0;
// }
