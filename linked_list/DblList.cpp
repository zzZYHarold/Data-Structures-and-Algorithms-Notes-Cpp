#ifndef DBL_LIST_IMPLEMENTATION
#define DBL_LIST_IMPLEMENTATION

#include "DblList.h"

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
        // 沿前驱方向：插到 p 的左侧。
        s->lLink = p->lLink;
        p->lLink = s;
        s->lLink->rLink = s;
        s->rLink = p;
    } else {
        // 沿后继方向：插到 p 的右侧。
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
        return false;  // 不能删除附加头结点

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
