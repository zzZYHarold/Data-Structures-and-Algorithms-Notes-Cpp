#ifndef STATIC_LINKED_LIST_H
#define STATIC_LINKED_LIST_H

#include <iosfwd>

// 2.3.5 静态链表
// 结点保存在数组中，next 保存后继结点的数组下标而不是指针。
using ElemType = int;

constexpr int MaxSize = 10;
constexpr int NullIndex = -1;
constexpr int FreeIndex = -2;

struct SLinkNode {
    ElemType data{};
    int next = FreeIndex;
};

using SLinkList = SLinkNode[MaxSize];

// list[0] 是附加头结点；其余下标为 1..MaxSize-1 的结点保存数据。
void InitList(SLinkList& list);
bool IsEmpty(const SLinkList& list);
int Length(const SLinkList& list);

// i 使用 1-based 表项序号。
bool GetElem(const SLinkList& list, int i, ElemType& value);
// 找到时返回结点的数组下标，未找到返回 NullIndex。
int LocateElem(const SLinkList& list, ElemType value);

// 在第 i 个位置插入；合法范围为 1..Length()+1。
bool ListInsert(SLinkList& list, int i, ElemType value);
// 删除第 i 个元素，并通过 value 返回其值。
bool ListDelete(SLinkList& list, int i, ElemType& value);

void PrintList(const SLinkList& list, std::ostream& out);

#endif
