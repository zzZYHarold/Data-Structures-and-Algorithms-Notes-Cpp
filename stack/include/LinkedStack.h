#pragma once

#include "Stack.h"

#include <iostream>

// 3.1 链式栈
// 对应课件第 22~29 页。

template <class T>
struct LinkNode {
    T data;
    LinkNode<T>* link;

    explicit LinkNode(LinkNode<T>* ptr = nullptr)
        : data(), link(ptr) {}

    LinkNode(const T& item, LinkNode<T>* ptr = nullptr)
        : data(item), link(ptr) {}
};

template <class T>
class LinkedStack : public Stack<T> {
private:
    LinkNode<T>* top; // 栈顶指针，也就是链头指针

public:
    LinkedStack() : top(nullptr) {}

    ~LinkedStack() override {
        makeEmpty();
    }

    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;

    void Push(const T& x) override {
        // 新结点直接插到链头，即新的栈顶。
        top = new LinkNode<T>(x, top);
    }

    bool Pop(T& x) override {
        if (IsEmpty())
            return false;

        LinkNode<T>* p = top;
        top = top->link;
        x = p->data;
        delete p;
        return true;
    }

    bool getTop(T& x) const override {
        if (IsEmpty())
            return false;

        x = top->data;
        return true;
    }

    bool IsEmpty() const override {
        return top == nullptr;
    }

    // 链式栈没有固定容量；只要还能申请到内存，就不存在“栈满”。
    bool IsFull() const override {
        return false;
    }

    int getSize() const override {
        int count = 0;
        for (LinkNode<T>* p = top; p != nullptr; p = p->link)
            ++count;

        return count;
    }

    void makeEmpty() {
        while (top != nullptr) {
            LinkNode<T>* p = top;
            top = top->link;
            delete p;
        }
    }

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedStack<U>& s);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const LinkedStack<T>& s) {
    os << "栈中元素个数=" << s.getSize() << '\n';
    for (LinkNode<T>* p = s.top; p != nullptr; p = p->link)
        os << p->data << ' ';

    return os;
}
