#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstddef>
#include <iostream>

// 2.3 单链表：带附加头结点的单链表

template <class T>
struct LinkNode {
    T data{};
    LinkNode<T>* link;

    explicit LinkNode(LinkNode<T>* next = nullptr) : link(next) {}
    LinkNode(const T& item, LinkNode<T>* next = nullptr)
        : data(item), link(next) {}
};

template <class T>
class List {
public:
    List();
    explicit List(const T& x);
    List(const List<T>& other);
    ~List();

    void makeEmpty();
    int Length() const;

    LinkNode<T>* getHead() const { return first; }
    LinkNode<T>* Search(const T& x) const;
    LinkNode<T>* Locate(int i) const;

    bool getData(int i, T& x) const;
    void setData(int i, const T& x);

    // 在第 i 个结点之后插入 x；i = 0 表示插到首元结点之前。
    bool Insert(int i, const T& x);
    // 删除第 i 个数据结点；i 从 1 开始。
    bool Remove(int i, T& x);

    bool IsEmpty() const { return first->link == nullptr; }
    bool IsFull() const { return false; }

    // PPT 的前插法 / 后插法建立单链表，遇到 endTag 停止。
    void inputFront(const T& endTag, std::istream& in = std::cin);
    void inputRear(const T& endTag, std::istream& in = std::cin);

    void output(std::ostream& out = std::cout) const;

    List<T>& operator=(const List<T>& other);

    // 课件末尾练习“单链表的逆置”：保留附加头结点，只反转数据结点。
    void reverse();

private:
    LinkNode<T>* first;
};

#ifndef LINKED_LIST_IMPLEMENTATION
#include "LinkedList.cpp"
#endif

#endif
