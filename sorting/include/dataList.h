#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

// 2026 秋 Lesson 9 最终版，9.1。独立命名空间允许与第五、七章的同名类型一起使用。
namespace sorting {

inline constexpr int DefaultSize = 100;

template <class T, class Field = std::monostate>
struct Element {
    T key{};
    Field otherdata{};

    bool operator<(const Element& other) const { return key < other.key; }
    bool operator>(const Element& other) const { return other < *this; }
    bool operator<=(const Element& other) const { return !(other < *this); }
    bool operator>=(const Element& other) const { return !(*this < other); }
    bool operator==(const Element& other) const { return key == other.key; }
};

namespace detail {

// 闭区间允许 [left, left-1] 表示空区间。
inline void CheckRange(int length, int left, int right) {
    if (left < 0 || left > length || right < left - 1 || right >= length)
        throw std::out_of_range("invalid sorting range");
}

} // namespace detail

template <class T, class Field = std::monostate>
class dataList {
public:
    explicit dataList(int capacity = DefaultSize) : maxSize(capacity) {
        if (capacity < 0)
            throw std::invalid_argument("negative sorting table capacity");
        Vector = std::make_unique<Element<T, Field>[]>(capacity);
    }

    dataList(const dataList& other) : dataList(other.maxSize) {
        for (int index = 0; index < other.Length(); ++index)
            Append(other[index]);
    }

    dataList& operator=(const dataList& other) {
        if (this != &other) {
            dataList copy(other);
            Vector.swap(copy.Vector);
            std::swap(maxSize, copy.maxSize);
            std::swap(currentSize, copy.currentSize);
        }
        return *this;
    }

    int Length() const { return currentSize; }

    // 课件未给出装入数据的接口。追加成功后才增加有效长度。
    bool Append(const Element<T, Field>& element) {
        if (currentSize == maxSize)
            return false;
        Vector[currentSize] = element;
        ++currentSize;
        return true;
    }

    Element<T, Field>& operator[](int index) {
        CheckIndex(index);
        return Vector[index];
    }

    const Element<T, Field>& operator[](int index) const {
        CheckIndex(index);
        return Vector[index];
    }

    void Swap(Element<T, Field>& first, Element<T, Field>& second) const {
        const auto saved = first;
        first = second;
        second = saved;
    }

    // 第 50 页前后指针法：左侧 < pivot，右侧 >= pivot。
    int Partition(int low, int high) {
        detail::CheckRange(Length(), low, high);
        if (low > high)
            throw std::out_of_range("partition requires a pivot");

        int pivotPosition = low;
        const auto pivot = Vector[low];
        for (int index = low + 1; index <= high; ++index) {
            if (Vector[index] < pivot) {
                ++pivotPosition;
                if (pivotPosition != index)
                    Swap(Vector[pivotPosition], Vector[index]);
            }
        }
        Vector[low] = Vector[pivotPosition];
        Vector[pivotPosition] = pivot;
        return pivotPosition;
    }

    // 第 55 页挖坑法：等键可能留在基准的任意一侧。
    int Partition_Classic(int low, int high) {
        detail::CheckRange(Length(), low, high);
        if (low > high)
            throw std::out_of_range("partition requires a pivot");

        const auto pivot = Vector[low];
        while (low < high) {
            while (low < high && Vector[high] >= pivot)
                --high;
            if (low < high) {
                Vector[low] = Vector[high];
                ++low;
            }

            while (low < high && Vector[low] <= pivot)
                ++low;
            if (low < high) {
                Vector[high] = Vector[low];
                --high;
            }
        }
        Vector[low] = pivot;
        return low;
    }

private:
    std::unique_ptr<Element<T, Field>[]> Vector;
    int maxSize;
    int currentSize = 0;

    void CheckIndex(int index) const {
        if (index < 0 || index >= currentSize)
            throw std::out_of_range("sorting table index out of range");
    }
};

} // namespace sorting
