# 第六章：字典与散列

**6.3～6.4 统一以 `Lesson 6 (2026秋)-CS(1).pdf` 完整正式课件为准，共 136 页。** 6.3 正文为第 60～91 页，6.4 为第 93～136 页。本文页码均为该 PDF 页序；6.1～6.2 见 [set/README.md](../set/README.md)。

已逐项核对有序字典、散列函数设计及冲突处理规则。原有 6.3 的搜索下标、`SortedChain` 和双向链表练习与本次课件对应；6.4 新增双向二次探查及相应课堂例子。散列表在课件中没有完整类定义，仓库类接口仍属于按正式规则补写的实现。

## 文件与课件对应

| 文件 | 课件内容 | 实现来源 |
| --- | --- | --- |
| `include/DictionaryEntry.h` | 2026 第 60～61 页，关键码与属性 | 补充的便捷表项类型，比较只看关键码 |
| `include/SortedList.h` | 2026 第 63～75 页，有序顺序表及搜索 | 顺序/递归折半搜索均按 0-based、失败 -1；插入/删除按声明补全 |
| `include/SortedChain.h` | 2026 第 77～83 页，有序链表字典 | 保留 `ChainNode/SortedChain`、搜索前驱、插入及删除算法，修正插入逻辑 |
| `include/BidirectionalSearch.h`、`src/BidirectionalSearch.cpp` | 2026 第 90～91 页，双向搜索练习 | 保留 `Node/search`，补齐练习条件，放在 `bidirectional_search` 命名空间 |
| `include/HashTable.h` | 第 110～129 页，闭散列/开地址法 | 按线性、单向平方、双向二次探查及双散列规则补写 |
| `include/ChainedHashTable.h` | 第 130～134 页，开散列/链地址法 | 按桶数组和同义词链补写 |
| `examples/chapter6.cpp` | 全章 6.1～6.4 | 包含第 123～127 页新例子及断言检查 |

字典中的关键码唯一，属性可以重复。`SortedList<E,K>` 的 `E` 表示整个表项，可使用 `DictionaryEntry<K,A>`，要求 `E == K`、`E < K`、`E > K` 按关键码比较。`SortedChain<E,K>` 的 `E` 则仅表示属性，关键码单独保存在 `ChainNode::name` 中；两个类的模板参数含义以课件各自定义为准。

## 有序顺序表 `SortedList<E,K>`

保留课件的 `public SeqList<E>` 继承关系，复用第二章的手写数组；课件中的 `n` 对应仓库 `Length()`，最后一个下标仍为 `last`。

- `Search(key)`：第 64 页顺序搜索，遇到更大的关键码即失败；返回 **0-based 下标**，失败返回 **-1**。
- `BinarySearch(key,low,high)`：第 72 页递归折半搜索，闭区间及返回下标均为 **0-based**；失败或空区间返回 **-1**。非空区间越界抛出 `std::out_of_range`。
- `BinarySearch(key)`：补充整表搜索入口。
- `Insert(key,entry)`：保留课件 `void` 接口，按关键码找到位置并移动表项。重复关键码或表项与参数关键码不符抛出 `std::invalid_argument`；表满抛出 `std::overflow_error`，不自动扩容。
- `Remove(key,entry)`：按关键码删除，成功写入被删表项并返回 `true`；内部将搜索下标加 1 后调用第二章基类的 `Remove`。
- `Update(key,entry)`：补充属性修改，不允许改变关键码，未找到返回 `false`。
- `getData(position,entry)`、`Length/Size/IsEmpty/IsFull` 继承自 `SeqList`，其中读取序号仍为 **1-based**。成功搜索后读取应使用 `getData(index+1,entry)`，失败时不要读取。

这改变了预习版搜索返回 1-based、失败为 0 的约定；原来的 `if(Search(key))` 必须改为 `if(Search(key) != -1)`，下标 0 是成功结果。

```cpp
#include "dictionary/include/DictionaryEntry.h"
#include "dictionary/include/SortedList.h"
#include <string>

using Entry = DictionaryEntry<int, std::string>;
SortedList<Entry, int> dictionary(50);
dictionary.Insert(20, {20, "book B"});
dictionary.Insert(10, {10, "book A"});
int index = dictionary.BinarySearch(20); // 1
Entry entry;
if (index != -1) dictionary.getData(index + 1, entry); // attribute == "book B"
dictionary.Update(20, {20, "updated"});
dictionary.Remove(10, entry); // entry.key == 10，正确处理下标 0
```

本类隐藏基类任意位置的 `setData`，`Insert/Remove/Search` 也使用字典接口。调用者应通过本类维护有序性；显式向上转换成 `SeqList<E>` 后操作会绕过限制，且基类析构非虚，不能通过基类指针删除对象。由于现有 `SeqList` 无安全的复制赋值，本类禁用复制，避免默认浅拷贝；本次不修改第二章实现。

## 有序链表字典 `SortedChain<E,K>`

第 77～83 页新增的正式实现采用带头结点的单链表，结点含 `K name`、`E attr` 和 `link`，关键码严格递增。

- `Search(key)` 返回是否存在；私有重载同时定位前驱，未找到时前驱仍可用于插入。
- `Insert(key,attribute)` 通过前驱连入有序位置；保留 `void` 接口，重复关键码抛出 `std::invalid_argument`。
- `Remove(key,attribute)` 通过引用输出属性；未找到返回 `false` 且不改输出参数。
- `Begin()` / `Next(node)` 返回首个有效结点/下一个结点，链尾返回空指针。可读取 `name/attr`，修改属性时保留关键码与链接不变。
- `makeEmpty()` 释放有效结点并保留头结点，清空后可继续插入；析构还释放头结点。
- `printDic()` 的函数体是补充，默认逐行输出 `关键码: 属性` 到 `std::cout`；也可传入 `std::ostream&`。

`K/E` 需要默认构造及复制，`K` 支持一致的 `<` 和 `==`；调用输出函数时两者需支持 `<<`。类禁用复制，避免默认浅拷贝。

```cpp
#include "dictionary/include/SortedChain.h"
#include <string>

SortedChain<std::string, int> dictionary; // E 是属性，不是 DictionaryEntry
dictionary.Insert(30, "book C");
dictionary.Insert(10, "book A");
dictionary.Insert(20, "book B"); // 中间插入，保留后面的 30
bool found = dictionary.Search(10);
std::string attribute;
dictionary.Remove(20, attribute); // attribute == "book B"
```

## 跳表与双向搜索练习

第 84～89 页讲解跳表的分层链结构和从最高层向右、遇到更大或相等关键码时向下的搜索规则，给出搜索 30、50 的例子；没有结点/类定义或插入、删除的完整实现。因此本次保留其概念说明，不自行设计 `SkipList` 类。

第 90～91 页双向链表练习保存上次成功位置，从该位置向左或向右搜索。使用 `bidirectional_search::Node` 与 `bidirectional_search::search(head,currentNode,key)`：

- `head` 指向首个有效结点，无头哨兵，两端为 `nullptr`。
- `currentNode` 为空时从 `head` 开始，否则必须指向本链中仍有效的结点。
- 找到后修改 `currentNode` 并返回 `true`；失败返回 `false`，保留上次成功位置。
- 链按非递减关键码排列；若有重复，返回沿当前方向遇到的匹配结点。

## 6.4 散列

散列表使用整个表项 `E`，要求 `E == K`；散列函数只取决于关键码，相等关键码必须散列到相同地址。第 98～109 页讲解直接定址、数字分析、除留余数、平方取中及折叠等函数设计方法；示例采用课件的取余和首字母函数，没有另造通用散列框架。

### 闭散列 `HashTable<E,K>`

采用手写槽数组。构造时传入表长 `m`、主散列函数、探查方式，二次散列还需第二个散列函数。散列函数用 `std::function<int(const K&)>` 保存便于传入 lambda，这只是可调用接口；核心存储和探查没有使用 `std::unordered_map` 等容器。

| `ProbingMethod` | 第 i 次探查地址（i 从 0 开始） | 对应课件 |
| --- | --- | --- |
| `Linear` | `(home+i)%m` | 第 110～120 页 |
| `Quadratic` | `(home+i*i)%m` | 第 121 页，仅正平方增量 |
| `QuadraticAlternating` | `home,home+1,home-1,home+4,home-4,…`，每次取模 | 第 122～124 页，正、负平方交替 |
| `DoubleHashing` | `(home+i*step)%m` | 第 125～128 页，`step=hash2(key)%m` |

主函数结果归一化为 0～m-1，因此负整数关键码也可以使用取余函数。地址计算用 `long long`，避免 `i*i` 的 `int` 溢出；负平方增量取模为负时加上 m。

`QuadraticAlternating` 要求表长是 **4k+3 形式的质数**，构造时验证，否则抛出 `std::invalid_argument`。第 0 次探查为 home，之后依次加减 `i²`，其中 `i=1,…,(m-1)/2`，共 m 个互异地址。它与 `Quadratic` 是两种不同算法，不能把单向平方序列用在第 123 页的双向例子上。

第 125 页要求双散列规范用法的步长为小于 m 且与 m 互素的正整数，这样能遍历全表。课件第 128、136 页同时使用表长 10 和 `7-key%7`，其中某些步长不互素（例如 58 的步长为 5）。为复现这两页，保留非互素步长的有限探查；运行时只拒绝模 m 后为 0 的步长，不保证非互素情况下所有空槽可达。需要保证全表可达时，按第 126 页使用质数表长及 1～m-1 的步长。

```cpp
#include "dictionary/include/HashTable.h"

HashTable<Entry, int> table(
    10, [](const int& key) { return key % 10; },
    ProbingMethod::DoubleHashing,
    [](const int& key) { return 7 - key % 7; });
table.Insert(49, {49, "record"});
int probes;
int address = table.Search(49, &probes); // 散列地址为 0-based；失败 -1
table.getData(address, entry);
```

`Insert` 成功返回 `true`；重复、表满或探查序列无法到达可用槽返回 `false`。**单向平方探查不保证遍历所有槽；双散列步长与表长不互素时也可能只访问部分槽。** 因而“仍有空槽”不保证这些方法插入成功。各方法最多探查 m 次，避免循环。

`Search(key,&probes)` 的可选计数包括空槽、删除标记和重复地址。删除使用补充的 `Deleted` 墓碑状态，使后续查找可以越过被删表项；插入记住第一个墓碑，并继续检查是否已有同名关键码，防止产生重复。`Update` 仅修改同一关键码的属性。`getData` 读取已占用的 0-based 槽；`Size/Length/makeEmpty` 用于观察容量、表项数和清空。表项关键码与参数不符抛出 `std::invalid_argument`。

第 116、121、128 页 `{89,18,49,58,69}` 在表长 10 下的结果：

| 方法 | 已占用的“地址:关键码” | 成功 ASL |
| --- | --- | --- |
| 线性探查 | `0:49, 1:58, 2:69, 8:18, 9:89` | `12/5 = 2.4` |
| 平方探查 | `0:49, 2:58, 3:69, 8:18, 9:89` | `10/5 = 2` |
| 双散列，`hash2(key)=7-key%7` | `0:69, 3:58, 6:49, 8:18, 9:89` | `8/5 = 1.6` |

第 117～118 页须区分**表长 12**与主函数 `key%11` 的**初始地址范围 0～10**。槽 11 虽不能作为初始地址，仍可处理冲突；失败 ASL 按 11 个等概率初始地址计算，得到 `75/11`，成功 ASL 为 `18/11`。不能简单把表长当作失败 ASL 的分母。第 119～120 页表长 11 的练习成功 ASL 为 `11/8`，失败 ASL 为 `26/11`。

第 123～124 页采用 `QuadraticAlternating`、表长 19，依次插入 `{37,25,14,36,49,68,57,11}`，对应地址为 `{18,6,14,17,11,12,0,10}`。成功 ASL 为 `11/8`，失败 ASL 为 `33/19`（19 个初始地址等概率）。

第 126～127 页采用 `DoubleHashing`、表长 11，`Hash(key)=3*key%11`、`ReHash(key)=7*key%10+1`，依次插入 `{22,41,53,46,30,13,1,67}`，对应地址为 `{0,2,5,6,3,8,10,1}`，成功 ASL 为 `17/8`。示例程序对这两组新增例子的关键地址和 ASL 保留断言检查。

失败 ASL 必须说明查询分布。线性探查或双向二次探查的序列只取决于初始地址，可按等概率初始地址平均；双散列的序列还取决于第二散列步长，不能只对主地址平均。

### 开散列 `ChainedHashTable<E,K>`

桶数组下标为 0～m-1，各桶存放手写单链表。采用尾插，保持第 133 页同义词子表的插入顺序，没有固定表项容量或自动扩桶。

- `Insert(key,entry)`、`Remove(key,entry)`、`Update(key,entry)` 成功返回 `true`，重复/不存在返回 `false`；关键码不一致抛出异常。
- `Search(key,entry,&probes)` 成功时返回表项；失败不改输出表项。按课件 ASL 约定，成功计访问结点数，失败计链长加一（包括链尾空指针检查）。
- `VisitBucket(index,visit)` 用只读回调观察桶中表项，便于画出散列表；非法桶号抛出 `std::out_of_range`。
- `Size/Length/makeEmpty` 分别表示桶数、表项数和清空。

第 133 页名字例子的桶为 `0: Attlee -> Alton`、`1: Burke -> Broad -> Blum`、`4: Ekers -> Ederly`、`7: Hecht`。以首字母减 `'A'` 为散列函数，成功 ASL 为 `13/8`，失败 ASL 为 `34/26`（26 个初始桶等概率）。

两种散列表均禁用复制，防止动态存储的默认浅拷贝。课件没有散列表类名和公开接口，以上接口、删除、更新和槽状态均是按正式规则补写的内容，不是课件逐行给出的类实现。第 129 页说明不能任意物理删除闭散列表项；本实现用墓碑保留探查链，并在插入时复用墓碑。

## 相较课件的修正与范围

2026 正式课件：

- 第 63 页 `SortedList : public SeqList` 补成 `SeqList<E>`；不重复定义第二章已有的全局 `defaultSize`，本类默认容量仍为 50。
- 第 72 页 `BinarySearch` 函数体对应的声明补入公开接口，按第 64、72 页统一为 0-based、失败 -1。使用 `low+(high-low)/2` 求中点，课件的 `n` 映射为基类的 `Length()`。
- `SortedList` 的插入、删除、属性修改和便捷记录类型仍属于补充。
- 第 78 页 `SortedChain::Remove(K,E)` 与第 83 页引用输出不一致，统一为 `Remove(K,E&)`；补充析构释放头结点、`makeEmpty` 与 `printDic`。
- 第 79～80 页搜索失败返回空指针，但第 81～82 页直接用它作为新结点后继，会在头部/中间插入时截断后续链。改用搜索得到的前驱的 `link` 作为后继，并拒绝重复关键码，保留有序字典语义。
- 部分页上灰框遮住或留空的练习步骤，按同节搜索、插入规则补全；双向搜索使用有意义的 `currentNode/candidate` 变量名。跳表未给出完整类，不扩展其增删接口。

散列部分的核对与补充：

- 第 95 页冲突例子的输入列表为 `07251`，后文等式误写成 `07250`；按十进制数计算，7251 的散列值才是 13444，7250 的值为 13443。示例中的前导 0 是展示格式，不应直接写成 C++ 八进制字面量。
- 第 121 页单向平方探查与第 122 页双向二次探查分别实现，后者验证质数表长并修正负余数。
- 第 125 页的互素步长要求与第 128、136 页练习并不完全一致，按上文明确区分规范条件与有限步演示。
- 第 135 页 MD5 仅作应用介绍，不实现密码学算法。第 136 页课堂作业可用现有散列表复现，不另建每题程序。

## 构建与验证

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target course_code chapter6_example
./cmake-build-debug/chapter6_example
```

项目保持 C++20 和现有 CMake 版本要求。`chapter6_example` 显式链接 `set/src/UFSets.cpp`、`set/src/SetApplications.cpp` 和 `dictionary/src/BidirectionalSearch.cpp`；`examples/*.cpp` 不进入 `course_code`。新增普通 `.cpp` 由根目录 glob 自动进入 `course_code`。

先前的 2026-10-13 预习实现已做散列地址、ASL、墓碑复用及随机操作检查。以下分别保留 6.3 更新与完整课件更新的验证记录。

2026-10-15 正式版更新已通过 `course_code`、`chapter6_example` 构建和示例运行。临时 C++20 测试开启 `-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer，验证：

- `SortedList` 4000 次混合操作对照 `std::map`，重点检查下标 0 的搜索、更新、删除，失败 -1，闭区间搜索及容量/参数边界。
- `SortedChain` 4000 次混合操作对照 `std::map`，遍历验证有序性与属性、首部/中间/尾部插入删除、重复关键码、空链复用、输出格式及字符串关键码。
- 双向搜索遍历 21 种起始位置（包括空游标）与 42 个目标关键码，检查向左/向右/原地匹配及失败保留游标，另测空链与单结点。
- 新实现与现有 `MinHeap/PQueue` 等头文件同时包含编译；运行例子确认原有散列预习地址和成功 ASL 保持一致。

2026-10-20 完整课件更新已通过 `course_code`、`chapter6_example` 构建及示例运行；示例及临时测试均在 C++20 下开启 `-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer，通过以下检查：

- 双向二次探查在 3/7/11/19/23/31/43/47/59/67/71/79/83/127 等表长下，对每个初始地址插满整表，逐项核对正负平方顺序、所有地址恰好访问一次、满表终止、墓碑复用及重复检测；另测负散列值、`INT_MIN/INT_MAX` 和非法/合数表长。
- 四种开地址策略各 4000 次混合操作对照 `std::map`，链地址法另做 4000 次混合操作；检查插入、搜索、删除、属性更新和清空。
- 核对第 113～118、119～120、123～124、126～128、133 页的关键地址及成功/失败 ASL，包括表长 12 与初始地址范围 11 的区别、平方探查未满插入失败和双散列非互素步长的有限探查。
- `chapter6_example` 中保留表长 19、11 两组新例子的断言，后续运行示例即可复查关键结果。当前全章示例不再带预习标记。

临时测试与构建产物位于 `/tmp`，不放入课程目标。
