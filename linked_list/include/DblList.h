#ifndef DBL_LIST_H
#define DBL_LIST_H

#include <iostream>

// 2.4 双向循环链表：带附加头结点

template <class T>
struct DblNode {
    T data{};
    DblNode<T>* lLink;
    DblNode<T>* rLink;

    DblNode(DblNode<T>* left = nullptr, DblNode<T>* right = nullptr)
        : lLink(left), rLink(right) {}

    DblNode(const T& value, DblNode<T>* left = nullptr, DblNode<T>* right = nullptr)
        : data(value), lLink(left), rLink(right) {}
};

template <class T>
class DblList {
public:
    DblList();
    ~DblList();

    int Length() const;
    bool IsEmpty() const { return first->rLink == first; }

    DblNode<T>* getHead() const { return first; }
    DblNode<T>* getNext(DblNode<T>* p) const { return p->rLink; }
    DblNode<T>* getPrior(DblNode<T>* p) const { return p->lLink; }

    // d == 0：沿前驱方向；d != 0：沿后继方向。
    DblNode<T>* Search(const T& x, int d = 1) const;
    // i == 0 返回头结点；i >= 1 返回沿 d 方向的第 i 个数据结点。
    DblNode<T>* Locate(int i, int d = 1) const;

    // 在沿 d 方向找到的第 i 个结点之后插入 x。
    bool Insert(int i, const T& x, int d = 1);
    // 删除沿 d 方向的第 i 个数据结点。
    bool Remove(int i, T& x, int d = 1);

    void output(std::ostream& out = std::cout, int d = 1) const;

private:
    DblNode<T>* first;
};

template <class T>
DblList<T>::DblList() {
    first = new DblNode<T>();
    first->lLink = first;
    first->rLink = first;
}

template <class T>
DblList<T>::~DblList() {
    DblNode<T>* p = first->rLink;
    while (p != first) {
        DblNode<T>* q = p;
        p = p->rLink;
        delete q;
    }
    delete first;
}

template <class T>
int DblList<T>::Length() const {
    int count = 0;
    for (DblNode<T>* p = first->rLink; p != first; p = p->rLink)
        ++count;
    return count;
}

template <class T>
DblNode<T>* DblList<T>::Search(const T& x, int d) const {
    DblNode<T>* current = (d == 0) ? first->lLink : first->rLink;

    while (current != first && current->data != x)
        current = (d == 0) ? current->lLink : current->rLink;

    return (current != first) ? current : nullptr;
}

template <class T>
DblNode<T>* DblList<T>::Locate(int i, int d) const {
    if (i < 0)
        return nullptr;
    if (i == 0)
        return first;

    DblNode<T>* current = (d == 0) ? first->lLink : first->rLink;
    int k = 1;
    while (current != first && k < i) {
        current = (d == 0) ? current->lLink : current->rLink;
        ++k;
    }

    return (current == first) ? nullptr : current;
}

template <class T>
bool DblList<T>::Insert(int i, const T& x, int d) {
    DblNode<T>* p = Locate(i, d);
    if (p == nullptr)
        return false;

    DblNode<T>* s = new DblNode<T>(x);

    if (d == 0) {
        s->lLink = p->lLink;
        p->lLink = s;
        s->lLink->rLink = s;
        s->rLink = p;
    } else {
        s->rLink = p->rLink;
        p->rLink = s;
        s->rLink->lLink = s;
        s->lLink = p;
    }

    return true;
}

template <class T>
bool DblList<T>::Remove(int i, T& x, int d) {
    if (i <= 0)
        return false;

    DblNode<T>* p = Locate(i, d);
    if (p == nullptr || p == first)
        return false;

    p->rLink->lLink = p->lLink;
    p->lLink->rLink = p->rLink;

    x = p->data;
    delete p;
    return true;
}

template <class T>
void DblList<T>::output(std::ostream& out, int d) const {
    DblNode<T>* p = (d == 0) ? first->lLink : first->rLink;
    while (p != first) {
        out << p->data;
        p = (d == 0) ? p->lLink : p->rLink;
        if (p != first)
            out << ' ';
    }
}

#endif
