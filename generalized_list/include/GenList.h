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
        // 课件后续删除算法默认附加头结点 ref 初值为 1。
        first->info.ref = 1;
    }

    // 按课件思路：析构时通过 Remove() 依据引用计数释放广义表。
    ~GenList() {
        Remove(first);
        first = nullptr;
    }

    // 补充：为无共享、非递归广义表提供安全深拷贝。
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

    // 课件算法：复制一个无共享、非递归广义表。
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

    // 课件算法：计算非递归广义表深度。
    // 空表深度为 1；只含原子的线性表深度也为 1。
    int Depth() const {
        return depth(first);
    }

    // 补充一个面向对象的公开接口，内部直接使用课件的 delvalue 递归算法：
    // 删除整个广义表中所有值等于 x 的原子结点。
    void DeleteValue(const T& x) {
        delvalue(first, x);
    }

    // 以下两个接口属于补充：用于在课件未给出 CreateList() 完整实现的情况下
    // 方便构造和验证广义表；核心存储仍使用课件结点设计。
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

    // 课件递归复制算法：用于无共享、非递归广义表。
    GenListNode<T>* Copy(GenListNode<T>* ls) const {
        GenListNode<T>* q = nullptr;

        if (ls != nullptr) {
            q = new GenListNode<T>;
            q->utype = ls->utype;

            switch (ls->utype) {
            case 0:
                q->info.ref = ls->info.ref;
                break;
            case 1:
                q->info.value = ls->info.value;
                break;
            case 2:
                q->info.hlink = Copy(ls->info.hlink);
                break;
            default:
                delete q;
                throw std::logic_error("invalid generalized-list node type");
            }

            q->tlink = Copy(ls->tlink);
        }

        return q;
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

    // 课件递归求深度算法。
    int depth(GenListNode<T>* ls) const {
        if (ls == nullptr || ls->tlink == nullptr)
            return 1;

        GenListNode<T>* temp = ls->tlink;
        int maxSubDepth = 0;

        while (temp != nullptr) {
            if (temp->utype == 2) {
                const int subDepth = depth(temp->info.hlink);
                if (maxSubDepth < subDepth)
                    maxSubDepth = subDepth;
            }
            temp = temp->tlink;
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

    // 课件“原子结点删除”算法：
    // 1. 连续删除同层开头所有值为 x 的原子结点；
    // 2. 遇到子表时递归进入子表；
    // 3. 再把当前结点视作链表前驱，继续处理后续结点。
    void delvalue(GenListNode<T>* ls, const T& x) {
        if (ls == nullptr || ls->tlink == nullptr)
            return;

        GenListNode<T>* p = ls->tlink;

        while (p != nullptr && p->utype == 1 && p->info.value == x) {
            ls->tlink = p->tlink;
            delete p;
            p = ls->tlink;
        }

        if (p != nullptr) {
            if (p->utype == 2)
                delvalue(p->info.hlink, x);

            delvalue(p, x);
        }
    }

    // 课件“表的删除”算法：附加头结点的引用计数先减 1，
    // 只有 ref <= 0 时才真正释放该表；遇到子表则递归处理。
    void Remove(GenListNode<T>* ls) {
        if (ls == nullptr)
            return;

        --ls->info.ref;
        if (ls->info.ref <= 0) {
            GenListNode<T>* q = nullptr;

            while (ls->tlink != nullptr) {
                q = ls->tlink;

                if (q->utype == 2)
                    Remove(q->info.hlink);

                ls->tlink = q->tlink;
                delete q;
            }

            delete ls;
        }
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
