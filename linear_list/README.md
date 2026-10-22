# 第二章：线性表与顺序表

**2.1–2.2 依据学院《Lesson 2 (2026秋)-CS - part1》PPT 整理。** 内容包括线性表抽象接口、顺序表及集合并/交；2.3–2.5 见 [linked_list/README.md](../linked_list/README.md)。

## 文件与课件对应

| 文件 | 课件内容 | 实现来源 |
| --- | --- | --- |
| `include/LinearList.h` | 2.1，第 6 页，线性表抽象基类 | 保留课件接口，修正赋值运算符返回类型 |
| `include/SeqList.h` | 2.2，第 12–18、20、23、26 页，顺序表 | 模板声明与完整实现，保留课件下标约定 |
| `include/SetOperations.h`、`src/SetOperations.cpp` | 第 29–30 页，集合并/交 | 按课件使用顺序表实现，去除无效模板参数 |

## 使用方式

项目以仓库根目录作为头文件搜索起点，使用时统一写：

```cpp
#include "linear_list/include/SeqList.h"
#include "linear_list/include/SetOperations.h"
```

## 整理时修正的课件代码问题

1. `enum bool {false, true};` 已删除：现代 C++ 内置 `bool`。
2. `iostream.h` 改为现代头文件 `<iostream>`。
3. `new` 后判空改用 `new (std::nothrow)`，使课件的判空逻辑成立。
4. `Search` 的大小写统一：课件声明为 `Search`，实现页一处写成 `search`。
5. `Insert(int i, E x)` 中的 `E` 改回 `T`。
6. `template <class T,>` 删除多余逗号。
7. 集合 `Union/Intersection` 的 `template<class T>` 是无效的未使用模板参数，整理版去掉。
8. `LinearList::operator=` 原课件按值返回抽象类对象，在现代 C++ 中不成立，改为引用返回。
9. `Remove` 成功后应令 `last` 减一；原整理版误写为加一，本次已修正。

## PPT 的下标约定

- `Search`、`getData`、`setData`、`Remove`：使用 **1-based 表项序号**。
- 第 23 页 `Insert` 的实际代码：使用 **0-based 数组插入下标**，合法范围 `0..Length()`。

本整理版保留了这一点，以便和 PPT 逐行对应。比如向长度为 `n` 的表尾追加元素时，应调用：

```cpp
LA.Insert(n, x);
```

## 模板组织方式

`SeqList<T>` 是类模板，其实现必须在实例化点可见，因此声明和实现都放在
`include/SeqList.h` 中。它不再依赖只针对 `int` 的显式实例化，因而可以直接使用
`SeqList<int>`、`SeqList<double>` 等不同类型。
