#ifndef LINEAR_LIST_H
#define LINEAR_LIST_H

// 2.1 线性表的抽象基类
// 根据 PPT 第 6 页整理。

template <class T>
class LinearList {
public:
    LinearList() = default;
    virtual ~LinearList() = default;

    virtual int Size() const = 0;                   // 求表最大体积
    virtual int Length() const = 0;                 // 求表长度
    virtual int Search(T& x) const = 0;             // 在表中搜索给定值 x
    virtual int Locate(int i) const = 0;            // 在表中定位第 i 个元素位置
    virtual T* getData(int i) const = 0;            // 取得第 i 个表项的值
    virtual void setData(int i, T& x) = 0;          // 修改第 i 个表项的值为 x
    virtual bool Insert(int i, T& x) = 0;           // 插入
    virtual bool Remove(int i, T& x) = 0;           // 删除第 i 个表项，通过 x 返回
    virtual bool IsEmpty() const = 0;               // 判表空
    virtual bool IsFull() const = 0;                // 判表满
    virtual void Sort() = 0;                        // 排序
    virtual void input() = 0;                       // 输入
    virtual void output() = 0;                      // 输出

    // PPT 原式按值返回 LinearList<T>，但抽象类不能按值返回对象。
    // 这里改为引用返回，使接口在现代 C++ 中合法。
    virtual LinearList<T>& operator=(LinearList<T>& L) = 0;
};

#endif
