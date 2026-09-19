#include "linked_list/include/StaticLinkedList.h"

#include <ostream>

namespace {
int AllocateNode(SLinkList& list) {
    for (int i = 1; i < MaxSize; ++i) {
        if (list[i].next == FreeIndex)
            return i;
    }
    return NullIndex;
}

int LocatePosition(const SLinkList& list, int i) {
    if (i < 0)
        return NullIndex;

    int current = 0;
    for (int position = 0; position < i; ++position) {
        current = list[current].next;
        if (current == NullIndex)
            return NullIndex;
    }
    return current;
}
}  // namespace

void InitList(SLinkList& list) {
    list[0] = SLinkNode{};
    list[0].next = NullIndex;

    for (int i = 1; i < MaxSize; ++i) {
        list[i] = SLinkNode{};
        list[i].next = FreeIndex;
    }
}

bool IsEmpty(const SLinkList& list) {
    return list[0].next == NullIndex;
}

int Length(const SLinkList& list) {
    int length = 0;
    for (int current = list[0].next;
         current != NullIndex;
         current = list[current].next) {
        ++length;
    }
    return length;
}

bool GetElem(const SLinkList& list, int i, ElemType& value) {
    if (i <= 0)
        return false;

    const int current = LocatePosition(list, i);
    if (current == NullIndex)
        return false;

    value = list[current].data;
    return true;
}

int LocateElem(const SLinkList& list, ElemType value) {
    for (int current = list[0].next;
         current != NullIndex;
         current = list[current].next) {
        if (list[current].data == value)
            return current;
    }
    return NullIndex;
}

bool ListInsert(SLinkList& list, int i, ElemType value) {
    if (i <= 0)
        return false;

    const int previous = LocatePosition(list, i - 1);
    if (previous == NullIndex)
        return false;

    const int newNode = AllocateNode(list);
    if (newNode == NullIndex)
        return false;

    list[newNode].data = value;
    list[newNode].next = list[previous].next;
    list[previous].next = newNode;
    return true;
}

bool ListDelete(SLinkList& list, int i, ElemType& value) {
    if (i <= 0)
        return false;

    const int previous = LocatePosition(list, i - 1);
    if (previous == NullIndex)
        return false;

    const int current = list[previous].next;
    if (current == NullIndex)
        return false;

    list[previous].next = list[current].next;
    value = list[current].data;
    list[current].data = ElemType{};
    list[current].next = FreeIndex;
    return true;
}

void PrintList(const SLinkList& list, std::ostream& out) {
    int current = list[0].next;
    while (current != NullIndex) {
        out << list[current].data;
        current = list[current].next;
        if (current != NullIndex)
            out << ' ';
    }
}
