#pragma once

#include "queue/include/Deque.h"
#include "queue/include/SeqQueue.h"

// 3.5 数组双端队列
// 复用循环队列的数组、front、rear 和 maxSize。

template <class T>
class SeqDeque : public SeqQueue<T>, public Deque<T> {
public:
    explicit SeqDeque(int sz = 10)
        : SeqQueue<T>(sz) {}

    // 消除两个基类中 EnQueue / DeQueue 的歧义，并保持普通队列语义。
    bool EnQueue(const T& x) override {
        return SeqQueue<T>::EnQueue(x);
    }

    bool DeQueue(T& x) override {
        return SeqQueue<T>::DeQueue(x);
    }

    bool getHead(T& x) const override {
        return SeqQueue<T>::getFront(x);
    }

    bool getTail(T& x) const override {
        if (SeqQueue<T>::IsEmpty())
            return false;

        x = this->elements[(this->rear - 1 + this->maxSize) % this->maxSize];
        return true;
    }

    bool EnQueueHead(const T& x) override {
        if (SeqQueue<T>::IsFull())
            return false;

        this->front = (this->front - 1 + this->maxSize) % this->maxSize;
        this->elements[this->front] = x;
        return true;
    }

    bool EnQueueTail(const T& x) override {
        return SeqQueue<T>::EnQueue(x);
    }

    bool DeQueueHead(T& x) override {
        return SeqQueue<T>::DeQueue(x);
    }

    bool DeQueueTail(T& x) override {
        if (SeqQueue<T>::IsEmpty())
            return false;

        this->rear = (this->rear - 1 + this->maxSize) % this->maxSize;
        x = this->elements[this->rear];
        return true;
    }
};
