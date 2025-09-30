//
// Created by seer on 2025/8/4.
//
#include <iostream>

class SharedCount {
public:
    SharedCount(): count{1} {
    }

    void add() {
        ++count;
    }

    void sub() {
        --count;
    }

    int get() {
        return count;
    }

private:
    std::atomic<int> count;
};

template<typename T>
class SharedPtr {
public:
    explicit SharedPtr(T *ptr): ptr(ptr), ref_count(new SharedCount()) {
    }

    SharedPtr(): ptr(nullptr), ref_count(new SharedCount()) {
    }

    SharedPtr(const SharedPtr &other) {
        this->ptr = other.ptr;
        this->ref_count = other.ref_count;
        ref_count->add();
    }

    SharedPtr &operator=(const SharedPtr &other) {
        if (this != &other) {
            clean();
            this->ptr = other.ptr;
            this->ref_count = other.ref_count;
            ref_count->add();
        }
        return *this;
    }

    SharedPtr(const SharedPtr &&other) noexcept {
        this->ptr = other.ptr;
        this->ref_count = other.ref_count;
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    SharedPtr &operator=(const SharedPtr &&other) {
        if (this != &other) {
            clean();
            this->ptr = other.ptr;
            this->ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    T *get() const {
        return ptr;
    }

    T *operator->() const {
        return ptr;
    }

    T *operator*() const {
        return *ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    ~SharedPtr() {
        clean();
    }

    void clean() {
        if (ref_count) {
            ref_count->sub();
            if (ref_count->get() == 0) {
                if (ptr) delete ptr;
                delete ref_count;
            }
        }
    }

private:
    T *ptr;
    SharedCount *ref_count;

    template<typename U>
    SharedPtr(const SharedPtr<U> &p, T *ptr) {
        this->ptr = ptr;
        this->ref_count = p.ref_count;
        ref_count->add();
    }
};

template<class T, class U>
std::shared_ptr<T> static_pointer_cast(const std::shared_ptr<U> &r) {
    T *ptr = static_cast<T *>(r.get());
    return SharedPtr<T>(r, ptr);
}
