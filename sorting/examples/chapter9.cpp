#include "sorting/include/Sort.h"
#include "sorting/include/staticlinkList.h"

#include <initializer_list>
#include <iostream>
#include <random>
#include <stdexcept>

namespace {

using Table = sorting::dataList<int, char>;
using KeyTable = sorting::dataList<int>;
constexpr int exampleLength = 7;

void require(bool condition, const char* message) {
    if (!condition)
        throw std::runtime_error(message);
}

void checkSorted(const Table& original, const Table& sorted, bool stable) {
    require(sorted.Length() == original.Length(), "wrong result length");
    bool seen[exampleLength]{};
    for (int index = 0; index < sorted.Length(); ++index) {
        const auto& record = sorted[index];
        const int originalIndex = record.otherdata - 'A';
        require(originalIndex >= 0 && originalIndex < original.Length(),
                "wrong record identifier");
        require(!seen[originalIndex] && record.key == original[originalIndex].key,
                "a record was duplicated or its key was changed");
        seen[originalIndex] = true;
        if (index > 0) {
            const auto& previous = sorted[index - 1];
            require(previous <= record, "keys are out of order");
            if (stable && previous == record)
                require(previous.otherdata < record.otherdata, "equal keys changed order");
        }
    }
}

KeyTable makeTable(std::initializer_list<int> keys) {
    KeyTable table(static_cast<int>(keys.size()));
    for (int key : keys)
        require(table.Append({key}), "append failed");
    return table;
}

void checkKeys(const KeyTable& table, std::initializer_list<int> expected, const char* step) {
    require(table.Length() == static_cast<int>(expected.size()), step);
    int index = 0;
    for (int key : expected)
        require(table[index++].key == key, step);

    std::cout << step;
    for (int position = 0; position < table.Length(); ++position)
        std::cout << ' ' << table[position].key;
    std::cout << '\n';
}

void checkQuickSortExamples() {
    auto forward = makeTable({4, 5, 3, 1, 2});
    const int pivotPosition = forward.Partition(0, forward.Length() - 1);
    checkKeys(forward, {2, 3, 1, 4, 5}, "第 61 页前后指针法第一趟");
    forward.Partition(0, pivotPosition - 1);
    checkKeys(forward, {1, 2, 3, 4, 5}, "第 61 页前后指针法第二趟");

    auto classic = makeTable({6, 7, 1, 2, 3, 4, 5, 0});
    classic.Partition_Classic(0, classic.Length() - 1);
    checkKeys(classic, {0, 5, 1, 2, 3, 4, 6, 7}, "第 62 页挖坑法第一趟");
}

void checkHomework() {
    auto quick = makeTable({20, 1, 2, 9, 8, 10, 6, 4});
    const int pivotPosition = quick.Partition(0, quick.Length() - 1);
    checkKeys(quick, {4, 1, 2, 9, 8, 10, 6, 20}, "第 119 页第 1 题快速排序第一趟");
    quick.Partition(0, pivotPosition - 1);
    checkKeys(quick, {2, 1, 4, 9, 8, 10, 6, 20}, "第 119 页第 1 题快速排序第二趟");

    const auto input = makeTable({34, 85, 43, 72, 95, 40, 49, 58, 65, 20});
    auto heap = input;
    const int last = heap.Length() - 1;
    for (int parent = heap.Length() / 2 - 1; parent >= 0; --parent)
        sorting::detail::siftDown(heap, parent, last);
    checkKeys(heap, {95, 85, 49, 72, 34, 40, 43, 58, 65, 20}, "第 119 页初始最大堆");
    heap.Swap(heap[0], heap[last]);
    sorting::detail::siftDown(heap, 0, last - 1);
    checkKeys(heap, {85, 72, 49, 65, 34, 40, 43, 58, 20, 95}, "第 119 页堆排序排出一个最大值");

    auto merged = input;
    auto temporary = input;
    sorting::MergePass(merged, temporary, 1);
    checkKeys(temporary, {34, 85, 43, 72, 40, 95, 49, 58, 20, 65}, "第 119 页二路归并第一趟");
    sorting::MergePass(temporary, merged, 2);
    checkKeys(merged, {34, 43, 72, 85, 40, 49, 58, 95, 20, 65}, "第 119 页二路归并第二趟");

    auto shell = input;
    sorting::ShellPass(shell, 0, shell.Length() - 1, 3);
    checkKeys(shell, {20, 58, 40, 34, 85, 43, 49, 95, 65, 72}, "第 119 页希尔排序增量为 3 的一趟");
}

} // namespace

int main() {
    using namespace sorting;

    // 第 76 页胜者树输入，A～G 标记记录原序，25B 在 25D 之前。
    Table original(exampleLength);
    const int keys[exampleLength] = {21, 25, 49, 25, 16, 8, 63};
    for (int index = 0; index < exampleLength; ++index)
        require(original.Append({keys[index], static_cast<char>('A' + index)}), "append failed");

    using RangeSort = void (*)(Table&, int, int);
    const struct {
        RangeSort sort;
        bool stable;
    } rangeSorts[] = {
        {InsertSort, true}, {BinaryInsertSort, true}, {Shellsort, false},
        {BubbleSort, true}, {QuickSort, false}, {QuickSort_Classic, false},
        {SelectSort, false}
    };
    for (const auto& [sort, stable] : rangeSorts) {
        auto sorted = original;
        sort(sorted, 0, sorted.Length() - 1);
        checkSorted(original, sorted, stable);
    }
    auto randomized = original;
    std::mt19937 generator(2026);
    RandomizedQuickSort(randomized, 0, randomized.Length() - 1, generator);
    checkSorted(original, randomized, false);

    auto tournament = original;
    TournamentSort(tournament);
    checkSorted(original, tournament, true);
    auto heap = original;
    HeapSort(heap);
    checkSorted(original, heap, false);
    auto merged = original;
    MergeSort(merged);
    checkSorted(original, merged, true);

    staticlinkList linked(original);
    Table linkedResult(original.Length());
    for (int index = linked.MergeSort(); index != 0; index = linked.Link(index))
        require(linkedResult.Append(linked[index]), "a merged chain contains a cycle");
    checkSorted(original, linkedResult, true);

    checkQuickSortExamples();
    checkHomework();

    std::cout << "第九章示例通过。胜者树输出（排序码与原序标记）";
    for (int index = 0; index < tournament.Length(); ++index)
        std::cout << ' ' << tournament[index].key << tournament[index].otherdata;
    std::cout << '\n';
}
