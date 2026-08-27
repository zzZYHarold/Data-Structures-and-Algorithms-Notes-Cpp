#ifndef SET_OPERATIONS_H
#define SET_OPERATIONS_H

#include "SeqList.h"

// PPT 第 29~30 页：用顺序表实现集合的“并”和“交”。
// PPT 写了 template<class T>，但 T 并未出现在形参类型中，无法被自动推导。
// 这里按其实际使用的 SeqList<int> 去掉无效模板参数。

void Union(SeqList<int>& LA, SeqList<int>& LB);
void Intersection(SeqList<int>& LA, SeqList<int>& LB);

#endif
