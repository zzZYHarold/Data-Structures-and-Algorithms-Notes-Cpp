#pragma once

#include <stdexcept>

// 3.4 优先级队列：数组实现
// 课件约定“数值越小，优先权越高”，因此数组保持升序，
// RemoveMin() 每次删除下标 0 的元素。

template <class T>
class PQueue {
protected:
    T* pqelements;
    int count;
    int maxSize;

    void adjust() {
        // 新元素先放在末尾，再按照插入排序思想向前寻找位置。
        // 对相等元素不越过已有元素，因此相同优先级保持 FIFO。
        T temp = pqelements[count - 1];
        int j = count - 2;

        for (; j >= 0; --j) {
            if (pqelements[j] <= temp)
                break;
            pqelements[j + 1] = pqelements[j];
        }
        pqelements[j + 1] = temp;
    }

public:
    explicit PQueue(int sz = 50)
        : pqelements(nullptr), count(0), maxSize(sz) {
        if (sz <= 0)
            throw std::invalid_argument("priority queue size must be positive");
        pqelements = new T[maxSize];
    }

    ~PQueue() {
        delete[] pqelements;
    }

    PQueue(const PQueue&) = delete;
    PQueue& operator=(const PQueue&) = delete;

    bool Insert(const T& x) {
        if (IsFull())
            return false;

        pqelements[count++] = x;
        adjust();
        return true;
    }

    bool RemoveMin(T& x) {
        if (IsEmpty())
            return false;

        x = pqelements[0];
        for (int i = 1; i < count; ++i)
            pqelements[i - 1] = pqelements[i];

        --count;
        return true;
    }

    bool getFront(T& x) const {
        if (IsEmpty())
            return false;
        x = pqelements[0];
        return true;
    }

    void makeEmpty() {
        count = 0;
    }

    bool IsEmpty() const {
        return count == 0;
    }

    bool IsFull() const {
        return count == maxSize;
    }

    int getSize() const {
        return count;
    }
};
