#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <variant>

#include "sorting/include/dataList.h"

namespace sorting {

// 第 113～117 页：记录放在 1..Length()，归并时只重连后继下标。
template <class Type, class Field = std::monostate>
class staticlinkList {
public:
    explicit staticlinkList(const dataList<Type, Field>& table)
        : Vector(std::make_unique<Node[]>(static_cast<std::size_t>(table.Length()) + 1)),
          length(table.Length()) {
        for (int index = 0; index < length; ++index)
            Vector[index + 1].element = table[index];
    }

    int Length() const { return length; }

    const Element<Type, Field>& operator[](int index) const {
        if (index <= 0 || index > length)
            throw std::out_of_range("static sorting list index out of range");
        return Vector[index].element;
    }

    int Link(int index) const {
        if (index < 0 || index > length)
            throw std::out_of_range("static sorting list link index out of range");
        return Vector[index].link;
    }

    // start1、start2 是互不相交的有序链首，0 表示空链。
    int ListMerge(int start1, int start2) {
        Link(start1);
        Link(start2);
        int tail = 0;
        int left = start1;
        int right = start2;
        while (left != 0 && right != 0) {
            int& winner = Vector[left].element <= Vector[right].element ? left : right;
            Vector[tail].link = winner;
            tail = winner;
            winner = Vector[winner].link;
        }
        Vector[tail].link = left != 0 ? left : right;
        return Vector[0].link;
    }

    int MergeSort(int left, int right) {
        if (left < 1 || right > length || right < left - 1)
            throw std::out_of_range("invalid static sorting list range");
        if (left > right) {
            Vector[0].link = 0;
            return 0;
        }
        if (left == right) {
            Vector[left].link = 0; // 重排已连过的链时，也从独立结点开始。
            Vector[0].link = left;
            return left;
        }

        const int middle = left + (right - left) / 2;
        const int leftHead = MergeSort(left, middle);
        const int rightHead = MergeSort(middle + 1, right);
        return ListMerge(leftHead, rightHead);
    }

    int MergeSort() { return MergeSort(1, length); }

private:
    struct Node {
        Element<Type, Field> element;
        int link = 0;
    };

    std::unique_ptr<Node[]> Vector;
    int length;
};

} // namespace sorting
