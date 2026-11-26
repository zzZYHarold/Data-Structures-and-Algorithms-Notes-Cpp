#pragma once

#include <algorithm>
#include <bit>
#include <cstddef>
#include <memory>
#include <random>
#include <stdexcept>

#include "sorting/include/dataList.h"

namespace sorting {

// 9.2 插入排序
template <class T, class Field>
void InsertSort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    if (left >= right)
        return;

    for (int index = left + 1; index <= right; ++index) {
        if (table[index] < table[index - 1]) {
            const auto saved = table[index];
            int previous = index - 1;
            do {
                table[previous + 1] = table[previous];
                --previous;
            } while (previous >= left && saved < table[previous]);
            table[previous + 1] = saved;
        }
    }
}

template <class T, class Field>
void BinaryInsertSort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    if (left >= right)
        return;

    for (int index = left + 1; index <= right; ++index) {
        const auto saved = table[index];
        int low = left;
        int high = index - 1;
        while (low <= high) {
            const int middle = low + (high - low) / 2;
            if (saved < table[middle])
                high = middle - 1;
            else
                low = middle + 1; // 插在已有等键元素之后。
        }
        for (int previous = index - 1; previous >= low; --previous)
            table[previous + 1] = table[previous];
        table[low] = saved;
    }
}

// 按指定增量做一趟组内直接插入，供完整排序和第 119 页作业共用。
template <class T, class Field>
void ShellPass(dataList<T, Field>& table, int left, int right, int gap) {
    detail::CheckRange(table.Length(), left, right);
    if (gap <= 0)
        throw std::invalid_argument("Shell sort gap must be positive");
    if (gap > right - left)
        return;

    for (int index = left + gap; index <= right; ++index) {
        if (table[index] < table[index - gap]) {
            const auto saved = table[index];
            int previous = index - gap;
            do {
                table[previous + gap] = table[previous];
                previous -= gap;
            } while (previous >= left && saved < table[previous]);
            table[previous + gap] = saved;
        }
    }
}

template <class T, class Field>
void Shellsort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    for (int gap = (right - left + 1) / 2; gap > 0; gap /= 2)
        ShellPass(table, left, right, gap);
}

// 9.3 交换排序
template <class T, class Field>
void BubbleSort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    for (int pass = left; pass < right; ++pass) {
        bool exchanged = false;
        for (int index = right; index > pass; --index) {
            if (table[index - 1] > table[index]) {
                table.Swap(table[index - 1], table[index]);
                exchanged = true;
            }
        }
        if (!exchanged)
            break;
    }
}

namespace detail {

template <class Partition>
void QuickSortRange(int left, int right, const Partition& partition) {
    if (left < right) {
        const int pivotPosition = partition(left, right);
        QuickSortRange(left, pivotPosition - 1, partition);
        QuickSortRange(pivotPosition + 1, right, partition);
    }
}

} // namespace detail

template <class T, class Field>
void QuickSort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    detail::QuickSortRange(left, right, [&table](int low, int high) {
        return table.Partition(low, high);
    });
}

template <class T, class Field>
void QuickSort_Classic(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    detail::QuickSortRange(left, right, [&table](int low, int high) {
        return table.Partition_Classic(low, high);
    });
}

// 第 63 页步骤：每个子区间先随机选基准，再换到首位进行划分。
template <class T, class Field, class Generator>
void RandomizedQuickSort(dataList<T, Field>& table, int left, int right,
                         Generator& generator) {
    detail::CheckRange(table.Length(), left, right);
    detail::QuickSortRange(left, right, [&](int low, int high) {
        const int chosen = std::uniform_int_distribution<int>(low, high)(generator);
        if (chosen != low)
            table.Swap(table[low], table[chosen]);
        return table.Partition(low, high);
    });
}

// 9.4 选择排序
template <class T, class Field>
void SelectSort(dataList<T, Field>& table, int left, int right) {
    detail::CheckRange(table.Length(), left, right);
    for (int index = left; index < right; ++index) {
        int minimum = index;
        for (int candidate = index + 1; candidate <= right; ++candidate) {
            if (table[candidate] < table[minimum])
                minimum = candidate;
        }
        if (minimum != index)
            table.Swap(table[index], table[minimum]);
    }
}

// 第 75～85 页胜者树：结点保存胜者的原始下标，-1 表示已出赛。
template <class T, class Field>
void TournamentSort(dataList<T, Field>& table) {
    const int length = table.Length();
    if (length < 2)
        return;

    const auto leafCount = static_cast<std::size_t>(length);
    const auto treeSize = 2 * leafCount; // 下标 0 留空，共 2n-1 个有效结点。
    const auto firstLeaf = std::bit_ceil(leafCount);
    auto winners = std::make_unique<int[]>(treeSize);
    const auto leafFor = [=](int index) {
        const auto leaf = firstLeaf + static_cast<std::size_t>(index);
        // 从最深层最左叶子开始，绕回较浅层的叶子。
        return leaf < treeSize ? leaf : leaf - leafCount;
    };
    for (int index = 0; index < length; ++index)
        winners[leafFor(index)] = index;

    const auto play = [&](std::size_t parent) {
        const int leftWinner = winners[2 * parent];
        const int rightWinner = winners[2 * parent + 1];
        if (leftWinner == -1 || (rightWinner != -1 && table[rightWinner] < table[leftWinner]))
            winners[parent] = rightWinner;
        else
            winners[parent] = leftWinner; // 同键时左侧叶子在原序列中更靠前。
    };
    for (std::size_t parent = leafCount - 1; parent > 0; --parent)
        play(parent);

    dataList<T, Field> sorted(length);
    for (int index = 0; index < length; ++index) {
        const int winner = winners[1];
        sorted.Append(table[winner]);
        const auto leaf = leafFor(winner);
        winners[leaf] = -1;
        for (auto parent = leaf / 2; parent > 0; parent /= 2)
            play(parent);
    }
    for (int index = 0; index < length; ++index)
        table[index] = sorted[index];
}

namespace detail {

template <class T, class Field>
void siftDown(dataList<T, Field>& table, int start, int last) {
    int parent = start;
    const auto saved = table[parent];
    while (parent < (last + 1) / 2) {
        int largerChild = 2 * parent + 1;
        if (largerChild < last && table[largerChild] < table[largerChild + 1])
            ++largerChild;
        if (saved >= table[largerChild])
            break;
        table[parent] = table[largerChild];
        parent = largerChild;
    }
    table[parent] = saved;
}

} // namespace detail

template <class T, class Field>
void HeapSort(dataList<T, Field>& table) {
    const int length = table.Length();
    for (int parent = length / 2 - 1; parent >= 0; --parent)
        detail::siftDown(table, parent, length - 1);
    for (int last = length - 1; last > 0; --last) {
        table.Swap(table[0], table[last]);
        detail::siftDown(table, 0, last - 1);
    }
}

// 9.5 迭代二路归并排序
template <class T, class Field>
void merge(const dataList<T, Field>& source, dataList<T, Field>& destination,
           int left, int middle, int right) {
    detail::CheckRange(source.Length(), left, right);
    if (middle < left || middle > right || right >= destination.Length()
        || &source == &destination)
        throw std::invalid_argument("invalid merge inputs");

    int leftIndex = left;
    int rightIndex = middle + 1;
    int output = left;
    while (leftIndex <= middle && rightIndex <= right) {
        if (source[leftIndex] <= source[rightIndex])
            destination[output++] = source[leftIndex++];
        else
            destination[output++] = source[rightIndex++];
    }
    while (leftIndex <= middle)
        destination[output++] = source[leftIndex++];
    while (rightIndex <= right)
        destination[output++] = source[rightIndex++];
}

template <class T, class Field>
void MergePass(const dataList<T, Field>& source, dataList<T, Field>& destination, int len) {
    const int length = source.Length();
    if (len <= 0 || &source == &destination || destination.Length() != length)
        throw std::invalid_argument("invalid merge pass inputs");

    for (int left = 0; left < length;) {
        const int middle = left + std::min(len, length - left) - 1;
        const int right = middle + std::min(len, length - 1 - middle);
        merge(source, destination, left, middle, right);
        left = right + 1;
    }
}

template <class T, class Field>
void MergeSort(dataList<T, Field>& table) {
    const int length = table.Length();
    if (length < 2)
        return;

    dataList<T, Field> temporary(length);
    for (int index = 0; index < length; ++index)
        temporary.Append({});

    for (int len = 1; len < length;) {
        MergePass(table, temporary, len);
        len += std::min(len, length - len);
        if (len == length) {
            for (int index = 0; index < length; ++index)
                table[index] = temporary[index];
            break;
        }
        MergePass(temporary, table, len);
        len += std::min(len, length - len);
    }
}

} // namespace sorting
