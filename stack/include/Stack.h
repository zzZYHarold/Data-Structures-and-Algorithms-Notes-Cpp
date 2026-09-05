#pragma once

// 3.1 栈：抽象数据类型
// 对应课件第 10 页。

template <class T>
class Stack {
public:
    virtual ~Stack() = default;

    virtual void Push(const T& x) = 0;   // 元素 x 进栈
    virtual bool Pop(T& x) = 0;          // 栈顶元素出栈，由 x 返回
    virtual bool getTop(T& x) const = 0; // 读取栈顶元素，由 x 返回
    virtual bool IsEmpty() const = 0;    // 判断栈空
    virtual bool IsFull() const = 0;     // 判断栈满
    virtual int getSize() const = 0;     // 栈中元素个数
};
