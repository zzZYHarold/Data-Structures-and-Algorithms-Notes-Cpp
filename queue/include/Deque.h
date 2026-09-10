#pragma once

// 3.5 双端队列：抽象接口
// 两端都允许插入和删除。

template <class T>
class Deque {
public:
    virtual ~Deque() = default;

    virtual bool getHead(T& x) const = 0;
    virtual bool getTail(T& x) const = 0;

    // 普通“入队/出队”分别按队尾入、队头出解释。
    virtual bool EnQueue(const T& x) {
        return EnQueueTail(x);
    }

    virtual bool DeQueue(T& x) {
        return DeQueueHead(x);
    }

    virtual bool EnQueueHead(const T& x) = 0;
    virtual bool EnQueueTail(const T& x) = 0;
    virtual bool DeQueueHead(T& x) = 0;
    virtual bool DeQueueTail(T& x) = 0;
};
