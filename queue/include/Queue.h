#pragma once

// 3.3 队列：抽象数据类型
// 课件第 87 页。

template <class T>
class Queue {
public:
    virtual ~Queue() = default;

    virtual bool EnQueue(const T& x) = 0;  // 队尾入队
    virtual bool DeQueue(T& x) = 0;        // 队头出队
    virtual bool getFront(T& x) const = 0; // 读取队头元素
    virtual bool IsEmpty() const = 0;
    virtual bool IsFull() const = 0;
    virtual int getSize() const = 0;
};
