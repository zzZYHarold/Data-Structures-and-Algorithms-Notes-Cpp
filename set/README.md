# 第六章：集合与并查集（预习版）

依据用户本次指定的 **`Lesson 6 (2025秋季)-BeibeiWang-final.pdf`**（101 页）整理。当前是 **2025 秋课件的课前预习实现，待 2026 秋正式课件核对**。本文页码均为 PDF 页序。

正文确实展开了全章：6.1 为第 5～23 页，6.2 为第 25～48 页；6.3、6.4 见 [dictionary/README.md](../dictionary/README.md)。第 1～2 页是引入示例，第 3～4、24、49、66 页是章节目录，不作为额外实现依据。

## 文件与课件对应

| 文件 | 课件内容 | 实现来源 |
| --- | --- | --- |
| `include/bitSet.h` | 6.1，第 10～19 页，16 位位向量集合 | 保留成员位映射及子集判断思路；构造、复制、增删、集合运算、输入输出按声明/规则补全 |
| `include/LinkedSet.h` | 6.1，第 20～23 页，有序链表集合 | 课件给出结点及类声明；函数体按有序链表补全 |
| `include/UFSets.h`、`src/UFSets.cpp` | 6.2，第 31～47 页，双亲数组并查集 | 构造、递归 `Find`、`Union`、`WeightedUnion` 对应课件代码；按高度合并及路径压缩按图示补全 |
| `include/SetApplications.h`、`src/SetApplications.cpp` | 第 25、48 页，省份问题 | 课件只有题目，按并查集思路补写 |

模板实现放在头文件，普通实现放在 `.cpp`；集合和并查集的核心存储均为手写数组/链表。

## 位向量集合 `bitSet<T>`

`T` 是整数类型，通常使用 `int`。构造参数 `sz` 是全集大小，可取成员为 **0～sz-1**。每个 `std::uint16_t` 保存 16 位，成员 `x` 位于数组 `x/16` 的第 `15-x%16` 位。未使用的尾部位始终保持 0，`getMember()` 返回 0/1，`putMember(x,v)` 只接受 0/1。

```cpp
#include "set/include/bitSet.h"

bitSet<int> leftSet(50), rightSet(50);
for (int member = 0; member < 10; ++member) {
    leftSet.addMember(member);
    rightSet.addMember(member + 7);
}
auto joined = leftSet + rightSet;       // {0,...,16}
auto common = leftSet * rightSet;       // {7,8,9}
auto difference = leftSet - rightSet;   // {0,...,6}
bool contained = common.subSet(leftSet); // true，调用对象是被判断的子集
```

重复添加、删除不存在的成员返回 `false`。越界成员抛出 `std::out_of_range`；负的全集大小或非法位值抛出 `std::invalid_argument`。并、交、差和子集比较要求全集大小一致，否则抛出 `std::invalid_argument`；相等比较在全集大小不同时返回 `false`。零大小全集合法。

课件未规定流输入格式，补充约定为 **成员数后跟各成员**，例如 `4 0 15 16 49`。输入替换原集合，重复成员自动去重；负数量、越界或数据不足设置流失败状态，保留原集合。输出为 `{0,15,16,49}`，与输入格式不同。复制构造与赋值采用深拷贝。

## 有序链表集合 `LinkedSet<T>`

保留 `SetNode<T>`、带头结点的 `first` 和尾指针 `last`，成员严格升序且唯一。`T` 需要默认构造、复制、`<` 和 `==`，这两个比较应一致。

- `addMember` / `delMember` / `Contains`：有序扫描；增删成功返回 `true`，重复或不存在返回 `false`。
- `+` / `*` / `-`：双指针归并实现并、交、差，时间为 O(m+n)，结果不改变操作数。
- `subSet(other)`：判断当前集合是否为 `other` 的子集；空集是任意集合的子集。
- `Min(value)` / `Max(value)`：空集返回 `false`，非空时写入结果；维护尾指针使 `Max` 为 O(1)。
- `makeEmpty`、复制构造、赋值和析构均管理真实结点，复制后两个对象互不影响。

## 并查集 `UFSets`

成员编号为 **0～Size()-1**。`parent[root]` 始终是**负的集合大小**；非根的值是双亲下标。保留以下教学顺序：

1. `Find(member)`：课件的递归查找，不改变树形。
2. `Union(firstRoot, secondRoot)`：将第二棵树挂到第一棵树。
3. `WeightedUnion(firstRoot, secondRoot)`：小集合挂到大集合，同大小时保留第一个根。
4. `UnionByHeight(firstRoot, secondRoot)`：按高度上界（秩）合并，相等时保留第一个根。
5. `CollapsingFind(member)`：两趟迭代查找并把路径上的非根结点直接连到根。

**三个合并接口接收根编号，而不是任意成员编号。** 应先调用 `Find` 或 `CollapsingFind`；根已相同则合并不做操作，避免形成自环和重复累计大小。传入越界编号抛出 `std::out_of_range`，传入非根抛出 `std::invalid_argument`。

```cpp
#include "set/include/UFSets.h"

UFSets classes(12);
int firstRoot = classes.Find(0);
int secondRoot = classes.Find(4);
classes.WeightedUnion(firstRoot, secondRoot);
bool sameClass = classes.Find(0) == classes.Find(4); // true
int memberCount = classes.SetSize(4);                // 2
```

`height[]` 是补充的独立数组：不压缩时维护树高；路径压缩后保留高度上界，后续按秩比较。这样不会把课件中“负的大小”和“负的高度”两种不同表示混在同一数组中，三个合并接口也可以混合调用。`Parent(member)`、`SetSize(member)`、`SetCount()` 是观察结构的补充接口。构造允许零成员；赋值和复制均深拷贝。

第 28～29 页的九个等价对处理后得到三类：`{0,2,4,7,11}`、`{1,3,5}`、`{6,8,9,10}`。具体根编号取决于合并方法，不能把不同但等价的根编号当作算法错误。

`CountProvinces(isConnected, cityCount)` 接收行优先的扁平数组。调用者保证数组有 `cityCount*cityCount` 项且矩阵对称；非零表示连通，只扫描上三角。使用加权合并与路径压缩，返回根数。第 48 页矩阵返回 2；零城市可传空指针，返回 0。非空城市的空指针及负数量抛出 `std::invalid_argument`。

## 相较课件的修正与范围

- 第 8～9 页 `Set` 是 ADT 示意：虚构造函数、缺少返回类型及按值返回抽象类不符合 C++。本目录保留其集合操作语义，使用两种具体表示，不另造多态集合层次。
- 第 11～13 页混用非模板 `bitSet` 与 `bitSet<T>`、`vectorSize/vecterSize` 及不一致返回类型；统一为模板、`vectorSize` 和 `int getMember`。
- 第 15 页的拆位/移位改为等价的掩码置位、清位，避免整数提升后的移位问题，并保持第 14 页的高位优先映射。
- 第 19 页的 `bitVector[i] & !R.bitVector[i]` 使用了逻辑非；按子集含义修正为 `bitVector[i] & ~R.bitVector[i]`。
- 第 21 页结点构造函数缺少函数体；第 23 页 `LinkedSet::subSet(bitSet<T>&)` 改为同表示的 `LinkedSet` 参数。
- 第 16～17 页示例的 `Set/AddMember/SubSet` 与类定义不一致，使用 `bitSet<int>/addMember/subSet`。
- 第 37 页未声明第 45 页的 `WeightedUnion`，现补上；按高度合并和路径压缩没有课件函数体，明确属于按规则补写。
- 补充深拷贝、基本参数检查、同根合并保护；没有用 STL 集合或并查集替代核心算法。

## 构建与验证

两份 `.cpp` 自动进入根 CMake 的 `course_code`。全章运行示例放在 `dictionary/examples/chapter6.cpp`，由独立目标 `chapter6_example` 管理，详见字典 README。

2026-10-13 已通过 `course_code`、`chapter6_example` 构建及示例运行；临时 C++20 测试开启 `-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer，通过位边界（0/1/15/16/17/31/32/33/50/100 大小）、集合随机操作与 `std::set` 对照、链表空集/尾结点/复制异常清理、深拷贝、课件等价类、退化链路径压缩、三种合并混用，以及随机图与独立 DFS 的省份数量对照。测试文件及构建产物保存在 `/tmp`，不放入课程目标。
