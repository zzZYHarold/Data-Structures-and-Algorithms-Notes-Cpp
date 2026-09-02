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

#ifndef DBL_LIST_IMPLEMENTATION
#include "DblList.cpp"
#endif

#endif
