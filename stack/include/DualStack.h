#pragma once

#include <stdexcept>

// 3.1 双栈共享一个数组空间
// 对应课件第 18~21 页。
// 栈 0 从左向右增长，栈 1 从右向左增长。

template <class T>
class DualStack {
private:
    T* elements;
    int top[2];       // top[0]：左栈栈顶；top[1]：右栈栈顶
    int bottom[2];    // bottom[0]：左栈栈底边界；bottom[1]：右栈栈底边界
    int maxSize;

    static void checkStackId(int d) {
        if (d != 0 && d != 1)
            throw std::invalid_argument("stack id must be 0 or 1");
    }

public:
    explicit DualStack(int sz = 50)
        : elements(nullptr), top{-1, sz}, bottom{-1, sz}, maxSize(sz) {
        if (sz <= 0)
            throw std::invalid_argument("stack size must be positive");
        elements = new T[maxSize];
    }

    ~DualStack() {
        delete[] elements;
    }

    DualStack(const DualStack&) = delete;
    DualStack& operator=(const DualStack&) = delete;

    bool Push(const T& x, const int d) {
        checkStackId(d);

        // 两个栈顶相邻时，整个共享空间已满。
        if (top[0] + 1 == top[1])
            return false;

        if (d == 0)
            ++top[0];
        else
            --top[1];

        elements[top[d]] = x;
        return true;
    }

    bool Pop(T& x, const int d) {
        checkStackId(d);

        if (top[d] == bottom[d])
            return false;

        x = elements[top[d]];
        if (d == 0)
            --top[0];
        else
            ++top[1];

        return true;
    }

    bool getTop(T& x, const int d) const {
        checkStackId(d);
        if (top[d] == bottom[d])
            return false;

        x = elements[top[d]];
        return true;
    }

    bool IsEmpty(const int d) const {
        checkStackId(d);
        return top[d] == bottom[d];
    }

    bool IsFull() const {
        return top[0] + 1 == top[1];
    }

    int getSize(const int d) const {
        checkStackId(d);
        return d == 0 ? top[0] + 1 : maxSize - top[1];
    }
};
