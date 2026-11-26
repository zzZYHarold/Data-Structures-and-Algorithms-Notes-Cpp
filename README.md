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
| 6.1 | 位向量集合、有序链表集合 | [set/](./set/) | `bitSet.h`、`LinkedSet.h` |
| 6.2 | 并查集、等价类、省份问题 | [set/](./set/) | `UFSets.h/.cpp`、`SetApplications.h/.cpp` |
| 6.3 | 字典、有序顺序表/链表、搜索、跳表概念及双向搜索练习 | [dictionary/](./dictionary/) | `SortedList.h`、`SortedChain.h`、`BidirectionalSearch.h/.cpp`（跳表仅概念说明） |
| 6.4 | 散列：线性/平方/双向二次探查、双散列、链地址法 | [dictionary/](./dictionary/) | `HashTable.h`、`ChainedHashTable.h`、`examples/chapter6.cpp` |
| 7.1 | 静态搜索表、监视哨/递归顺序搜索、折半搜索 | [search/](./search/) | `dataList.h`、`examples/chapter7.cpp` |
| 7.2 | 二叉搜索树、递归/迭代搜索、插入、后继替补删除 | [tree/](./tree/) | `BST.h` |
| 7.3 | AVL 平衡因子、四种旋转、插入及前驱替补删除 | [tree/](./tree/) | `AVLTree.h` |
| 8.1 | 图的基本概念、图的抽象接口 | [graph/](./graph/) | `Graph.h`（连通性与生成树仅概念） |
| 8.2 | 邻接矩阵、邻接表、图的增删与邻接点枚举 | [graph/](./graph/) | `Graphmtx.h`、`Graphlnk.h`、`examples/chapter8_part1.cpp` |
| 8.3 | DFS、BFS、无向图连通分量 | [graph/](./graph/) | `GraphTraversal.h`、`examples/chapter8_part2.cpp` |
| 8.4 | 最小生成树：Kruskal、候选边堆版本 Prim | [graph/](./graph/) | `MinSpanTree.h`、`MinimumSpanningTree.h`、`examples/chapter8_part2.cpp` |
| 8.5 | Dijkstra 单源最短路径、Floyd 全源最短路径 | [graph/](./graph/) | `ShortestPath.h`、`examples/chapter8_part3.cpp` |
| 8.6 | AOV 拓扑排序、AOE 工期与关键活动 | [graph/](./graph/) | `ActivityNetwork.h`、`examples/chapter8_part3.cpp` |
| 9.1 | 排序码、稳定性和待排序数据表 | [sorting/](./sorting/) | `dataList.h` |
| 9.2 | 直接插入、折半插入、希尔排序 | [sorting/](./sorting/) | `Sort.h` |
| 9.3 | 冒泡、快速排序及两种划分方法 | [sorting/](./sorting/) | `Sort.h`、`dataList.h` |
| 9.4 | 直接选择、锦标赛、堆排序 | [sorting/](./sorting/) | `Sort.h` |
| 9.5 | 迭代二路归并、递归静态链表归并 | [sorting/](./sorting/) | `Sort.h`、`staticlinkList.h`、`examples/chapter9.cpp` |

第五章 **5.1–5.7** 已统一按 2026 秋完整正式课件整理。具体实现范围、课件勘误和接口用法见 [tree/README.md](./tree/README.md)。

第六章 **6.1–6.4** 已统一按 `Lesson 6 (2026秋)-CS(1).pdf`（136 页）完整正式课件核对。散列部分区分单向平方探查与 `H₀,+1,-1,+4,-4,…` 双向二次探查，并补充表长 19 的二次探查、表长 11 的双散列例子及 ASL 检查。版本、实现范围、接口约定和课件勘误见 [set/README.md](./set/README.md) 和 [dictionary/README.md](./dictionary/README.md)，运行目标为 `chapter6_example`。

第七章 **7.1–7.3** 已按 `Lesson 7 (2026秋季)-BeibeiWang-最终.pdf`（111 页）核对。最终版新增图形学搜索应用、展开 BST 删除图示并收录作业，现有核心算法继续适用。静态搜索、版本差异和勘误见 [search/README.md](./search/README.md)，搜索树见 [tree/README.md](./tree/README.md#第七章二叉搜索树与-avl-树)。全章目标为 `chapter7_example`，包含最终版第 111 页 AVL 连续删除作业。

第八章已按 `Lesson 8 (2026秋季)-BeibeiWang-最终.pdf`（158 页）核对 **8.1–8.6**。已同步最终版页码与关键路径勘误说明，并收录第 93～94 页遍历/生成树作业、第 109 页最短路径作业及 Dijkstra/Floyd 过程表。页码、课件勘误与算例见 [graph/README.md](./graph/README.md)。三个运行目标为 `chapter8_part1_example`、`chapter8_part2_example`、`chapter8_part3_example`。

第九章 **9.1–9.5** 已按 `Lesson 9 (2026秋季)-BeibeiWang-最终.pdf`（119 页）核对。[排序章节 README](./sorting/README.md) 收录 10 种排序的性能与稳定性对照、两种快排的搬移次数分析和第 119 页作业。全章示例目标为 `chapter9_example`，包含新算例和作业过程自检。

## 项目约定

项目使用 C++20 和 CMake。普通课程代码由根目录 `CMakeLists.txt` 的 `course_code` 目标编译检查，`examples/` 下的示例按需设置独立目标。头文件引用以仓库根目录为起点，例如 `#include "stack/include/SeqStack.h"`。后续整理规则见 [AGENTS.md](./AGENTS.md)。
