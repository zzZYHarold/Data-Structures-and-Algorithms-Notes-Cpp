# 第六章：集合与并查集

**6.1～6.2 以 `Lesson 6 (2026秋)-CS(1).pdf` 完整正式课件为准。** 文件共 136 页，正文覆盖第六章 6.1～6.4。本文页码均为该正式课件的 PDF 页序。

6.1 正文为第 2～32 页，6.2 为第 34～58 页；6.3～6.4 见 [dictionary/README.md](../dictionary/README.md)。本次重新核对集合、并查集的类定义与算法，其现有实现与完整课件对应，页码和接口约定保持不变。

## 文件与课件对应

| 文件 | 课件内容 | 实现来源 |
| --- | --- | --- |
| `include/bitSet.h` | 6.1，第 9～19 页，16 位位向量集合 | 位映射、置空、取位/改位、子集判断对应课件；其余操作按声明/规则补全 |
| `include/LinkedSet.h` | 6.1，第 20～32 页，有序链表集合 | 结点、搜索、增删及原地并/交/差对应课件；其余声明补全 |
| `include/UFSets.h`、`src/UFSets.cpp` | 6.2，第 41～52 页，双亲数组并查集 | 构造、迭代 `Find`、`Union`、`WeightedUnion`、`CollapsingFind` 均有正式代码 |
| `include/SetApplications.h`、`src/SetApplications.cpp` | 第 34 页，省份问题 | 课件只有题目，按并查集思路补写 |

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
- `+` / `*` / `-`：按第 27～32 页双指针归并，**原地修改左操作数，返回 `LinkedSet<T>&`**；右操作数保持不变。保留左链中需要的结点，只为并集中新增的右侧成员分配结点；交、差释放被排除的左侧结点。时间为 O(m+n)。
- `subSet(other)`：判断当前集合是否为 `other` 的子集；空集是任意集合的子集。
- `Min(value)` / `Max(value)`：空集返回 `false`，非空时写入结果；维护尾指针使 `Max` 为 O(1)。
- `makeEmpty`、复制构造、赋值和析构均管理真实结点，复制后两个对象互不影响。

这与预习版“不改变原集合、按值返回”的行为不同，也与 `bitSet` 的运算行为不同。若想保留左集合，需要先复制：

```cpp
LinkedSet<int> leftSet, rightSet;
for (int member : {1, 3, 5}) leftSet.addMember(member);
for (int member : {3, 4}) rightSet.addMember(member);
LinkedSet<int> common(leftSet);
common * rightSet;        // common 变为 {3}，leftSet 仍为 {1,3,5}
leftSet + rightSet;       // leftSet 变为 {1,3,4,5}
leftSet - rightSet;       // leftSet 变为 {1,5}
```

增加同一对象运算的保护：`set+set`、`set*set` 保持原集合，`set-set` 清空，避免删除结点后继续沿自身右操作数访问。复制、赋值、置空、相等、子集、`Min/Max` 的函数体仍属于对声明的补充。

## 并查集 `UFSets`

成员编号为 **0～Size()-1**。`parent[root]` 始终是**负的集合大小**；非根的值是双亲下标。保留以下教学顺序：

1. `Find(member)`：第 44 页的迭代查找，不改变树形；替代预习版递归实现。
2. `Union(firstRoot, secondRoot)`：将第二棵树挂到第一棵树。
3. `WeightedUnion(firstRoot, secondRoot)`：小集合挂到大集合，同大小时保留第一个根。
4. `CollapsingFind(member)`：第 52 页的两趟迭代查找，把路径上的非根结点直接连到根；现在属于课件已有实现。

**两个合并接口接收根编号，而不是任意成员编号。** 应先调用 `Find` 或 `CollapsingFind`；根已相同则合并不做操作，避免形成自环和重复累计大小。传入越界编号抛出 `std::out_of_range`，传入非根抛出 `std::invalid_argument`。

```cpp
#include "set/include/UFSets.h"

UFSets classes(12);
int firstRoot = classes.Find(0);
int secondRoot = classes.Find(4);
classes.WeightedUnion(firstRoot, secondRoot);
bool sameClass = classes.Find(0) == classes.Find(4); // true
int memberCount = classes.SetSize(4);                // 2
```

正式课件第 41 页的类采用单一 `parent[]` 数组，没有 `UnionByHeight` 接口。因此移除预习版补写的 `UnionByHeight` 和 `height[]`，调用者改用 `WeightedUnion`。第 47 页提到按高度合并，但未给出对应实现，不额外扩展类。`Parent(member)`、`SetSize(member)`、`SetCount()` 仍是观察结构的补充接口。构造允许零成员；赋值和复制均深拷贝。

第 56～58 页的九个等价对处理后得到三类：`{0,2,4,7,11}`、`{1,3,5}`、`{6,8,9,10}`。具体根编号取决于合并方法，不能把不同但等价的根编号当作算法错误。

`CountProvinces(isConnected, cityCount)` 接收行优先的扁平数组。调用者保证数组有 `cityCount*cityCount` 项且矩阵对称；非零表示连通，只扫描上三角。使用加权合并与路径压缩，返回根数。第 34 页矩阵返回 2；零城市可传空指针，返回 0。非空城市的空指针及负数量抛出 `std::invalid_argument`。

## 相较课件的修正与范围

- 第 7～8 页 `Set` 是 ADT 示意：虚构造函数、缺少返回类型及按值返回抽象类不符合 C++。本目录保留其集合操作语义，使用两种具体表示，不另造多态集合层次。
- 第 11～15 页混用非模板 `bitSet` 与 `bitSet<T>`、不一致的 `getMember/putMember` 类型；统一为模板、`int getMember` 和 `putMember(T,int)`。
- 第 15 页的拆位/移位改为等价的掩码置位、清位，避免整数提升后的移位问题，并保持第 14 页的高位优先映射。
- 正式课件第 16 页已使用正确的按位非 `~`；第 21 页已给出结点构造函数体，第 23 页子集参数也已修正为 `LinkedSet`。这些不再列为今年课件勘误。
- 第 17～18 页示例的 `Set/AddMember/SubSet` 与类定义不一致，使用 `bitSet<int>/addMember/subSet`。
- 第 22～23 页集合运算声明按值返回，但第 27～32 页函数体返回引用并明确修改 `this`；以函数体的原地算法为准统一签名。部分定义缺少模板声明或与声明的参数传递形式不同，统一为 `const T&` / `const LinkedSet&`。
- 增删、原地集合运算在摘链时立即维护前驱链接和尾指针，保持空表、尾结点删除后的有效状态。
- 补充深拷贝、基本参数检查、同根合并保护；没有用 STL 集合或并查集替代核心算法。

## 构建与验证

两份 `.cpp` 自动进入根 CMake 的 `course_code`。全章正式内容的运行示例放在 `dictionary/examples/chapter6.cpp`，由独立目标 `chapter6_example` 管理，详见字典 README。

2026-10-15 已通过 `course_code`、`chapter6_example` 构建及示例运行。临时 C++20 测试开启 `-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer，验证：

- 7 个候选成员的全部 16384 对集合，逐项检查原地并、交、差、引用返回、右侧不变及操作后尾指针；同一对象运算、复制、赋值和成员复制异常后的清理。
- 位向量在全集大小 0/1/15/16/17/31/32/33/50 下的运算和子集关系。
- 100000 个结点的退化链迭代查找和路径压缩、加权根选择、课件三类等价类、随机合并关系对照、非法根及同根合并。
- 课件省份矩阵及随机无向图与独立 DFS 的数量对照。

临时测试与构建产物位于 `/tmp`，不放入课程目标。

2026-10-20 完整课件更新已重新核对本目录来源与页码，并通过 `course_code` 构建和全章示例的 C++20、AddressSanitizer、UndefinedBehaviorSanitizer 运行检查；本目录核心算法本次未改动。
