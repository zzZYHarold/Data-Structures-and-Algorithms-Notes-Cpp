# 第八章 图

以 **`Lesson 8 (2026秋季)-BeibeiWang-最终.pdf`（158 页）**为准，于 2026-11-20 按最终版核对。以下页码均为最终 PDF 的页序。

| 小节 | 正文页码 | 实现 |
| --- | --- | --- |
| 8.1 | 13～23 | 图的概念与抽象接口 |
| 8.2 | 25～50 | 邻接矩阵与邻接表 |
| 8.3 | 52～65 | DFS、BFS、无向图连通分量 |
| 8.4 | 67～91 | Kruskal、候选边堆版本 Prim |
| 8.5 | 96～120 | Dijkstra、Floyd |
| 8.6 | 122～158 | AOV 拓扑排序、AOE 关键活动与工期 |

最终版沿用现有图存储和核心算法。第 93 页正式收录原 part2 第 66 页的遍历作业，第 94 页新增 Prim 和 Kruskal 作业，第 109 页收录最短路径作业与地图导航思考题。第 154 页已修正预习版关键路径代码的 `Vl` 初始化与逆向循环边界。历史验证记录保留各次整理日期。

## 文件与课件对应

| 文件 | 课件范围 | 实现来源 |
| --- | --- | --- |
| `include/Graph.h` | 第 22～28 页 ADT 与模板基类 | 保留类名、计数字段与核心接口，统一为可编译的抽象基类 |
| `include/Graphmtx.h` | 第 29～38 页邻接矩阵 | 顶点表、二维矩阵、构造与邻接点扫描按课件；增删操作按声明补写 |
| `include/Graphlnk.h` | 第 39～50 页邻接表 | `Edge<T,E>`、`Vertex<T,E>`、`NodeTable`、构造/析构与邻接点查找按课件；取权与增删按声明补写 |
| 上述两个派生类的流操作 | 第 33、44 页仅有友元声明 | 补充统一输入/输出格式与失败时保留原图的处理 |
| `examples/chapter8_part1.cpp` | 第 40 页图与接口边界 | 补充的可运行示例和自检，包含删点搬移、邻接枚举、流操作与多态析构 |
| `src/README.md` | 目录说明 | 模板实现均在头文件中 |

## 存储与接口约定

构造函数默认创建带权无向简单图。两种存储都可通过第二个构造参数 `GraphKind::Directed` 创建有向图，`IsDirected()` 可查询模式。有向邻接表存出边，有向邻接矩阵只更新对应的行列单元。无权图和 AOV 的先后关系可用权值 `1` 表示。

- `T` 是顶点值，支持默认构造、复制和相等比较；使用流操作时还需支持 `>>`/`<<`。顶点值要求唯一，重复插入返回 `false`（补充约定，避免按值定位产生歧义）。
- `E` 是能表示 `maxWeight` 的内置数值类型，例如 `int`、`double`。课件没有给出无穷大数值，本仓库选择 `maxWeight = 1'000'000'000`，默认容量 `DefaultVertices = 30`。
- 无向图边权范围为 `0 < cost < maxWeight`。有向图允许 `-maxWeight < cost < maxWeight`，包括零权弧。矩阵按非对角且小于哨兵识别出边。两种模式均拒绝 NaN 和无穷大。
- 容量固定、不自动扩容；容量 0 合法，负容量抛出 `std::invalid_argument`。顶点从 **0 开始编号**，有效范围 `[0, NumberOfVertices())`。
- `insertVertex` 在满表或重复值时返回 `false`。`insertEdge` 在下标非法、自环、重复边或权值非法时返回 `false`，不会更新已有边权。`removeEdge` 在边不存在或下标非法时返回 `false`；无向边的两个方向共同增删，`numEdges` 只计一次。
- `removeVertex(v)` 删除关联的所有边，并用**末顶点填补空位**。原末顶点的下标会变为 `v`；矩阵同步搬移行列，邻接表同步修改相关 `dest`。不要在删点后继续沿用旧的末顶点编号或旧的邻接枚举位置。允许删除最后一个顶点得到空图，也可重新插入。
- `getValue(v)` / `getWeight(v,w)` 的非法下标抛出 `std::out_of_range`，不再用 `0`/`NULL` 冒充任意类型的失败值。合法下标的对角权为 `0`，非对角无边为 `maxWeight`，两种存储的取权语义一致。
- `getVertexPos(value)` 按顶点值查找，找到返回 0-based 下标，未找到返回 `-1`。part2 的类外 DFS/BFS/Prim 需要调用它，现已从保护/私有接口提升为公开虚接口。
- `getFirstNeighbor(v)` 没有邻接点或下标非法时返回 `-1`。`getNextNeighbor(v,w)` 要传当前枚举到的邻接点，结束/非法下标返回 `-1`。矩阵保留课件算法：从 `w+1` 列向后扫描；邻接表则先在链中找到 `w`，不存在时返回 `-1`。
- **邻接顺序不同**：矩阵按顶点下标递增扫描；补写的邻接表插边使用头插法，因而通常按插边的逆序枚举。第 40 页只给出链表示意图，没有规定插入顺序，不要求示例头插后的链顺序与原图相同。
- 保留第 27 页 `GraphEmpty()` 的原语义：**没有边**。同时提供第 23 页的 `IsEmpty()`：**没有顶点**。只有孤立顶点的图前者为 `true`、后者为 `false`；计数使用 `NumberOfVertices()` / `NumberOfEdges()`。

两类均持有手写动态数组/链表，不使用 STL 容器替代核心存储。`std::unique_ptr` 只用于分配期间的异常清理；持久存储仍是课件的 `T*`、`E**`、`Vertex<T,E>*` 和边链表。基类析构为虚函数，允许经 `Graph<T,E>*` 释放派生图。禁止复制/赋值，避免课件裸指针被默认浅拷贝后重复释放；本次不增加深拷贝或移动功能。

```cpp
#include "graph/include/Graphmtx.h"
#include "graph/include/Graphlnk.h"
#include <string>

Graphmtx<std::string, int> graph(4); // 换成 Graphlnk 使用相同接口
graph.insertVertex("A"); // 0
graph.insertVertex("B"); // 1
graph.insertVertex("C"); // 2
graph.insertEdge(0, 1, 5);
graph.insertEdge(0, 2, 8);
for (int neighbor = graph.getFirstNeighbor(0); neighbor != -1;
     neighbor = graph.getNextNeighbor(0, neighbor)) {
    auto name = graph.getValue(neighbor);
    auto cost = graph.getWeight(0, neighbor);
    // 使用 name 和 cost；这是邻接点枚举，不是 DFS/BFS。
}
graph.removeVertex(1); // B 被删除，C 移到下标 1，A-C 权仍为 8
```

矩阵占用 O(capacity²) 空间，取权 O(1)，一次邻接点查找最多扫描 O(n) 列。邻接表占用 O(capacity+m) 空间，取权与下一邻接点查找 O(degree(v))；反复调用课件的 `getNextNeighbor` 会从链头重新搜索，枚举整个邻接表最坏 O(degree(v)²)。补写的邻接表删点先删除关联边，再扫描剩余边修正编号；不将这些接口当作额外的图遍历算法。

## 流操作（补充格式）

输入和输出格式相同：**顶点数 边数**，随后依次给出顶点值，再给出每条无向边的 **起点下标 终点下标 权值**。例如第 40 页的无权图可写为：

```text
4 3
A B C D
0 1 1
0 3 1
1 2 1
```

输入成功后替换原图，沿用构造时的容量和模式。负数量、超容量、重复顶点或边、非法边以及数据不足会设置或保留流失败状态，原图保留。普通字符串以空白分隔。输出逐条记录无向边或有向弧。读回时用相同模式构造图，顶点与边权可以还原，邻接链顺序可能变化。

## 课件勘误与必要修正

1. 第 22～23 页为非模板 ADT 示意，其中 `getWeight` 返回 `T`，第 25～28 页明确使用边权类型 `E`；实现以正式模板基类为准。第 28 页按值、第 46 页按引用的 `insertVertex` 统一为 `const T&`，保证正确覆盖；只读接口补 `const`。
2. 第 26 页 `maxWeight = ……` 没有具体数值，补为上述哨兵。第 27～28 页的基类虚函数没有定义，改为纯虚接口，析构设为虚函数；`getVertexPos` 同样设为纯虚函数，part2 更新时公开以供类外算法使用。
3. 模板派生类对基类成员的未限定访问不能直接编译，使用 `using Graph<T,E>::...`；修正全角问号/析构符号及 `Vertex`/`vertex`、`NumVertices`/`numVertices` 等大小写笔误。
4. 第 34 页 `i <= numVertices` 越过有效末尾，第 34、37～38、49～50 页只检查下标不为 `-1` 也不够；补充完整下标检查。`getValue` 的 `NULL`/`0` 返回与一般 `T` 不兼容，改为异常。
5. 第 36 页的 `int**`/`new int[]` 与 `E** Edge` 矛盾，按 `E` 分配。第 34 页析构只删外层 `Edge` 导致各行泄漏，改为先逐行释放；构造中途分配失败也释放已经分配的存储。
6. 第 43 页 `Edge` 声明的是 `cost`，初始化列表却写 `weight(cost)`；统一初始化 `cost`。默认边和顶点结点补齐初始化，避免未初始化指针。
7. 增删算法、边权范围、重复值处理、删点末项填补、邻接表头插和流格式均为本仓库补充约定，**不是这份课件已有的函数体**。邻接表插边先分配两个结点再挂接，避免分配失败时留下单方向边。

## 构建与验证

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target course_code chapter8_part1_example
./cmake-build-debug/chapter8_part1_example
```

模板由独立目标 `chapter8_part1_example` 实例化检查，示例 `.cpp` 不进入 `course_code`。自检使用始终执行的条件检查，Release 模式也不会省略增删操作。

2026-11-02 实际验证：

- 使用 CLion 附带的 CMake，在 `/private/tmp/course-lesson8-part1-build` 全新配置，构建 `course_code`、`chapter8_part1_example` 并成功运行示例。
- 示例以 C++20、`-Wall -Wextra -pedantic`、AddressSanitizer、UndefinedBehaviorSanitizer 编译运行通过；另以 `-DNDEBUG -O2` 编译运行通过。
- 临时随机测试在容量 0、1、2、8、17 上执行共 **30,000 次**混合插入/删除。两种图逐步与独立的“顶点值对 → 权值”参考映射核对顶点、边数、所有边权和邻接枚举，通过 ASan/UBSan；标准库容器仅作测试参照。
- 示例覆盖空图/满容量、非法下标、重复顶点/边、自环和非法权值、删中间/末顶点、槽位复用、浮点权、输入失败保留原图、流往返与基类释放。未模拟内存耗尽或自定义顶点复制抛异常；本次不承诺用户自定义 `T` 抛异常时增删操作的事务保证。

临时测试、课件渲染和构建产物放在 `/private/tmp`，仓库保留可重跑的课件示例。

## 第二部分：8.3 遍历与连通性、8.4 最小生成树

DFS 和 BFS 在有向模式下沿出弧访问。课件中的 `Components`、`Kruskal` 和 `Prim` 面向无向图，传入有向图会抛出 `std::invalid_argument`。

| 文件/接口 | 完整课件页码 | 来源与处理 |
| --- | --- | --- |
| `include/GraphTraversal.h`：`DFS` 两个重载 | 56～57 | 按课件递归算法，分别以顶点值/顶点下标为入口，补起点检查和自动资源释放 |
| 同上 `BFS` | 61～62 | 按课件队列分层算法，复用 `LinkedQueue<int>`，入队时标记 |
| 同上 `Components` | 63～65 | 共用 visited，逐个扫描未访问顶点并执行 DFS，每个分量输出一行 |
| `include/MinSpanTree.h` | 75～76 | 保留 `MSTEdgeNode<T,E>`、`MinSpanTree<T,E>` 和手写边数组，补全 `Insert`、析构与只读检查接口 |
| `include/MinimumSpanningTree.h`：`Kruskal` | 70～80 | 保留最小堆选边、并查集判环，补堆耗尽判断 |
| 同上 `Prim` | 81～90 | 保留候选边最小堆、`Vmst` 标记思路，补起点检查、堆耗尽退出和资源释放 |
| `examples/chapter8_part2.cpp` | 73～74、80、83～87、93～94 | 两种 MST 选边顺序及边界自检，包含最终版遍历和生成树作业 |

### 遍历接口与练习

保留课件调用形式 `DFS(graph,startValue)`、`BFS(graph,startValue)`、`Components(graph)`，默认向 `std::cout` 输出；补充可选的最后一个 `std::ostream&` 参数，便于保存结果和自检。访问每个顶点时输出 `value` 加空格，`Components` 在每个分量后换行，不额外输出分量编号。

- **主入口的起点是顶点值，不是下标**。即使 `T=int`，`DFS(graph,20)`、`Prim(graph,20,mst)` 也按值定位 `20`；顶点不存在（包括空图）抛出 `std::invalid_argument`。
- `DFS(graph,vertexIndex,visited)` 是课件的递归重载；调用方需提供至少 `NumberOfVertices()` 个已初始化的布尔标记。非法下标抛出 `std::out_of_range`，空指针抛出 `std::invalid_argument`。已访问的起点直接返回。
- DFS/BFS 仅访问起点所在的连通分量；`Components` 从下标 0 开始扫描所有顶点，包含孤立顶点。空图的 `Components` 无输出。
- 访问顺序遵循 `getFirstNeighbor`/`getNextNeighbor`；调用期间不增删图。DFS 采用课件递归，最坏递归深度 O(n)，大规模深链图可能受调用栈限制。

第 93 页作业（原 part2 第 66 页）按 `A=0,...,G=6` 编号，边为 `AB,AC,AD,AE,BG,CE,DF,EF,FG`。按题目“编号小者优先”：

| 操作 | 顶点访问顺序 | 生成树边（父顶点 → 新访问顶点） |
| --- | --- | --- |
| 从 A 开始 DFS | `A B G F D E C` | `A→B, B→G, G→F, F→D, F→E, E→C` |
| 从 E 开始 BFS | `E A C F B D G` | `E→A, E→C, E→F, A→B, A→D, F→G` |

示例对两种存储都检查此序列。矩阵天然按编号枚举；邻接表通过逆序插入边得到递增邻接顺序，不改写其头插存储。以上生成树边是对访问过程的说明，当前遍历函数不另建树对象。

### 最小生成树接口

`MSTEdgeNode<T,E>` 的 `tail/head` 是原图下标，`cost` 是边权。`MinSpanTree(capacity=DefaultVertices-1)` 只保存边数组，负容量抛出 `std::invalid_argument`，容量 0 合法。`Insert(edge)` 成功返回 0-based 存储下标，满时返回 `-1`；它只负责存储，不自行验证成环或连通。补充 `Size()`、`Capacity()`、`getEdge(index)` 与 `Clear()` 供算法和学习检查，非法读取下标抛出 `std::out_of_range`。类持有动态数组，禁止浅拷贝。

```cpp
#include "graph/include/Graphmtx.h"
#include "graph/include/GraphTraversal.h"
#include "graph/include/MinimumSpanningTree.h"

Graphmtx<char, int> graph(3);
graph.insertVertex('A');
graph.insertVertex('B');
graph.insertVertex('C');
graph.insertEdge(0, 1, 5);
graph.insertEdge(1, 2, 2);
graph.insertEdge(0, 2, 9);
DFS(graph, 'A');
BFS(graph, 'A');
MinSpanTree<char, int> mst(2);
Kruskal(graph, mst); // 选 BC、AB，总权 7
Prim(graph, 'A', mst); // 选 AB、BC，总权 7；替换前次结果
```

算法复用第五章 `MinHeap<E,MSTEdgeNode<T,E>>`：`Element.key` 存边权，`Element.other` 存完整树边，保证按 cost 比较；不为本章重写一套单模板参数堆。Kruskal 还复用第六章 `UFSets`，调用 `Find` 与 `Union`，因此使用它的可执行目标必须链接 `set/src/UFSets.cpp`。Prim 保持课件的堆实现，没有换成 `lowcost[]` 的另一版本。

两个算法的容量校验均要求至少 `n-1` 条边，校验失败抛出 `std::length_error`；Prim 起点不存在抛出 `std::invalid_argument`，这些校验发生在清空旧结果之前。校验通过后从空结果重新构造，防止重复调用时追加出多棵树。

图不连通时，两者均在候选堆耗尽后抛出 `std::runtime_error`，不会读取无效边或无限循环。**异常后的结果不是完整 MST**：Kruskal 保留已选出的森林边，Prim 保留起点分量内已选出的树边；调用者捕获异常后不能将它当成全图生成树。单顶点的 MST 为 0 条边；空图的 Kruskal 返回空结果，空图 Prim 因无有效起点而报错。

课件第 80 页 Kruskal 的选中顺序为 `(0,5,10),(2,3,12),(1,6,14),(1,2,16),(3,4,22),(4,5,25)`，其中 `(3,6,18)`、`(4,6,24)` 成环被舍弃；第 87 页 Prim 从 0 出发为 `(0,5,10),(5,4,25),(4,3,22),(3,2,12),(2,1,16),(1,6,14)`。两者总权均为 **99**。等权边不规定唯一的选中顺序，只保证最小总权与树结构。

沿用 part1 的正权范围及无边哨兵。MST 类不另设累计权值字段，示例/调用者按需累加；权值和可能大于 `maxWeight` 或 `int` 上限，应选择足够宽的累计类型。图删点后编号可能变化，已有 MST 中的端点下标也应重新计算。

### 第 94 页生成树作业

第 2 题矩阵按 `v1,...,v5` 排列。对角线的 ∞ 表示无自环，建图时只将上三角的有限非对角权作为无向边录入。顶点值用 `1,...,5` 表示，存储下标用 `0,...,4` 表示，调用 `Prim(graph,1,mst)` 从顶点值 v1 出发。

| 次序 | 选中的边 | 当前总权 |
| --- | --- | --- |
| 1 | `(v1,v2,1)` | 1 |
| 2 | `(v1,v4,5)` | 6 |
| 3 | `(v4,v5,4)` | 10 |
| 4 | `(v5,v3,2)` | **12** |

第 3 题图中顶点标号为 `1,...,7`。Kruskal 最终选边为 `(1,2,1)`、`(2,3,2)`、`(4,5,2)`、`(2,5,3)`、`(4,6,3)`、`(5,7,4)`，总权 **15**。两条权为 2 的边和两条权为 3 的边各自的出堆先后可不同，示例按无向边集合、连通性和总权验证。两道题均在矩阵与邻接表上自检。

### 遍历与生成树勘误、复杂度说明

1. 第 26、33、45 页的 `getVertexPos` 为保护/私有成员，但第 56、61、88 页的类外函数调用它；本次公开此接口并保留虚调用。第 56 页“从顶点 0 开始”的注释与 `loc` 不符，实现从找到的起点开始。
2. 第 56 页主过程调用了第 57 页才定义的递归重载，实现先定义递归重载，避免模板查找问题；补充缺失起点和递归参数检查。辅助数组使用 `unique_ptr<bool[]>` 值初始化，输出或队列分配抛异常时也会释放。
3. 第 61 页 `Queue<int>` 是仓库中的抽象接口，改用已实现的 `LinkedQueue<int>`；修正弯引号。第 65 页 `OutputNewComponent()` 未提供定义，这里用换行分隔分量。
4. 第 75 页的 `FLOAT_MAX`/`maxValue` 与邻接矩阵的 `maxWeight` 哨兵不同；第 78 页若直接比较会把不存在的边插入堆。统一依据图存储的 `maxWeight` 识别无边，不额外引入第二个哨兵。
5. 第 76 页 `DefaultSize` 未定义、`MaxSize` 与字段大小写不一致、`MSTEdgeNode` 缺模板参数，均修正；补齐原来仅声明的 `Insert` 和遗漏的析构。检查和清空接口为仓库补充。
6. 第 78、88 页用的单模板参数 `MinHeap<MSTEdgeNode<...>>` 与第五章课件/仓库堆接口不同，且边结构本身没有比较运算，改用现有堆的关键码/附加数据设计。第 80 页并查集图示按集合大小合并，而第 79 页代码写 `Union`；这里跟随代码的普通 `Union`，不要求中间 parent 数组与图示相同。
7. 第 79 页未检查堆是否为空，第 89～90 页在断开图上可能反复进入空堆循环。补全耗尽退出与不连通异常；第 88 页重复声明 `u`、第 89 页 `bool Vmst = new bool[n]` 缺 `*`、标记数组未释放也一并修正。
8. 第 91 页稠密/稀疏图的适用性是一般比较；本次 Prim 是课件的**边堆版本**，不能直接标作 `lowcost[]` 版本的 O(n²)。当前 Kruskal 保留全顶点对扫描，矩阵下约为 O(n² + m log m + m·h)，`h` 是普通 Union 的并查集高度；链表的逐对取权及 `getNextNeighbor` 从链头重查还会带来额外扫描。DFS/BFS 在矩阵下 O(n²)，当前链表接口下最多 O(n + Σdegree(v)²)，并非直接 O(n+m)。

### part2 构建与实际验证（2026-11-05）

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target course_code chapter8_part1_example chapter8_part2_example
./cmake-build-debug/chapter8_part1_example
./cmake-build-debug/chapter8_part2_example
```

- 使用 CLion 附带 CMake 在 `/private/tmp/course-lesson8-part2-build` 全新配置，以上目标编译、两个示例运行均通过。新目标显式链接 `set/src/UFSets.cpp`，队列和堆为头文件模板。
- part2 示例以 C++20、`-Wall -Wextra -pedantic`、ASan/UBSan 编译运行通过；另以 `-DNDEBUG -O2` 编译运行通过。
- 保留自检覆盖两种存储的 part2 第 66 页遍历顺序、多个分量/孤立点、整数顶点值与下标的区别、无效起点、递归参数检查、课件两种 MST 选边顺序、Prim 不同起点、重复调用、断开图/过期候选边、单点/空图、容量不足、等权边和浮点权。
- 临时测试生成 **720 个 1～6 顶点的小图**，通过穷举边子集独立求出最小生成树总权；两种存储上的 Kruskal 和各起点 Prim 共 **6,480 次运行**均与穷举一致，通过 ASan/UBSan，包含断开图、等权边和累计权值超过 `int` 范围的情况。

## 第三部分 8.5 最短路径与 8.6 活动网络

| 文件 | 完整课件页码 | 实现 |
| --- | --- | --- |
| `include/ShortestPath.h` | 97～108、114～120 | `ShortestPath` 使用 Dijkstra 线性选点，`Floyd` 按中间顶点三层递推 |
| `include/ActivityNetwork.h` | 129～141、147～156 | `TopologicalSort` 复用入度数组作为栈，`CriticalPath` 正反拓扑递推 |
| `examples/chapter8_part3.cpp` | 98～108、109、116、132～137、151～152 | 两种图存储的课件算例与边界检查 |

### 最短路径

- `ShortestPath(graph, source, dist, path)` 的源点用 **0-based 下标**，与第 106 页函数体访问数组的方式一致。调用方提供两个长度至少为顶点数的数组。负权边导致 `std::invalid_argument`，剩余顶点均不可达时提前结束。相同距离保留原前驱。
- `Floyd(graph, a, path)` 接收两组行指针。每组有 `n` 行，每行至少 `n` 项。负边可参与松弛，遇到负对角线就抛出 `std::domain_error`，此时输出矩阵可能只完成部分更新。
- `dist` 和 `a` 的元素为 `GraphDistance<E>`。整数权使用 `long long`，浮点权使用 `long double`。图存储的无边哨兵为 `maxWeight`，路径不可达使用 `unreachableDistance<E>`。先判断可达再相加，累计距离可以超过单边权上界。
- `path[to]` 和 `path[from][to]` 记录终点的直接前驱，下标 `-1` 表示源点自身或不可达。可达时从终点沿前驱回溯到源点，再反向读取路径。第 108 页从 0 到 4 的路径为 `0→3→2→4`，长度 60。第 119 页从 1 到 0 的路径为 `1→3→2→0`，长度 11。
- 第 109 页练习按 `a,b,c,d,e,f,h` 排列，距离为 `0,1,6,9,10,2,13`。到 h 的等长路径可经 `c→h` 或 `c→e→h`。

第 106～107 页的下标类型和 `GetWeight` 大小写已统一。第 117 页的二维数组声明改为可编译的行指针接口，并将前驱哨兵从 0 改为 -1，以区分合法顶点 0。两种算法分别保留矩阵下 O(n²) 和 O(n³) 的主循环，邻接表的按下标取权可能增加扫描开销。第 96 页标记不讲的 Bellman–Ford 和第 110～113 页的进展补充属于阅读材料。

### 最短路径过程表

第 98～108 页 Dijkstra 算例以 0 为源点。表中列出每次确定最近顶点并完成松弛后的状态，`path` 为终点的直接前驱。源点始终满足 `dist[0]=0`、`path[0]=-1`，其余四个顶点的状态如下。

| 阶段 | 已确定最短距离的顶点集合 S | `dist[1],dist[2],dist[3],dist[4]` | `path[1],path[2],path[3],path[4]` |
| --- | --- | --- | --- |
| 从源点初始化 | `{0}` | `10,∞,30,100` | `0,-1,0,0` |
| 选中 1，松弛 `1→2` | `{0,1}` | `10,60,30,100` | `0,1,0,0` |
| 选中 3，松弛 `3→2`、`3→4` | `{0,1,3}` | `10,50,30,90` | `0,3,0,3` |
| 选中 2，松弛 `2→4` | `{0,1,2,3}` | `10,50,30,60` | `0,3,0,2` |
| 选中 4 | `{0,1,2,3,4}` | `10,50,30,60` | `0,3,0,2` |

第 116 页 Floyd 算例用 `A(-1)` 表示初始距离矩阵，用 `A(k)` 表示允许顶点 `0,...,k` 作为中间点后的矩阵。每格按列下标 `0,1,2,3` 列出一整行。

| 阶段 | 第 0 行 | 第 1 行 | 第 2 行 | 第 3 行 |
| --- | --- | --- | --- | --- |
| `A(-1)` | `0,1,∞,4` | `∞,0,9,2` | `3,5,0,8` | `∞,∞,6,0` |
| `A(0)` | `0,1,∞,4` | `∞,0,9,2` | `3,4,0,7` | `∞,∞,6,0` |
| `A(1)` | `0,1,10,3` | `∞,0,9,2` | `3,4,0,6` | `∞,∞,6,0` |
| `A(2)` | `0,1,10,3` | `12,0,9,2` | `3,4,0,6` | `9,10,6,0` |
| `A(3)` | `0,1,9,3` | `11,0,8,2` | `3,4,0,6` | `9,10,6,0` |

松弛 `i→k→j` 成功时，前驱更新为 `path[i][j]=path[k][j]`。例如 `A(2)` 中 `1→0` 长度为 12，到 `A(3)` 时经 `1→3→2→0` 缩短为 11，直接前驱仍是顶点 2。第 119 页的回溯依次读取 `path[1][0]=2`、`path[1][2]=3`、`path[1][3]=1`。

### 活动网络

- `TopologicalSort(graph, output)` 在已建好的有向图上统计入度，保留第 135 页的 `count[]` 栈链接。每行输出一个顶点值。存在环时输出已排出的前缀和提示，返回 `false`。第 132 页例图的输出为 `C4,C0,C3,C2,C1,C5`，一般顺序受邻接枚举影响。
- `CriticalPath(graph, output)` 返回工期并逐行输出关键活动，两种接口的输出流默认是 `std::cout`。先计算拓扑序，允许按任意顺序录入事件。有环抛出 `std::domain_error`，负持续时间抛出 `std::invalid_argument`。空有向图的工期为 0。
- 正向用 `Ve[next] = max(Ve[next], Ve[current] + dur)`，逆向用 `Vl[current] = min(Vl[current], Vl[next] - dur)`。最终版第 154 页已将全部 `Vl` 初始化为工期，并将逆向循环改为 `j >= 0`，修正预习版未初始化与漏算下标 0 的问题。仓库实现已包含这两处修正，并通过逆拓扑序处理任意事件编号。随后按 `e = Ve[tail]`、`l = Vl[head] - dur` 判定关键活动。
- 多源的事件从时刻 0 开始，多汇共用最晚完成时刻。整数时间精确比较，浮点时间按 `4 × epsilon × 工期 × 顶点数` 容许舍入误差，该范围内的微小余量也会归为关键。
- 第 151～152 页工期是 **58**，`4→6` 有 10 个单位余量，其余 9 条活动为关键。三条关键路径为 `1→2→4→5→7→8`、`1→3→4→5→7→8`、`1→3→6→7→8`。第 157 页不正确的说法为 B、D。

第 138 页的读边操作由图存储负责，排序从已建好的图计算入度。第 141 页的 O(n+m) 假设一次顺序扫描能遍历一条出边链。当前通用接口下，矩阵扫描 O(n²)，邻接表的 `getNextNeighbor` 会从链头重查，最坏为 O(n + Σ出度²)。

### 预习完整版历史验证（2026-11-10）

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target course_code chapter8_part1_example chapter8_part2_example chapter8_part3_example
./cmake-build-debug/chapter8_part3_example
```

使用 CLion 附带的 CMake 构建 Release 版 `course_code` 及三个示例目标，全部运行通过。part3 还通过 ASan/UBSan，检查了不可达顶点、零权和负权弧、负权环、超过 `int` 的累计距离、有向图删点和流往返、AOV 有环、非拓扑编号的 AOE 以及多汇点和浮点工期。

### 最终版更新与验证（2026-11-20）

本次同步了 158 页最终课件的来源、正文与代码注释页码，将第 93 页遍历练习标为正式作业，补入第 94 页两道生成树作业，并更新第 154 页已修正问题的版本说明。Dijkstra 的逐轮距离、前驱和 Floyd 的各阶段距离矩阵见上方过程表。

- 使用 CLion 附带 CMake 在 `/private/tmp/course-lesson8-final-20261120-build` 全新配置 Release 构建，`course_code` 和第八章三个示例目标编译通过，三个示例均运行成功。
- `chapter8_part2.cpp` 与 `chapter8_part3.cpp` 以 C++20、`-Wall -Wextra -pedantic`、ASan/UBSan 编译运行通过。新增 Prim/Kruskal 作业在两种存储下分别得到总权 12、15，并核对树结构及选边。
- Floyd 过程表通过临时的独立矩阵递推核对。早前 30,000 次随机图存储测试和 720 个小图穷举属于上方历史记录，保留原验证日期。
