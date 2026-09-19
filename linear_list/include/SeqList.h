#ifndef SEQ_LIST_H
#define SEQ_LIST_H

#include <cstdlib>
#include <iostream>
#include <new>

const int defaultSize = 100;

// 2.2 顺序表
// 根据 PPT 第 12~18、20、23、26 页整理。
template <class T>
class SeqList {
protected:
    T* data;          // 存放数组
    int maxSize;      // 最大可容纳表项的项数
    int last;         // 最后一个表项的下标；空表时为 -1

    void reSize(int newSize);   // 改变 data 数组空间大小

public:
    explicit SeqList(int sz = defaultSize);  // 构造函数
    SeqList(SeqList<T>& L);                  // 拷贝构造函数
    ~SeqList() { delete[] data; }            // 析构函数

    int Size() const { return maxSize; }
    int Length() const { return last + 1; }

    int Search(T& x) const;                  // 找到返回 1-based 序号，否则返回 0

    bool getData(int i, T& x) const {        // i 为 1-based 序号
        if (i > 0 && i <= last + 1) {
            x = data[i - 1];
            return true;
        }
        return false;
    }

    void setData(int i, T& x) {              // i 为 1-based 序号
        if (i > 0 && i <= last + 1)
            data[i - 1] = x;
    }

    // 注意：PPT 第 23 页代码中的 i 实际按数组下标使用，范围为 0..Length()。
    // 因此它与 getData / Remove 的 1-based 序号约定并不一致。
    // 这里保留 PPT 的实际代码语义，便于和课件逐行对应。
    bool Insert(int i, T& x);

    bool Remove(int i, T& x);                // i 为 1-based 序号

    bool IsEmpty() const { return last == -1; }
    bool IsFull() const { return last == maxSize - 1; }

    // PPT 在本文件 2.2 小节中只给出了下列成员的声明，未给出实现：
    // int Locate(int i) const;
    // void input();
    // void output();
    // SeqList<T> operator=(SeqList<T>& L);
};

// 构造函数（PPT 第 15 页）
template <class T>
SeqList<T>::SeqList(int sz) {
    if (sz > 0) {
        maxSize = sz;
        last = -1;

        data = new (std::nothrow) T[maxSize];
        if (data == nullptr) {
            std::cerr << "存储分配错误！" << std::endl;
            std::exit(1);
        }
    } else {
        maxSize = 0;
        last = -1;
        data = nullptr;
    }
}

// 拷贝构造函数（PPT 第 16 页）
template <class T>
SeqList<T>::SeqList(SeqList<T>& L) {
    maxSize = L.Size();
    last = L.Length() - 1;

    data = new (std::nothrow) T[maxSize];
    if (data == nullptr) {
        std::cerr << "存储分配错误！" << std::endl;
        std::exit(1);
    }

    T value;
    for (int i = 1; i <= last + 1; ++i) {
        L.getData(i, value);
        data[i - 1] = value;
    }
}

// 扩充存储数组空间（PPT 第 18 页）
template <class T>
void SeqList<T>::reSize(int newSize) {
    if (newSize <= 0) {
        std::cerr << "无效的数组大小！" << std::endl;
        return;
    }

    if (newSize < last + 1) {
        std::cerr << "新空间不能小于当前表长！" << std::endl;
        return;
    }

    if (newSize != maxSize) {
        T* newArray = new (std::nothrow) T[newSize];
        if (newArray == nullptr) {
            std::cerr << "存储分配错误！" << std::endl;
            std::exit(1);
        }

        int n = last + 1;
        T* srcPtr = data;
        T* destPtr = newArray;
        while (n--)
            *destPtr++ = *srcPtr++;

        delete[] data;
        data = newArray;
        maxSize = newSize;
    }
}

// 顺序搜索（PPT 第 20 页）
template <class T>
int SeqList<T>::Search(T& x) const {
    for (int i = 0; i <= last; ++i) {
        if (data[i] == x)
            return i + 1;
    }
    return 0;
}

// 插入（PPT 第 23 页）：i 为 0-based 插入下标。
template <class T>
bool SeqList<T>::Insert(int i, T& x) {
    if (i < 0 || i > last + 1)
        return false;
    if (last == maxSize - 1)
        return false;

    for (int j = last; j >= i; --j)
        data[j + 1] = data[j];

    data[i] = x;
    ++last;
    return true;
}

// 删除（PPT 第 26 页）：i 为 1-based 表项序号。
template <class T>
bool SeqList<T>::Remove(int i, T& x) {
    if (last == -1)
        return false;
    if (i < 1 || i > last + 1)
        return false;

    x = data[i - 1];
    for (int j = i; j <= last; ++j)
        data[j - 1] = data[j];

    --last;
    return true;
}

#endif
