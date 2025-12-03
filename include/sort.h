//
// Created by seer on 2025/5/8.
//
#pragma once
#include <common.h>
#include <filesystem>
#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <random>

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
};
