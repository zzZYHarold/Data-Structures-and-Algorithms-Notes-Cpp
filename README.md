# 数据结构与算法课程代码笔记（C++）

本仓库按**数据结构主题**组织代码，因此同一章可能分布在几个目录中。下面按 PPT 小节列出查找入口；点进目录后可查看该目录的 README 和具体文件。

## PPT 小节与代码目录

| PPT 小节 | 内容 | 代码目录 | 主要文件 |
| --- | --- | --- | --- |
| 2.1–2.2 | 线性表、顺序表、集合运算 | [linear_list/](./linear_list/) | `LinearList.h`、`SeqList.h`、`SetOperations.h/.cpp` |
| 2.3 | 单链表、静态链表 | [linked_list/](./linked_list/) | `LinkedList.h`、`StaticLinkedList.h/.cpp` |
| 2.4 | 循环链表、双向链表、约瑟夫问题 | [linked_list/](./linked_list/) | `CircList.h`、`DblList.h`、`Josephus.h` |
| 2.5 | 多项式及其存储与加法 | [linked_list/](./linked_list/) | `PolynomialStorage.h`、`Polynomial.h/.cpp` |
| 3.1 | 栈及其应用 | [stack/](./stack/) | `Stack.h`、`SeqStack.h`、`LinkedStack.h`、`DualStack.h`、`StackApplications.h/.cpp` |
| 3.2 | 栈与递归 | [stack/](./stack/) | `RecursionExamples.h/.cpp` |
| 3.3 | 队列与杨辉三角 | [queue/](./queue/) | `Queue.h`、`SeqQueue.h`、`LinkedQueue.h`、`QueueApplications.h/.cpp` |
| 3.4 | 优先级队列 | [queue/](./queue/) | `PQueue.h` |
| 3.5 | 双端队列 | [queue/](./queue/) | `Deque.h`、`SeqDeque.h` |
| 4.1 | 数组存储与图像翻转示例 | [array/](./array/) | `ArrayStorage.h/.cpp`、`examples/image_flip.cpp` |
| 4.2 | 特殊矩阵的压缩存储 | [array/](./array/) | `SymmetricMatrix.h`、`TriDiagonalMatrix.h`、`BandMatrix.h` |
| 4.3 | 稀疏矩阵 | [array/](./array/) | `SparseMatrix.h` |
| 4.4 | 字符串与模式匹配 | [string/](./string/) | `AString.h/.cpp` |
| 4.5 | 广义表 | [generalized_list/](./generalized_list/) | `GenList.h` |
| 5.1 | 树的基本概念 | [tree/](./tree/) | 无单独实现文件 |
| 5.2–5.3 | 二叉树及其遍历、建树等操作 | [tree/](./tree/) | `BinaryTree.h` |
| 5.4 | 线索二叉树 | [tree/](./tree/) | `ThreadTree.h` |
| 5.5 | 树与森林：子女—兄弟表示、树的遍历 | [tree/](./tree/) | `Tree.h` |
| 5.6 | 最小堆、关键码与附加数据 | [tree/](./tree/) | `MinHeap.h`（`Element`、`MinHeap`） |
| 5.7 | Huffman 树 | [tree/](./tree/) | `HuffmanTree.h` |

第五章 **5.1–5.7** 已统一按 2026 秋完整正式课件整理。具体实现范围、课件勘误和接口用法见 [tree/README.md](./tree/README.md)。

## 项目约定

项目使用 C++20 和 CMake。普通课程代码由根目录 `CMakeLists.txt` 的 `course_code` 目标编译检查，`examples/` 下的示例按需设置独立目标。头文件引用以仓库根目录为起点，例如 `#include "stack/include/SeqStack.h"`。后续整理规则见 [AGENTS.md](./AGENTS.md)。
