#ifndef CHAPTER4_GEN_LIST_H
#define CHAPTER4_GEN_LIST_H

#include <stdexcept>
#include <utility>

namespace chapter4 {

// 4.5 广义表结点。
// utype = 0：附加头结点，info.ref 保存引用计数
// utype = 1：原子结点，info.value 保存原子值
// utype = 2：子表结点，info.hlink 指向子表的附加头结点
template <class T>
struct GenListNode {
    int utype;
    union {
        int ref;
        T value;
        GenListNode<T>* hlink;
    } info;
    GenListNode<T>* tlink;  // 同层下一个结点

    GenListNode() : utype(0), tlink(nullptr) {
        info.ref = 0;
    }

    GenListNode(const GenListNode<T>& r)
        : utype(r.utype), tlink(r.tlink) {
        info = r.info;
    }
};

// Head() 返回表头时使用的结构，保持课件设计。
template <class T>
struct Items {
    int utype;
    union {
        int ref;
        T value;
        GenListNode<T>* hlink;
    } info;

    Items() : utype(0) {
        info.ref = 0;
    }

    Items(const Items<T>& r) : utype(r.utype) {
        info = r.info;
    }
};

template <class T>
class GenList {
public:
    GenList() : first(new GenListNode<T>) {
        first->info.ref = 1;
    }

    // 补充：课件声明了析构函数，但未给出实现。
    ~GenList() {
        Remove(first);
        first = nullptr;
    }

    // 补充：为动态链式结构提供安全深拷贝。
    GenList(const GenList<T>& r) : first(Copy(r.first)) {}

    GenList<T>& operator=(const GenList<T>& r) {
        if (this == &r)
            return *this;
        GenList<T> temp(r);
        std::swap(first, temp.first);
        return *this;
    }

    // 若广义表非空，通过 x 返回第一个表元素。
    bool Head(Items<T>& x) const {
        if (first->tlink == nullptr)
            return false;

        x.utype = first->tlink->utype;
        if (x.utype == 0)
            x.info.ref = first->tlink->info.ref;
        else if (x.utype == 1)
            x.info.value = first->tlink->info.value;
        else if (x.utype == 2)
            x.info.hlink = first->tlink->info.hlink;
        else
            throw std::logic_error("invalid generalized-list node type");
        return true;
    }

    // 若广义表非空，通过 lt 返回除表头元素以外的表尾。
    bool Tail(GenList<T>& lt) const {
        if (first->tlink == nullptr)
            return false;

        GenList<T> temp;
        temp.first->tlink = Copy(first->tlink->tlink);
        lt = temp;
        return true;
    }

    GenListNode<T>* First() const {
        return first->tlink;
    }

    GenListNode<T>* Next(GenListNode<T>* x) const {
        return x == nullptr ? nullptr : x->tlink;
    }

    // 复制一个无共享、非递归广义表。
    void Copy(const GenList<T>& r) {
        if (this == &r)
            return;

        GenListNode<T>* newFirst = Copy(r.first);
        Remove(first);
        first = newFirst;
    }

    int Length() const {
        return Length(first);
    }

    // 按课件定义：空表和只含原子的线性表深度均为 1。
    // 本实现针对非递归表；递归表的深度为无穷大，不在此函数中处理。
    int Depth() const {
        return depth(first);
    }

    // 以下两个接口属于补充，只用于在没有 CreateList() 完整实现的情况下
    // 方便构造和验证课件中的广义表结构；核心存储仍使用课件结点设计。
    void AppendAtom(const T& value) {
        GenListNode<T>* node = new GenListNode<T>;
        node->utype = 1;
        node->info.value = value;
        appendNode(node);
    }

    void AppendSublist(const GenList<T>& sublist) {
        GenListNode<T>* node = new GenListNode<T>;
        node->utype = 2;
        node->info.hlink = Copy(sublist.first);
        appendNode(node);
    }

private:
    GenListNode<T>* first;  // 广义表附加头结点

    // 深拷贝从 ls 开始的同层链，并递归复制其子表。
    // 与课件注释一致：用于无共享、非递归广义表。
    GenListNode<T>* Copy(GenListNode<T>* ls) const {
        if (ls == nullptr)
            return nullptr;

        GenListNode<T>* node = new GenListNode<T>;
        node->utype = ls->utype;

        if (ls->utype == 0) {
            node->info.ref = ls->info.ref;
        } else if (ls->utype == 1) {
            node->info.value = ls->info.value;
        } else if (ls->utype == 2) {
            node->info.hlink = Copy(ls->info.hlink);
        } else {
            delete node;
            throw std::logic_error("invalid generalized-list node type");
        }

        node->tlink = Copy(ls->tlink);
        return node;
    }

    int Length(GenListNode<T>* ls) const {
        if (ls == nullptr)
            return 0;

        GenListNode<T>* p = (ls->utype == 0) ? ls->tlink : ls;
        int length = 0;
        while (p != nullptr) {
            ++length;
            p = p->tlink;
        }
        return length;
    }

    int depth(GenListNode<T>* ls) const {
        if (ls == nullptr)
            return 1;

        GenListNode<T>* p = (ls->utype == 0) ? ls->tlink : ls;
        int maxSubDepth = 0;

        while (p != nullptr) {
            if (p->utype == 2) {
                const int subDepth = depth(p->info.hlink);
                if (subDepth > maxSubDepth)
                    maxSubDepth = subDepth;
            }
            p = p->tlink;
        }

        return maxSubDepth + 1;
    }

    bool equal(GenListNode<T>* s, GenListNode<T>* t) const {
        if (s == nullptr || t == nullptr)
            return s == t;
        if (s->utype != t->utype)
            return false;

        if (s->utype == 1 && !(s->info.value == t->info.value))
            return false;
        if (s->utype == 2 && !equal(s->info.hlink, t->info.hlink))
            return false;

        return equal(s->tlink, t->tlink);
    }

    // 释放一个无共享、非递归广义表/子表链。
    void Remove(GenListNode<T>* ls) {
        if (ls == nullptr)
            return;

        GenListNode<T>* next = ls->tlink;
        if (ls->utype == 2)
            Remove(ls->info.hlink);
        delete ls;
        Remove(next);
    }

    void appendNode(GenListNode<T>* node) {
        GenListNode<T>* p = first;
        while (p->tlink != nullptr)
            p = p->tlink;
        p->tlink = node;
    }
};

}  // namespace chapter4

#endif
