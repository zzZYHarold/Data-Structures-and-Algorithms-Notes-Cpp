# Lesson 2：2.3–2.5 代码整理

依据《Lesson 2 (2026秋)-CS》2.3–2.5 整理。

## 目录结构

```text
linked_list/
├── include/
│   ├── LinkedList.h
│   ├── CircList.h
│   ├── DblList.h
│   ├── Josephus.h
│   ├── StaticLinkedList.h
│   ├── Polynomial.h
│   └── PolynomialStorage.h
├── src/
│   ├── StaticLinkedList.cpp
│   └── Polynomial.cpp
└── README.md
```

`LinkedList<T>`、`CircList<T>`、`DblList<T>` 和 `Josephus<T>` 都是模板，完整实现放在
各自的 `.h` 中。`StaticLinkedList` 与 `Polynomial` 是普通实现，因此保留 `.h + .cpp`
结构。

项目以仓库根目录作为头文件搜索起点，例如：

```cpp
#include "linked_list/include/LinkedList.h"
#include "linked_list/include/StaticLinkedList.h"
```

## 文件对应

- `include/LinkedList.h`
  - 2.3 单链表
  - `LinkNode<T>`、带附加头结点的 `List<T>`
  - 查找、定位、插入、删除、建表、清空和逆置

- `include/StaticLinkedList.h`、`src/StaticLinkedList.cpp`
  - 2.3.5 静态链表
  - 数组保存结点，`next` 保存后继结点的数组下标
  - 初始化、判空、求长、按位访问、按值查找、插入、删除和输出

- `include/CircList.h`
  - 2.4 带附加头结点的循环链表
  - `Length`、`Search`、`Locate`、`Insert`、`Remove`

- `include/Josephus.h`
  - 2.4 约瑟夫问题
  - 按循环链表报数、跳过附加头结点并删除第 `m` 个结点

- `include/DblList.h`
  - 2.4 双向循环链表
  - 双向查找、定位、插入和删除

- `include/PolynomialStorage.h`
  - 2.5 多项式的三种顺序存储表示
  - 静态系数数组、动态系数数组、稀疏项 `(coef, exp)` 表示

- `include/Polynomial.h`、`src/Polynomial.cpp`
  - 2.5 多项式链表存储
  - 按指数升序存储非零项，并用双指针合并完成多项式相加

## 静态链表的实现约定

课件给出了 `SLinkNode` 与数组存储结构，但没有规定空闲结点的管理方式。本整理版采用：

- `list[0]` 为附加头结点；
- `next == -1` 表示链表结束；
- `next == -2` 表示该数组槽位空闲；
- 插入和删除位置使用 1-based 表项序号；
- `MaxSize == 10` 时最多保存 9 个数据元素。

当前 `ElemType` 为 `int`。分配结点时扫描下标 `1..MaxSize-1`，删除后把槽位重新标记为
空闲，后续插入可以复用。

## 与 PPT 原代码相比的必要整理

课件包含若干旧式写法和笔误。这里以“保持算法不变、保证现代 C++20 可编译”为原则做了必要修正：

1. `NULL` 统一为 `nullptr`。
2. 修正全角标点、缺失括号、类名 `DbNode/DblNode` 不一致等明显排版错误。
3. `List::~List()` 除清空数据结点外，也释放附加头结点。
4. `inputFront/inputRear` 改成 `List<T>` 成员函数，直接操作自身头结点。
5. `CircList` 不强行继承接口不完整匹配的 `LinearList<T>`，并补充默认构造。
6. Josephus 删除结点时通过 `eraseNode()` 同步维护尾指针。
7. 双向链表统一 `Search(x, d)` 接口，并禁止删除附加头结点。
8. 多项式 `operator+` 按课件的指数比较与合并流程补成可运行实现。
9. 模板实现全部合并到头文件，不再把 `.cpp` 当作模板头文件包含。

## 复杂度要点

- 链表按位置查找：`O(n)`
- 插入 / 删除若包含“先找到第 i 个位置”：总体 `O(n)`
- 已知目标结点或其前驱时，修改链接：`O(1)`
- 当前静态链表扫描空闲槽位：`O(MaxSize)`
