#ifndef JOSEPHUS_H
#define JOSEPHUS_H

#include <iostream>
#include <stdexcept>

#include "linked_list/include/CircList.h"

// 2.4 约瑟夫问题：按 PPT 的“循环链表 + 跳过头结点”思路实现。
// 返回最后剩下的元素；每次淘汰值可输出到 out。
template <class T>
T Josephus(CircList<T>& js, int m, std::ostream& out = std::cout) {
    const int n = js.Length();
    if (n <= 0 || m <= 0)
        throw std::invalid_argument("Josephus requires n > 0 and m > 0");

    CircLinkNode<T>* pr = js.getHead();
    CircLinkNode<T>* p = js.getNext(pr);

    for (int i = 0; i < n - 1; ++i) {
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
