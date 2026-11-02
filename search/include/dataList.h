#pragma once

#include <istream>
#include <limits>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

// 2026 秋 Lesson 7（预习课件，已核对），7.1：E 是附加数据类型，K 是关键码类型。
template <class E, class K> class dataList;

template <class E, class K>
class dataNode {
    friend class dataList<E, K>;
public:
    dataNode() = default;
    explicit dataNode(const K& value, const E& payload = E()) : key(value), other(payload) {}
    K getKey() const { return key; }
    void setKey(K value) { key = value; }
    const E& getOther() const { return other; } // 补充：读取附加数据。
private:
    K key{};
    E other{};
};

template <class E, class K>
class dataList {
public:
    explicit dataList(int capacity = 100) : ArraySize(capacity) {
        if (capacity < 0 || capacity == std::numeric_limits<int>::max())
            throw std::invalid_argument("invalid search table capacity");
        // 额外保留一个监视哨槽；ArraySize 仍为有效数据容量。
        Element = new dataNode<E, K>[capacity + 1];
    }

    dataList(const dataList& other) : ArraySize(other.ArraySize), CurrentSize(other.CurrentSize) {
        auto copied = std::make_unique<dataNode<E, K>[]>(ArraySize + 1);
        for (int index = 0; index < CurrentSize; ++index)
            copied[index] = other.Element[index];
        Element = copied.release();
    }

    dataList& operator=(const dataList& other) {
        if (this != &other) {
            dataList copied(other);
            std::swap(Element, copied.Element);
            std::swap(ArraySize, copied.ArraySize);
            std::swap(CurrentSize, copied.CurrentSize);
        }
        return *this;
    }

    virtual ~dataList() { delete[] Element; }
    virtual int Length() const { return CurrentSize; }
    virtual K getKey(int position) const { return getData(position).getKey(); }
    virtual void setKey(K value, int position) {
        checkPosition(position);
        Element[position - 1].key = value;
    }

    const dataNode<E, K>& getData(int position) const {
        checkPosition(position);
        return Element[position - 1];
    }

    // 课件的 E 为附加域，却把 E 赋给整个结点；修正为插入完整 dataNode。
    virtual bool Insert(const dataNode<E, K>& entry) {
        if (CurrentSize == ArraySize)
            return false;
        Element[CurrentSize] = entry;
        ++CurrentSize;
        return true;
    }
    bool Insert(const K& key, const E& payload) { return Insert(dataNode<E, K>(key, payload)); }

    virtual bool Remove(K key, E& removedPayload) {
        int index = 0;
        while (index < CurrentSize && Element[index].key != key)
            ++index;
        if (index == CurrentSize)
            return false;
        removedPayload = Element[index].other;
        Element[index] = Element[CurrentSize - 1]; // 尾元素填补，不保序。
        --CurrentSize;
        return true;
    }

    // 第 17 页：成功返回 1-based 序号，失败返回 Length()+1。
    virtual int SeqSearch(const K key) const {
        // 监视哨保证循环终止，每轮只比较关键码，无需再检查数组边界。
        Element[CurrentSize].key = key;
        int index = 0;
        while (Element[index].key != key)
            ++index;
        return index + 1;
    }

    // 第 20 页：递归版失败返回 0，与监视哨版不同。
    int SeqSearch(const K key, int position) const {
        // position是1-based
        if (position < 1)
            throw std::out_of_range("search position must be positive");
        if (position > CurrentSize)
            return 0;
        if (Element[position - 1].key == key)
            return position;
        return SeqSearch(key, position + 1);
    }

    // 补充：第 25 页判定树对应的折半搜索，要求关键码非递减。
    // 成功返回 1-based 序号，失败为 0；不自动排序。
    int BinarySearch(const K key) const {
        int low = 0, high = CurrentSize - 1;
        while (low <= high) {
            int middle = low + (high - low) / 2;
            if (key < Element[middle].key)
                high = middle - 1;
            else if (key > Element[middle].key)
                low = middle + 1;
            else return middle + 1;
        }
        return 0;
    }

    // 补充输入格式：数量，随后是各个 key other；成功后替换原表。
    friend std::istream& operator>>(std::istream& input, dataList& table) {
        int count;
        if (!(input >> count)) return input;
        if (count < 0 || count > table.ArraySize) {
            input.setstate(std::ios::failbit);
            return input;
        }
        dataList replacement(table.ArraySize);
        for (int index = 0; index < count; ++index) {
            K key;
            E payload;
            if (!(input >> key >> payload)) return input;
            replacement.Insert(key, payload);
        }
        table = replacement;
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const dataList& table) {
        for (int position = 1; position <= table.Length(); ++position)
            output << table.getKey(position) << ':' << table.getData(position).getOther() << '\n';
        return output;
    }

protected:
    dataNode<E, K>* Element = nullptr;
    int ArraySize;
    int CurrentSize = 0;

private:
    void checkPosition(int position) const {
        if (position < 1 || position > CurrentSize)
            throw std::out_of_range("search table position out of range");
    }
};
