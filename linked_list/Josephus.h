#ifndef JOSEPHUS_H
#define JOSEPHUS_H

#include <iostream>
#include "CircList.h"

// 2.4 约瑟夫问题：按 PPT 的“循环链表 + 跳过头结点”思路实现。
// 返回最后剩下的元素；每次淘汰值可输出到 out。
template <class T>
T Josephus(CircList<T>& js, int m, std::ostream& out = std::cout);

#ifndef JOSEPHUS_IMPLEMENTATION
#include "Josephus.cpp"
#endif

#endif
