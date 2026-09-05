#pragma once

#include "Stack.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

// 3.1 顺序栈
// 对应课件第 11~17 页。
// 栈底固定在 elements[0]，top == -1 表示空栈。

template <class T>
class SeqStack : public Stack<T> {
private:
    T* elements;          // 存放栈元素的数组
    int top;              // 栈顶下标
    int maxSize;          // 当前容量
    int stackIncrement;   // 扩容时增加的容量

    void overflowProcess() {
        const int newSize = maxSize + stackIncrement;
        T* newArray = new T[newSize];

        for (int i = 0; i <= top; ++i)
            newArray[i] = elements[i];

        delete[] elements;
        elements = newArray;
        maxSize = newSize;
    }

public:
    explicit SeqStack(int sz = 50, int increment = 20)
        : elements(nullptr), top(-1), maxSize(sz), stackIncrement(increment) {
        if (sz <= 0 || increment <= 0)
            throw std::invalid_argument("stack size and increment must be positive");

        elements = new T[maxSize];
    }

    ~SeqStack() override {
        delete[] elements;
    }

    // 为避免原始指针导致浅拷贝、重复释放，这版中直接禁用拷贝。
    SeqStack(const SeqStack&) = delete;
    SeqStack& operator=(const SeqStack&) = delete;

    void Push(const T& x) override {
        if (IsFull())
            overflowProcess();

        elements[++top] = x;
    }

    bool Pop(T& x) override {
        if (IsEmpty())
            return false;

        x = elements[top--];
        return true;
    }

    bool getTop(T& x) const override {
        if (IsEmpty())
            return false;

        x = elements[top];
        return true;
    }

    bool IsEmpty() const override {
        return top == -1;
    }

    bool IsFull() const override {
        return top == maxSize - 1;
    }

    int getSize() const override {
        return top + 1;
    }

    int capacity() const {
        return maxSize;
    }

    void MakeEmpty() {
        top = -1;
    }

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const SeqStack<U>& s);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const SeqStack<T>& s) {
    os << "top=" << s.top << '\n';
    for (int i = 0; i <= s.top; ++i)
        os << s.elements[i] << ' ';

    return os;
}

