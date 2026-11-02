# 第七章预习：静态搜索结构

**依据老师发布的 `Lesson 7 (2026秋季)-Beibei Wang.pdf`（101 页），已于 2026-10-26 按今年预习课件逐项核对。** 覆盖范围是课件提供的预习内容，不表示课堂已经讲授完毕。

下文页码均指今年 PDF 页序。正文完整覆盖 **7.1（第 4～25 页）、7.2（第 27～51 页）、7.3（第 53～101 页）**。7.2/7.3 按主题放在 [tree/](../tree/README.md#第七章预习二叉搜索树与-avl-树)。第 101 页仅提及 B/B+ 树，没有展开，本次不实现。

## 与去年预习版的核对结果

- 今年第 2 页新增日常搜索场景，第 17 页新增“为何使用监视哨”的思考；第 63、66 页单旋代码页调整了配图，类接口及代码没有算法变化。
- 去年第 2～100 页的正文对应今年第 3～101 页。已核对数据表设计、搜索返回约定、BST 后继替补、AVL 的 bf 符号、四种旋转与插入/删除回溯规则，现有实现与今年版本一致；本次更新来源、页码及说明，核心算法无需改写。
- 今年仍有下标断言、监视哨空间、插入参数及递归返回等代码问题，下方勘误继续适用。折半搜索只有规则/判定树，AVL 增删只有步骤与图例，仓库补全的函数体仍明确标为补充。
- 今年 PDF 未包含去年的第 101～102 页课下作业。示例保留 **2025 第 101 页**的 AVL 连续删除，明确列为往年补充练习，不把它标成今年作业。

## 文件与课件对应

| 文件 | 范围 | 来源 |
| --- | --- | --- |
| `include/dataList.h` | 第 8～14 页 `dataNode<E,K>`、`dataList<E,K>` | 保留数组数据表设计，修正插入参数和监视哨空间，补全复制、流操作 |
| 同上 `SeqSearch(key)` | 第 16～17 页 | 课件监视哨算法 |
| 同上 `SeqSearch(key,position)` | 第 19～20 页 | 课件递归顺序搜索，补齐声明和起始序号检查 |
| 同上 `BinarySearch(key)` | 第 25 页折半判定树 | 根据规则补写，课件此处没有函数体 |
| `../tree/include/BST.h` | 7.2，第 29～51 页 | 结点、搜索、插入、删除按课件，详见树目录 README |
| `../tree/include/AVLTree.h` | 7.3，第 55～99 页 | 四种旋转按课件代码；增删按文字与图例补全 |
| `examples/chapter7.cpp` | 全章示例和自检 | 监视哨边界、BST 建树、四种旋转、AVL 插入序列、删除特例和复制；另保留 2025 第 101 页连续删除练习 |
| `src/README.md` | 目录说明 | 模板实现全在头文件，无额外普通 `.cpp` |

## 数据表接口与下标

`dataNode<E,K>` 的 `K key` 是关键码，`E other` 是附加数据。与 BST/AVL 中“E 是含 key 的完整记录”的含义不同。

- `dataList(capacity=100)`：手写数组，固定容量。分配 `capacity+1` 个槽，末尾额外槽始终可用于监视哨；容量 0 合法。负容量或 `INT_MAX` 抛出 `std::invalid_argument`，后者不能安全表示额外槽及失败序号。
- `Insert(dataNode<E,K>)` 或补充的 `Insert(key,payload)`：尾部插入，满表返回 `false`。课件没有去重算法，因此允许重复关键码；顺序搜索返回首次匹配。
- `Remove(key,payload)`：删除首次匹配，通过引用返回附加数据，用尾部**整个结点**填补空位，因此不保序。未找到返回 `false`，不改变输出参数。
- `Length()`：有效数据项数，不含监视哨。`getKey(position)`、`setKey(key,position)`、补充的 `getData(position)` 均使用 **1-based 序号**，越界抛出 `std::out_of_range`。`getData` 只读，`getOther()` 读取附加数据。
- `SeqSearch(key)`：成功返回 **1～Length()**；失败返回 **Length()+1**。保留课件监视哨写入，因此即便声明 `const`，也会改写备用槽，不应在同一对象上并发调用。
- `SeqSearch(key,position)`：从给定 1-based 序号递归向后搜索；成功返回序号，失败返回 **0**。小于 1 的起点抛出 `std::out_of_range`，大于长度返回 0；递归深度 O(n)。
- `BinarySearch(key)`：要求表按关键码非递减；成功返回 **1-based 序号**，失败返回 **0**。不检查有序性或自动排序；重复关键码返回其中一个。`setKey`、尾部插入及尾项填补删除后，应先确保有序再使用它。

不要混用今年第六章 `SortedList` 的约定：它的搜索是 **0-based，失败 -1**。

```cpp
#include "search/include/dataList.h"
#include <string>

dataList<std::string, int> table(3);
table.Insert(10, "A");
table.Insert(20, "B");
table.Insert(30, "C");
int position = table.SeqSearch(20); // 2
if (position <= table.Length()) {
    auto payload = table.getData(position).getOther(); // "B"
}
int missing = table.SeqSearch(99);    // 4，满表也有监视哨空间
int recursive = table.SeqSearch(99, 1); // 0
int binary = table.BinarySearch(20); // 2，前提是有序
```

复制构造和赋值为深拷贝。K/E 需要默认构造和复制，K 需要相应的相等与大小比较；调用流操作时需支持 `>>`/`<<`。`std::unique_ptr` 只用于复制时的异常清理，核心存储与搜索仍是手写数组算法。

课件仅声明流操作，没有定义格式。补充输入为 **数量，随后各项 `key other`**，例如 `3 10 A 20 B 30 C`，成功后替换原表；负数量、超容量或输入不足设置/保留流失败状态并保留原表。字符串属性以空白分隔。输出逐行 `key:other`，用于阅读，不与输入格式对称。

今年第 17 页“为何使用监视哨”：末尾放入目标关键码后，循环总能遇到匹配项，每轮只需判等，不必再检查是否到达表尾；结束后通过返回序号是否大于长度区分失败。时间复杂度仍为 O(n)，独立的额外槽保证满表时也不会越界。

## 课件勘误与补充

1. 第 9 页只有带关键码的构造函数，但第 10 页却直接创建结点数组；补默认构造和字段初始化。`CuurentSize` 统一为 `CurrentSize`。
2. 第 10 页仅分配 `sz` 项，第 17 页访问 `Element[CurrentSize]`，满表越界；增加独立监视哨槽，有效容量仍为 sz。
3. 第 11 页下标断言使用 `i>0 || i<=CurrentSize`，应为同时满足两个条件；改为实际边界检查。
4. 第 13 页 `Insert(E&)` 把附加数据 E 赋给 `dataNode<E,K>`，与第 9、14 页设计不一致。保留关键码/附加域分离的设计，将该参数改为完整结点，并加 `(key,payload)` 便捷入口；没有为 E 猜测或隐式生成关键码。
5. 第 14 页 `for` 后的分号与循环变量作用域矛盾；改为显式扫描，保留尾项填补算法。
6. 第 18 页只传一个模板参数、`Seqsearch` 大小写及成功后再加 1 都与类定义不符；示例用两个模板参数、`SeqSearch`，直接使用已返回的序号。
7. 第 20 页递归重载未列入类声明，现已补齐；其失败为 0 的约定按原代码保留，不强行与监视哨版统一。

第 21～25 页的 ASL 分析保留在课件中对照：等概率监视哨顺序搜索成功 ASL 为 `(n+1)/2`，失败为 `n+1`；第 25 页六个元素的折半搜索成功为 `14/6`，失败为 `20/7`。折半比较次数按访问的关键码结点计，不把 C++ 的每个关系运算符都单独计数。未另外设计 ASL 统计框架。

## 构建与验证

全章模板自检目标为 `chapter7_example`；`examples/` 中的 `.cpp` 不进入 `course_code`，但会由该独立 executable 实例化新模板。

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target course_code chapter7_example
./cmake-build-debug/chapter7_example
```

自检使用始终执行的条件检查，Release 构建也不会跳过增删。实际验证记录见下方更新说明。

2026-10-26 本次更新验证通过：

- 使用 CLion 附带 CMake 在 `/tmp/course-lesson7-2026-build` 重新配置并构建 `course_code`、`chapter7_example`，示例运行成功。
- 全章示例以 C++20、`-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer 编译运行通过，包含满表监视哨、BST 后继替补、四种 AVL 插入旋转、删除特殊单旋、课件建树序列、复制和基类接口检查。
- 此次核心算法行为未变，更新了今年课件的来源与页码、监视哨思考说明、往年作业的补充标记；未重复运行下方历史随机/排列测试，也未新增课件范围外的数据结构。

2026-10-22（2025 预习版历史验证，以下随机与排列测试未在本次重跑）：

- 使用 CLion 附带的 CMake 4.3.1，在 `/tmp/course-lesson7-build` 配置并构建 `course_code`、`chapter7_example`，运行示例成功。
- 新示例通过 C++20 `-Wall -Wextra -pedantic` 编译及 AddressSanitizer、UndefinedBehaviorSanitizer 运行检查；另以 `-DNDEBUG -O2` 验证检查仍然执行。
- 临时测试对 BST/AVL 进行 30,000 次随机混合插入、删除、搜索，与 `std::map` 对照；逐步核对结点数量、严格次序、完整属性、最值、树高与每个 bf。标准库容器仅作测试参照，没有进入核心实现。
- 穷举六个结点的 720 种插入排列，每种采用六个循环删除顺序（4,320 组），每步检查 AVL 结构和平衡因子。另验证深拷贝、赋值、自赋值、经 BST 引用赋值/增删及清空后的独立性。
- 静态表检查容量 0/1/2/17/100，包含满表监视哨、成功/失败搜索、非法序号和负容量；示例还验证尾项填补的完整属性、输入失败保留原表。2025 课件第 101 页原图的删除 22、3、10、9 通过结构和平衡检查。

临时测试和构建产物放在 `/tmp`，仓库保留 `chapter7_example` 便于复习重跑。今年课件的核对结果见上方版本说明。
