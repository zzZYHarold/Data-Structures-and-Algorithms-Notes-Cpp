# Lesson 2：2.1-2.2 代码整理

来源：学院《Lesson 2 (2026秋)-CS - part1》PPT。

文件对应：

- `LinearList.h`：2.1 线性表抽象基类（PPT 第 6 页）
- `SeqList.h`：2.2 顺序表类声明与页内联成员（第 12-14 页）
- `SeqList.cpp`：构造、复制构造、扩容、查找、插入、删除（第 15-18、20、23、26 页）
- `SetOperations.h/.cpp`：顺序表实现集合并/交（第 29-30 页）

## 整理时修正的课件代码问题

1. `enum bool {false, true};` 已删除：现代 C++ 内置 `bool`。
2. `iostream.h` 改为现代头文件 `<iostream>`。
3. `new` 后判空改用 `new (std::nothrow)`，使课件的判空逻辑成立。
4. `Search` 的大小写统一：课件声明为 `Search`，实现页一处写成 `search`。
5. `Insert(int i, E x)` 中的 `E` 改回 `T`。
6. `template <class T,>` 删除多余逗号。
7. 集合 `Union/Intersection` 的 `template<class T>` 是无效的未使用模板参数，整理版去掉。
8. `LinearList::operator=` 原课件按值返回抽象类对象，在现代 C++ 中不成立，改为引用返回。

## 特别注意：PPT 的下标约定不统一

- `Search`、`getData`、`setData`、`Remove`：使用 **1-based 表项序号**。
- 第 23 页 `Insert` 的实际代码：使用 **0-based 数组插入下标**，合法范围 `0..Length()`。

本整理版保留了这一点，以便和 PPT 逐行对应。比如向长度为 `n` 的表尾追加元素时，应调用：

```cpp
LA.Insert(n, x);
```

## 模板与 .cpp

现代 C++ 的类模板通常把实现放在头文件（或 `.tpp`）中。为了按本次要求保留 `.h + .cpp` 分离，同时能直接编译，本整理版在 `SeqList.cpp` 中显式实例化了：

```cpp
template class SeqList<int>;
```

因此当前文件组直接支持本章示例使用的 `SeqList<int>`。
