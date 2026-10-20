#pragma once

#include "linear_list/include/SeqList.h"

#include <stdexcept>

// 6.3 有序顺序表（2026 秋正式课件第 63~75 页）。E 的 ==、<、> 与 K 比较关键码。
// 搜索返回 0-based 下标，失败返回 -1；BinarySearch 区间同样为 0-based。
template <class E, class K>
class SortedList : public SeqList<E> {
    // 通过本类接口维护有序性，隐藏基类任意位置改写。
    using SeqList<E>::setData;

public:
    explicit SortedList(int sz = 50) : SeqList<E>(sz) {}
    // SeqList 尚无安全的复制赋值；禁止默认浅拷贝。
    SortedList(const SortedList&) = delete;
    SortedList& operator=(const SortedList&) = delete;

    int Search(K targetKey) const {
        for (int index = 0; index <= this->last; ++index) {
            if (this->data[index] == targetKey) return index;
            if (this->data[index] > targetKey) break;
        }
        return -1;
    }

    int BinarySearch(K targetKey, int low, int high) const {
        if (low > high)
            return -1;
        if (low < 0 || high >= this->Length())
            throw std::out_of_range("BinarySearch uses 0-based indices");
        int middle = low + (high - low) / 2;
        if (this->data[middle] < targetKey)
            return BinarySearch(targetKey, middle + 1, high);
        if (this->data[middle] > targetKey)
            return BinarySearch(targetKey, low, middle - 1);
        return middle;
    }
    int BinarySearch(K targetKey) const {
        return BinarySearch(targetKey, 0, this->last);
    }

    // Insert / Remove 课件只有声明，按字典关键码唯一性补全。
    void Insert(const K targetKey, const E& entry) {
        if (!(entry == targetKey))
            throw std::invalid_argument("entry key differs from insertion key");
        if (Search(targetKey) != -1) throw std::invalid_argument("duplicate dictionary key");
        if (this->IsFull()) throw std::overflow_error("SortedList is full");
        int insertIndex = 0;
        while (insertIndex <= this->last && this->data[insertIndex] < targetKey)
            ++insertIndex;
        for (int index = this->last; index >= insertIndex; --index)
            this->data[index + 1] = this->data[index];
        this->data[insertIndex] = entry;
        ++this->last;
    }

    bool Remove(const K targetKey, E& removedEntry) {
        int index = Search(targetKey);
        return index != -1 && SeqList<E>::Remove(index + 1, removedEntry);
    }
    // 补充：仅修改属性，不改变关键码及排序位置。
    bool Update(const K targetKey, const E& entry) {
        if (!(entry == targetKey))
            throw std::invalid_argument("Update cannot change a key");
        int index = Search(targetKey);
        if (index == -1) return false;
        this->data[index] = entry;
        return true;
    }
};
