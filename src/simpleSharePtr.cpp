//
// Created by seer on 2026/8/12.
//
#pragma once
#include <atomic>
#include <cstdint>
#include <iostream>
#include <utility>

class ShareCount {
public:
    ShareCount() : ref_count(1) {
    }

    ShareCount(const ShareCount &other) = delete;

    ShareCount &operator=(const ShareCount &other) = delete;

    void addRef() {
        ref_count.fetch_add(1, std::memory_order_relaxed);
    }

    bool release() {
        return ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1;
    }

    uint32_t get() const {
        return ref_count.load(std::memory_order_relaxed);
    }

private:
    std::atomic_uint32_t ref_count;
};

template<typename T>
class SimpleSharePtr {
public:
    SimpleSharePtr() noexcept : count(nullptr), ptr(nullptr) {
    }

    explicit SimpleSharePtr(T *t) : count(nullptr), ptr(t) {
        try {
            count = new ShareCount();
        } catch (...) {
            delete t;
            throw;
        }
    }

    SimpleSharePtr(SimpleSharePtr &&other) noexcept : count(other.count), ptr(other.ptr) {
        other.count = nullptr;
        other.ptr = nullptr;
    }

    SimpleSharePtr(const SimpleSharePtr &other) : count(other.count), ptr(other.ptr) {
        if (count) count->addRef();
    }

    SimpleSharePtr &operator=(SimpleSharePtr other) {
        swap(other);
        return *this;
    }
    SimpleSharePtr &operator=(SimpleSharePtr &other) noexcept {
        if (&other == this) return *this;
        if (other.count) other.count->addRef();
        if (count && count->release()) {
            delete count;
            delete ptr;
        }
        count = other.count;
        ptr = other.ptr;
        return *this;
    }

    ~SimpleSharePtr() {
       if (count && count->release()) {
           delete count;
           delete ptr;
       }
    };
    void swap(SimpleSharePtr &other) noexcept {
        std::swap(count, other.count);
        std::swap(ptr, other.ptr);
    }
    T *get() const {
        return ptr;
    }
    T &operator*() const {
        return *ptr;
    }
    T *operator->() const {
        return ptr;
    }


private:
    ShareCount *count;
    T *ptr;
};
