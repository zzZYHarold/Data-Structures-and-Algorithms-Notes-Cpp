# Lesson 2 — Sections 2.3–2.5 C++ Implementations

依据《Lesson 2 (2026秋)-CS》2.3–2.5 整理。整理在 `linked_list/` 目录学习与编译。

## 文件对应

- `LinkedList.h/.cpp`
  - 2.3 单链表
  - `LinkNode<T>`、带附加头结点的 `List<T>`
  - `Length`、`Search`、`Locate`、`getData/setData`
  - `Insert`、`Remove`
  - 前插法 / 后插法建表
  - `makeEmpty`
  - 课件末尾练习：单链表逆置

- `CircList.h/.cpp`
  - 2.4 循环链表
  - 带附加头结点的循环链表
  - `Length`、`Search`、`Locate`、`Insert`、`Remove`

- `Josephus.h/.cpp`
  - 2.4 约瑟夫问题
  - 按 PPT 的循环链表报数、跳过附加头结点、删除第 m 个结点的思路实现

- `DblList.h/.cpp`
  - 2.4 双向循环链表
  - `DblNode<T>`、`DblList<T>`
  - 双向 `Search/Locate`
  - 前驱方向 / 后继方向 `Insert`、`Remove`

- `PolynomialStorage.h`
  - 2.5 多项式的前三种顺序存储表示
  - 静态系数数组、动态系数数组、稀疏项 `(coef, exp)` 表示

- `Polynomial.h/.cpp`
  - 2.5 多项式链表存储
  - `Term`、`Polynomial`
  - 按指数升序存储非零项
  - 两个多项式相加（对应 PPT 第 112–121 页的双指针合并算法）

## 与 PPT 原代码相比的必要整理

课件包含若干旧式/笔误代码。这里以“保持算法不变、保证现代 C++17 可编译”为原则做了必要修正：

1. `NULL` 统一为 `nullptr`。
2. 修正全角标点、缺失括号、类名 `DbNode/DblNode` 不一致等明显排版错误。
3. `List::~List()`：PPT 只调用 `makeEmpty()`，会保留并泄漏附加头结点；这里额外 `delete first`。
4. PPT 的 `inputFront/inputRear` 片段依赖外部对象 `L.setFirst(...)`，但类声明没有对应接口；这里改成 `List<T>` 成员函数，直接操作自身 `first`，算法步骤不变。
5. 循环链表类在 PPT 中声明为继承 `LinearList<T>`，但当前页给出的接口与 2.1 抽象基类并不完整匹配；本整理不强行继承，避免生成一个无法实例化的抽象类。循环链表本身的结构与算法保持一致。
6. PPT 的 `CircList` 示例 `main()` 使用默认构造，但类声明只展示了 `CircList(const T x)`；这里同时提供默认构造。
7. PPT 的 Josephus 代码直接摘链，会使类中的 `last` 在删除尾结点后可能失效；这里通过 `eraseNode()` 同步维护 `last`。
8. PPT 双向链表类声明里的 `Search` 与实现页参数不一致；这里采用实现页形式 `Search(x, d)`，`d=0` 沿前驱，`d!=0` 沿后继。
9. PPT 双向链表 `Insert` 中出现 `if (p=first&&i>0)`，属于赋值/比较笔误；这里按语义修正。
10. PPT 双向链表 `Remove` 的条件可能允许删除 `i=0` 的附加头结点；这里明确要求 `i>=1`。
11. 2.5 多项式链表只给出了类定义和“相加算法步骤”，没有完整函数代码；`operator+` 是严格按 PPT 第 114 页的指数比较/合并流程补成的可运行版本。

## 关于模板文件为什么 `.h` 会包含 `.cpp`

课程使用类模板，又要求按 `.h/.cpp` 分文件。C++ 模板定义必须在实例化点可见，因此本项目让 `.h` 在末尾包含对应 `.cpp` 实现文件。这样既保留课程的 `.h/.cpp` 结构，又可以直接 `#include "LinkedList.h"` 使用任意 `T`。

在 CMake 中，即使这些 `.cpp` 也被作为源文件单独列出，也不会产生普通非模板函数那样的重复定义问题；但更简洁的做法是模板类只把 `.h` 加入工程即可。

## 复杂度要点

课件本章小结给出的结论：

- 链表按位置查找：`O(n)`
- 插入 / 删除如果包含“先找到第 i 个位置”：总体 `O(n)`
- 若目标结点（或其前驱）已经拿到，只考虑改指针：`O(1)`

