#ifndef CIRC_LIST_H
#define CIRC_LIST_H

#include <iostream>

// 2.4 循环链表：带附加头结点

template <class T>
struct CircLinkNode {
    T data{};
    CircLinkNode<T>* link;

    explicit CircLinkNode(CircLinkNode<T>* next = nullptr) : link(next) {}
    CircLinkNode(const T& item, CircLinkNode<T>* next = nullptr)
        : data(item), link(next) {}
};

template <class T>
class CircList {
public:
    CircList();
    explicit CircList(const T& headValue);
    CircList(const CircList<T>& other);
    ~CircList();

    int Length() const;
    bool IsEmpty() const { return first->link == first; }

    CircLinkNode<T>* getHead() const { return first; }
    CircLinkNode<T>* getNext(CircLinkNode<T>* p) const { return p->link; }
    bool IsHead(CircLinkNode<T>* p) const { return p == first; }

    // 搜索失败时返回附加头结点 first（与 PPT 的循环扫描写法一致）。
    CircLinkNode<T>* Search(const T& x) const;
    // i = 0 返回头结点；i >= 1 表示第 i 个数据结点；越界返回 nullptr。
    CircLinkNode<T>* Locate(int i) const;

    bool getData(int i, T& x) const;
    void setData(int i, const T& x);

    bool Insert(int i, const T& x);
    bool Remove(int i, T& x);

    void output(std::ostream& out = std::cout) const;

    // Josephus 示例需要：删除指定结点并保持 last 正确。
    bool eraseNode(CircLinkNode<T>* prev, CircLinkNode<T>* node, T& x);

private:
    CircLinkNode<T>* first;
    CircLinkNode<T>* last;
};

#ifndef CIRC_LIST_IMPLEMENTATION
#include "CircList.cpp"
#endif

#endif
