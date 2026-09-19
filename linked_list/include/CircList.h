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

template <class T>
CircList<T>::CircList() {
    first = new CircLinkNode<T>();
    first->link = first;
    last = first;
}

template <class T>
CircList<T>::CircList(const T& headValue) {
    first = new CircLinkNode<T>(headValue);
    first->link = first;
    last = first;
}

template <class T>
CircList<T>::CircList(const CircList<T>& other) : CircList() {
    CircLinkNode<T>* tail = first;
    for (CircLinkNode<T>* p = other.first->link; p != other.first; p = p->link) {
        tail->link = new CircLinkNode<T>(p->data, first);
        tail = tail->link;
    }
    last = tail;
}

template <class T>
CircList<T>::~CircList() {
    CircLinkNode<T>* p = first->link;
    while (p != first) {
        CircLinkNode<T>* q = p;
        p = p->link;
        delete q;
    }
    delete first;
}

template <class T>
int CircList<T>::Length() const {
    int count = 0;
    for (CircLinkNode<T>* p = first->link; p != first; p = p->link)
        ++count;
    return count;
}

template <class T>
CircLinkNode<T>* CircList<T>::Search(const T& x) const {
    CircLinkNode<T>* current = first->link;
    while (current != first && current->data != x)
        current = current->link;
    return current;
}

template <class T>
CircLinkNode<T>* CircList<T>::Locate(int i) const {
    if (i < 0)
        return nullptr;
    if (i == 0)
        return first;

    CircLinkNode<T>* current = first->link;
    int k = 1;
    while (current != first && k < i) {
        current = current->link;
        ++k;
    }
    return (current == first) ? nullptr : current;
}

template <class T>
bool CircList<T>::getData(int i, T& x) const {
    CircLinkNode<T>* p = Locate(i);
    if (p == nullptr || p == first)
        return false;

    x = p->data;
    return true;
}

template <class T>
void CircList<T>::setData(int i, const T& x) {
    CircLinkNode<T>* p = Locate(i);
    if (p != nullptr && p != first)
        p->data = x;
}

template <class T>
bool CircList<T>::Insert(int i, const T& x) {
    CircLinkNode<T>* current = Locate(i);
    if (current == nullptr)
        return false;

    CircLinkNode<T>* newNode = new CircLinkNode<T>(x, current->link);
    current->link = newNode;

    if (last == current)
        last = newNode;
    return true;
}

template <class T>
bool CircList<T>::Remove(int i, T& x) {
    if (i <= 0)
        return false;

    CircLinkNode<T>* prev = Locate(i - 1);
    if (prev == nullptr)
        return false;

    CircLinkNode<T>* node = prev->link;
    if (node == first)
        return false;
    return eraseNode(prev, node, x);
}

template <class T>
void CircList<T>::output(std::ostream& out) const {
    CircLinkNode<T>* p = first->link;
    while (p != first) {
        out << p->data;
        p = p->link;
        if (p != first)
            out << ' ';
    }
}

template <class T>
bool CircList<T>::eraseNode(CircLinkNode<T>* prev, CircLinkNode<T>* node, T& x) {
    if (prev == nullptr || node == nullptr || node == first || prev->link != node)
        return false;

    prev->link = node->link;
    if (node == last)
        last = (prev == first && prev->link == first) ? first : prev;

    x = node->data;
    delete node;
    return true;
}

#endif
