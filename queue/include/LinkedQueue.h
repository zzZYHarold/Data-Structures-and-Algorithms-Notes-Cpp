#pragma once

#include "queue/include/Queue.h"

#include <ostream>

// 3.3 链式队列
// 队头在链头，队尾在链尾。

template <class T>
struct QueueNode {
    T data;
    QueueNode<T>* link;

    explicit QueueNode(QueueNode<T>* ptr = nullptr)
        : data(), link(ptr) {}

    QueueNode(const T& item, QueueNode<T>* ptr = nullptr)
        : data(item), link(ptr) {}
};

template <class T>
class LinkedQueue : public Queue<T> {
protected:
    QueueNode<T>* front;
    QueueNode<T>* rear;

public:
    LinkedQueue() : front(nullptr), rear(nullptr) {}

    ~LinkedQueue() override {
        makeEmpty();
    }

    LinkedQueue(const LinkedQueue&) = delete;
    LinkedQueue& operator=(const LinkedQueue&) = delete;

    bool EnQueue(const T& x) override {
        QueueNode<T>* node = new QueueNode<T>(x);

        if (front == nullptr) {
            front = rear = node;
        }
        else {
            rear->link = node;
            rear = node;
        }
        return true;
    }

    bool DeQueue(T& x) override {
        if (IsEmpty())
            return false;

        QueueNode<T>* p = front;
        x = front->data;
        front = front->link;
        delete p;

        // 课件原代码遗漏了这一点：删除最后一个结点后，rear 也必须恢复为空，否则会留下悬空指针。
        if (front == nullptr)
            rear = nullptr;

        return true;
    }

    bool getFront(T& x) const override {
        if (IsEmpty())
            return false;

        x = front->data;
        return true;
    }

    bool IsEmpty() const override {
        return front == nullptr;
    }

    // 链式队列没有固定数组容量。
    bool IsFull() const override {
        return false;
    }

    int getSize() const override {
        int count = 0;
        for (QueueNode<T>* p = front; p != nullptr; p = p->link)
            ++count;

        return count;
    }

    void makeEmpty() {
        while (front != nullptr) {
            QueueNode<T>* p = front;
            front = front->link;
            delete p;
        }
        rear = nullptr;
    }

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedQueue<U>& Q);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& Q) {
    os << "队列中元素个数=" << Q.getSize() << '\n';
    for (QueueNode<T>* p = Q.front; p != nullptr; p = p->link)
        os << p->data << ' ';

    return os;
}
