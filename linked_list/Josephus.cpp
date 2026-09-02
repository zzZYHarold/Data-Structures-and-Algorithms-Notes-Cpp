#ifndef JOSEPHUS_IMPLEMENTATION
#define JOSEPHUS_IMPLEMENTATION

#include "Josephus.h"
#include <stdexcept>

template <class T>
T Josephus(CircList<T>& js, int m, std::ostream& out) {
    const int n = js.Length();
    if (n <= 0 || m <= 0)
        throw std::invalid_argument("Josephus requires n > 0 and m > 0");

    CircLinkNode<T>* pr = js.getHead();
    CircLinkNode<T>* p = js.getNext(pr);

    for (int i = 0; i < n - 1; ++i) {
        // 从当前 p 开始把 p 计作 1，走到第 m 个数据结点。
        for (int j = 1; j < m; ++j) {
            pr = p;
            p = p->link;
            if (js.IsHead(p)) {
                pr = p;
                p = p->link;
            }
        }

        out << (i + 1) << ": " << p->data << '\n';

        T removed{};
        js.eraseNode(pr, p, removed);

        p = pr->link;
        if (js.IsHead(p)) {
            pr = p;
            p = p->link;
        }
    }

    return p->data;
}

#endif
