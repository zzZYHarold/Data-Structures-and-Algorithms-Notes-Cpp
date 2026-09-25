#pragma once

#include <algorithm>
#include <iostream>

// 5.6 堆
// 依据 2025 秋课件整理；当前 2026 part1 尚未覆盖本节。

constexpr int DefaultHeapSize = 20;

template <class T, class E>
class MinPQ {
public:
    virtual ~MinPQ() = default;
    virtual bool Insert(const E& d) = 0;
    virtual bool Remove(E& d) = 0;
};

template <class T, class E>
class MinHeap : public MinPQ<T, E> {
public:
    explicit MinHeap(int sz = DefaultHeapSize) {
        maxHeapSize = std::max(DefaultHeapSize, sz);
        heap = new E[maxHeapSize];
        currentSize = 0;
    }

    MinHeap(const E arr[], int n) {
        maxHeapSize = std::max(DefaultHeapSize, n);
        heap = new E[maxHeapSize];

        for (int i = 0; i < n; ++i) {
            heap[i] = arr[i];
        }
        currentSize = n;

        // 从最后一个分支结点开始，自下向上筛选建堆。
        for (int currentPos = currentSize / 2 - 1;
             currentPos >= 0;
             --currentPos) {
            siftDown(currentPos, currentSize - 1);
        }
    }

    ~MinHeap() override {
        delete[] heap;
    }

    bool Insert(const E& x) override {
        if (currentSize == maxHeapSize) {
            return false;
        }

        heap[currentSize] = x;
        siftUp(currentSize);
        ++currentSize;
        return true;
    }

    bool Remove(E& x) override {
        if (currentSize == 0) {
            return false;
        }

        x = heap[0];
        heap[0] = heap[currentSize - 1];
        --currentSize;

        if (currentSize > 0) {
            siftDown(0, currentSize - 1);
        }
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

    const E* data() const {
        return heap;
    }

private:
    E* heap;
    int currentSize;
    int maxHeapSize;

    void siftDown(int start, int m) {
        int i = start;
        int j = 2 * i + 1;
        E temp = heap[i];

        while (j <= m) {
            if (j < m && heap[j] > heap[j + 1]) {
                ++j;
            }

            if (temp <= heap[j]) {
                break;
            }

            heap[i] = heap[j];
            i = j;
            j = 2 * j + 1;
        }

        heap[i] = temp;
    }

    void siftUp(int start) {
        int j = start;
        int i = (j - 1) / 2;
        E temp = heap[j];

        while (j > 0) {
            if (heap[i] <= temp) {
                break;
            }

            heap[j] = heap[i];
            j = i;
            i = (i - 1) / 2;
        }

        heap[j] = temp;
    }
};
