#pragma once

#include "queue/include/Queue.h"

#include <ostream>
#include <stdexcept>

// 3.3 顺序（循环）队列
// 使用一个空位置区分“队空”和“队满”：
//   队空：front == rear
//   队满：(rear + 1) % maxSize == front
// 因此数组长度为 maxSize 时，最多保存 maxSize - 1 个元素。

template <class T>
class SeqQueue : public Queue<T> {
protected:
    int rear;      // 指向队尾元素的下一个位置
    int front;     // 指向当前队头元素
    T* elements;   // 队列存放数组
    int maxSize;   // 数组长度

public:
    explicit SeqQueue(int sz = 10)
        : rear(0), front(0), elements(nullptr), maxSize(sz) {
        if (sz <= 1)
            throw std::invalid_argument("queue array size must be greater than 1");
        elements = new T[maxSize];
    }

    ~SeqQueue() override {
        delete[] elements;
    }

    SeqQueue(const SeqQueue&) = delete;
    SeqQueue& operator=(const SeqQueue&) = delete;

    bool EnQueue(const T& x) override {
        if (IsFull())
            return false;

        elements[rear] = x;
        rear = (rear + 1) % maxSize;
        return true;
    }

    bool DeQueue(T& x) override {
        if (IsEmpty())
            return false;

        x = elements[front];
        front = (front + 1) % maxSize;
        return true;
    }

    bool getFront(T& x) const override {
        if (IsEmpty())
            return false;

        x = elements[front];
        return true;
    }

    void makeEmpty() {
        front = rear = 0;
    }

    bool IsEmpty() const override {
        return front == rear;
    }

    bool IsFull() const override {
        return (rear + 1) % maxSize == front;
    }

    int getSize() const override {
        return (rear - front + maxSize) % maxSize;
    }

    // 实际可存元素个数。由于必须空出一个位置，所以是 maxSize - 1。
    int capacity() const {
        return maxSize - 1;
    }

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const SeqQueue<U>& Q);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const SeqQueue<T>& Q) {
    os << "front=" << Q.front << ", rear=" << Q.rear << '\n';
    for (int i = Q.front; i != Q.rear; i = (i + 1) % Q.maxSize)
        os << Q.elements[i] << ' ';

    return os;
}
