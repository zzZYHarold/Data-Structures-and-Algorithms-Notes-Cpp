#pragma once

#include <algorithm>
#include <stdexcept>

// 5.6 堆
// 依据 2026 秋完整课件第 157-177 页整理。

constexpr int DefaultHeapSize = 20;

// 课件第 160 页：T 是关键码类型，E 是附加数据类型。
// 比较仅使用 key；搬移元素时 key 和 other 一起搬移。
template <class T, class E>
struct Element {
    T key{};
    E other{};

    Element() = default;
    Element(const T& priority, const E& payload = E()) : key(priority), other(payload) {}

    Element(const Element&) = default;
    Element& operator=(const Element&) = default;

    bool operator<=(const Element& otherElement) const {
        return key <= otherElement.key;
    }

    bool operator>(const Element& otherElement) const {
        return key > otherElement.key;
    }
};

template <class T, class E>
class MinHeap {
public:
    explicit MinHeap(int capacity = DefaultHeapSize) {
        if (capacity < 0)
            throw std::invalid_argument("heap capacity must be nonnegative");

        maxHeapSize = std::max(DefaultHeapSize, capacity);
        heap = new Element<T, E>[maxHeapSize];
        currentSize = 0;
    }

    MinHeap(const Element<T, E> elements[], int elementCount) {
        if (elementCount < 0 || (elementCount > 0 && elements == nullptr))
            throw std::invalid_argument("invalid heap input array");

        maxHeapSize = std::max(DefaultHeapSize, elementCount);
        heap = new Element<T, E>[maxHeapSize];

        for (int index = 0; index < elementCount; ++index)
            heap[index] = elements[index];

        currentSize = elementCount;

        // 从最后一个分支结点开始，自下向上筛选建堆。
        for (int currentPos = currentSize / 2 - 1;
             currentPos >= 0;
             --currentPos) {
            siftDown(currentPos, currentSize - 1);
        }
    }

    // 补充：拥有动态数组，禁止默认浅拷贝造成重复释放。
    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    ~MinHeap() {
        delete[] heap;
    }

    bool Insert(const Element<T, E>& element) {
        if (currentSize == maxHeapSize)
            return false;

        heap[currentSize] = element;
        siftUp(currentSize);
        ++currentSize;
        return true;
    }

    bool Remove(Element<T, E>& minimumElement) {
        if (currentSize == 0)
            return false;

        minimumElement = heap[0];
        heap[0] = heap[currentSize - 1];
        --currentSize;

        if (currentSize > 0)
            siftDown(0, currentSize - 1);

        return true;
    }

    bool IsEmpty() const {
        return currentSize == 0;
    }

    bool IsFull() const {
        return currentSize == maxHeapSize;
    }

    void MakeEmpty() {
        currentSize = 0;
    }

    int Size() const {
        return currentSize;
    }

    // Size/data 属于补充的只读检查接口。
    const Element<T, E>* data() const {
        return heap;
    }

private:
    Element<T, E>* heap;
    int currentSize;
    int maxHeapSize;

    void siftDown(int startIndex, int lastIndex) {
        int parentIndex = startIndex;
        int smallerChildIndex = 2 * parentIndex + 1;
        Element<T, E> savedElement = heap[parentIndex];

        while (smallerChildIndex <= lastIndex) {
            if (smallerChildIndex < lastIndex && heap[smallerChildIndex] > heap[smallerChildIndex + 1])
                ++smallerChildIndex;

            if (savedElement <= heap[smallerChildIndex])
                break;

            heap[parentIndex] = heap[smallerChildIndex];
            parentIndex = smallerChildIndex;
            smallerChildIndex = 2 * parentIndex + 1;
        }

        heap[parentIndex] = savedElement;
    }

    void siftUp(int startIndex) {
        int childIndex = startIndex;
        Element<T, E> savedElement = heap[childIndex];

        while (childIndex > 0) {
            int parentIndex = (childIndex - 1) / 2;
            if (heap[parentIndex] <= savedElement)
                break;

            heap[childIndex] = heap[parentIndex];
            childIndex = parentIndex;
        }

        heap[childIndex] = savedElement;
    }
};
