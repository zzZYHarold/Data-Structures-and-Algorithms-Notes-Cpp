#include "SeqList.h"

// 构造函数（PPT 第 15 页）
template <class T>
SeqList<T>::SeqList(int sz) {
    if (sz > 0) {
        maxSize = sz;
        last = -1;  // 空表

        // PPT 使用 new 后判空；现代 C++ 普通 new 失败时会抛异常。
        // 使用 nothrow 保留课件的判空结构。
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

    // PPT 此函数的标题是“扩充”，原代码若缩到 Length() 以下会复制越界。
    // 这里显式拒绝这种情况，不改变“扩充”用途。
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
            return i + 1;   // 返回第几个元素（1-based）
    }
    return 0;
}

// 插入（PPT 第 23 页）
// 注意：这里严格保留 PPT 代码的下标语义：i 为 0-based 插入下标。
template <class T>
bool SeqList<T>::Insert(int i, T& x) {
    if (i < 0 || i > last + 1)
        return false;                  // 参数不合理
    if (last == maxSize - 1)
        return false;                  // 表满

    for (int j = last; j >= i; --j)
        data[j + 1] = data[j];         // 依次后移

    data[i] = x;
    ++last;
    return true;
}

// 删除（PPT 第 26 页）
// i 为 1-based 表项序号。
template <class T>
bool SeqList<T>::Remove(int i, T& x) {
    if (last == -1)
        return false;                  // 表空
    if (i < 1 || i > last + 1)
        return false;                  // 参数不合理

    x = data[i - 1];
    for (int j = i; j <= last; ++j)
        data[j - 1] = data[j];         // 依次前移，填补空位

    last++;
    return true;
}

// 为了让模板实现可以放在 .cpp 中并正常链接，
// 这里显式实例化本章示例实际使用的 int 类型。
template class SeqList<int>;
