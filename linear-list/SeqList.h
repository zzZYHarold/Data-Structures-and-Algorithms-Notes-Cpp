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

// 本次 .cpp 分离版本显式实例化 int 类型；与 PPT 的集合示例一致。
extern template class SeqList<int>;

#endif
