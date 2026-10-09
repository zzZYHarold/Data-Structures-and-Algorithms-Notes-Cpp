# 第五章：树与二叉树

本目录整理《数据结构与算法》第五章“树与二叉树”的主要 C++ 代码实现。

## 课件来源

**5.1～5.7 统一以 2026 秋 `Lesson 5 (2026秋)-CS.pdf` 完整课件为准。** 文件共 197 页，包含第五章全部小节及末页总结。

完整课件的前 146 页与此前 part2 一致；新增的第 147～155 页完成 5.5 的树与森林遍历，第 157～177 页展开堆，第 178～196 页展开 Huffman 树与编码。所有页码均指 PDF 页序。

2026 part1 已覆盖的核心代码包括：二叉树结点/类结构、父结点查找、销毁、广义表建树、递归/非递归遍历、结点数、高度、复制、前序序列建树、广义表输出、层次序遍历和交换左右子树等。

## 目录结构

```text
tree/
├── include/
│   ├── BinaryTree.h
│   ├── ThreadTree.h
│   ├── Tree.h
│   ├── MinHeap.h
│   └── HuffmanTree.h
├── src/
│   └── README.md
└── README.md
```

本章主要使用模板类，所以模板的声明与实现均保存在头文件中，`src/` 暂无独立 `.cpp` 实现。

## 文件说明

### `include/BinaryTree.h`

对应 5.2～5.3，代码见完整课件第 37～88 页：

- `BinTreeNode<T>` 二叉链表结点
- `BinaryTree<T>` 二叉树类
- 求树高 `Height()`
- 求结点数 `Size()`
- 查找父结点 `Parent()`
- 左/右子女访问
- 递归前序、中序、后序遍历
- 非递归前序、中序、后序遍历
- 层次序遍历
- 前序序列 + 空结点标记递归建树
- 广义表建树（2026 新增内容）
- 广义表形式输出（2026 新增内容）
- 复制二叉树
- 交换所有结点左右子树（2026 课堂练习）

### `include/ThreadTree.h`

对应 5.4，依据完整课件第 90～121 页：

- `ThreadNode<T>` 线索二叉树结点
- `ltag / rtag`
- 中序线索化
- 中序第一个 / 最后一个结点
- 中序前驱 / 后继
- 利用线索完成中序遍历
- 第 118～120 页练习：通过“子树最左结点的前驱”和“子树最右结点的后继”两条路径查找双亲，不再从根递归搜索整个树。

调用 `Next()`、`Prior()`、`parent()` 和线索中序遍历前，先执行 `createInThread()`。`parent()` 的结点参数应属于本树。`ltag/rtag` 保留课件命名：`0` 表示真实子女指针，`1` 表示线索；析构仅沿真实子女指针释放结点。

课件还讲解了前序、后序线索树寻找后继的逻辑，但没有给出与中序线索化同等完整的类实现，因此本文件暂以课件完整给出的**中序线索化实现**为主，没有擅自扩展成另一套前序/后序线索树类。

### `include/Tree.h`

对应 5.5，依据完整课件第 123～155 页：

- 树的子女-兄弟表示
- `TreeNode<T>`
- `Tree<T>`
- `getRoot()` / `FirstChild(node)` / `NextSibling(node)` / `Parent(node)` 直接返回结点指针
- 查找结点
- 先根、后根、层次序遍历

新版课件第 132～137 页采用“传入结点、返回结点指针”的接口。旧预习版的 `current` 游标和无参数 `Root/Parent/FirstChild/NextSibling/getCurrent` 已移除，调用方式改为：

```cpp
Tree<char> tree;
tree.BuildRoot('A');
TreeNode<char>* rootNode = tree.getRoot();
TreeNode<char>* child = tree.InsertChild(rootNode, 'B');
TreeNode<char>* parentNode = tree.Parent(child);  // 返回 rootNode
TreeNode<char>* foundNode = tree.Find('B');       // 返回 child，未找到时返回 nullptr
```

`Find()` 在课件中只有声明，仓库按先根次序补全查找，重复值返回首次匹配。第 145～146 页给出了先根、后根遍历规则，仓库据此补成 `PreOrder()`、`PostOrder()`；完整课件第 147～148 页正式给出 `LevelOrder()` 的队列算法，当前实现已与之对齐。回调参数均为 `TreeNode<T>*`。对于课件中的 `A(B(E,F),C,D(G))`，三种遍历结果分别为 `ABEFCDG`、`EFBCGDA`、`ABCDEFG`。

第 149～155 页继续给出森林的先根、后根与层次序遍历规则及示例，但没有独立森林类的完整代码；当前不另建 `Forest` 类。

树与二叉树、森林与二叉树之间的转换在课件中主要以规则和示意图讲解，没有给出完整转换函数，因此这里没有凭空增加转换 API。

### `include/MinHeap.h`

对应 5.6，依据完整课件第 157～177 页：

- `Element<T, E>`：`T key` 保存关键码，`E other` 保存附加数据；比较只看 `key`
- 最小堆 `MinHeap`
- 数组顺序存储
- 筛选法建堆
- `siftDown()` 下滑调整
- `siftUp()` 上滑调整
- 插入
- 删除堆顶

保持课件中“完全二叉树 + 数组”的核心设计，没有使用 `std::priority_queue` 替代堆的核心实现。

`MinHeap<T, E>` 的两个模板参数现在分别代表关键码类型、附加数据类型，`Insert()`、`Remove()` 和数组建堆接口均使用 `Element<T, E>`。原预习版直接传入裸 `E` 的调用应改为元素对象；原 `MinPQ` 抽象类已按正式课件中的 `MinHeap` 类定义移除。例如：

```cpp
MinHeap<int, char> heap;
heap.Insert(Element<int, char>(7, 'A'));
heap.Insert(Element<int, char>(2, 'C'));
Element<int, char> minimumElement;
heap.Remove(minimumElement);  // key == 2，other == 'C'
```

默认容量下限为 20；堆满或堆空时，插入或删除返回 `false`。`MakeEmpty()` 清空元素计数，`Size()` 返回元素个数，`data()` 返回只读的 `Element` 数组。若 `other` 保存指针，堆不负责释放其指向的对象。

### `include/HuffmanTree.h`

对应 5.7，依据完整课件第 178～196 页：

- Huffman 结点
- 使用最小堆维护森林
- 每次取两个最小权值结点合并
- 建立 Huffman 树
- 计算带权路径长度 WPL

保留课件的 `HuffmanTree<T, E>`、`HuffmanNode<T, E>` 类名与模板形式；`E` 是数值权值类型，`T` 当前没有用于保存字符。输入权值数组采用 0-based 下标，权值应为非负频度或概率。建树的森林堆使用 `MinHeap<E, HuffmanNode<T, E>*>`，`Element.key` 为根权值，`Element.other` 为根结点指针；结点独立分配，地址不会随堆元素搬移而失效。

课件频度 `{2, 7, 4, 5}` 建成的树根权值为 `18`，WPL 为 `35`。空输入生成空树，单个权值生成一个叶结点，其深度和 WPL 均为 `0`。默认构造、`build()` 重建、`getRoot()` 与 `weightedPathLength()` 是便于使用的补充接口。

课件对 Huffman 编码给出了构造思想和例子，但没有给出一套完整的“字符 → 编码串”类接口，所以本次主要整理其**建树实现**；编码映射暂不额外设计。

## 对课件代码所做的必要修正

以下属于为了让代码可以直接编译、避免明显错误而进行的修正，而不是改变课程算法：

1. **统一二叉树遍历函数命名。**
   课件中 public 接口使用 `preOrder / inOrder / postOrder`，部分实现却写成 `PreOrder / InOrder / PostOrder`；整理后统一使用 public 接口的命名。

2. **修正非递归前序遍历中的遍历指针初始化。**
   课件代码片段出现 `BinTreeNode<T>* p = t;`，该函数中并没有参数 `t`；整理后按算法含义从 `root` 初始化遍历指针，当前变量名为 `currentNode`。

3. **修正模板函数形参前后不一致。**
   课件中部分 protected 声明使用引用 `BinTreeNode<T>&`，实现却按指针访问；整理后统一为 `BinTreeNode<T>*`。

4. **使用标准 C++ `nullptr`、`std::stack`、`std::queue`。**
   栈和队列在本章只是遍历算法的辅助结构，并非本章需要重新实现的数据结构；二叉树、线索树和堆的核心实现仍然自行完成。

5. **广义表建树增加基本输入检查。**
   课件算法默认输入合法；整理版跳过空白，并对括号栈明显不合法的情况返回 `false`。

6. **修正 Huffman 课件代码中的明显接口/生命周期问题。**
   第 190～192 页使用 `minHeap/MinHeap` 混合命名，引用变量没有绑定对象，`w[i+1]` 假定输入为 1-based，直接向 `Element` 堆插入结点的类型也不匹配。合并函数对引用使用 `->`，并将数值权值写成 `data.key`；堆内对象地址也不能作为长期子结点地址。仓库统一采用 0-based 数值权值和 `Element<权值类型, 结点指针>`，按原算法反复取出两棵最小权值的树并合并。

7. **补充 part2 中的空指针与线索判断。**
   `Tree::FindParent()` 补充空子树检查；`ThreadTree::First/Last()` 处理空指针；线索双亲查找只把真实子女指针视为双亲关系。线索化递归也检查标志位，避免重复调用时沿已有线索进入循环。另外，`BinaryTree::Parent(nullptr)` 明确返回空指针，避免误认缺少子女的结点为双亲。

8. **统一堆元素及删除接口。**
   第 177 页的 `MinHeap<T>::Remove(E&)` 与第 161～162 页的 `Element<T, E>` 存储不一致，仓库统一为 `MinHeap<T, E>::Remove(Element<T, E>&)`。比较与赋值支持 `const` 对象；数组建堆从 `currentSize / 2 - 1` 开始，删除最后一个元素后跳过下滑调整，明确处理空堆、单元素堆。

9. **修正树的层次序遍历起点。**
   第 148 页用未定义的 `t` 初始化遍历指针；当前实现从本树的 `root` 入队，沿每个结点的子女链依次入队。

## 变量命名

第五章代码中的形参与局部变量使用便于阅读的名称，类名与主要公开操作名继续与课件对应。例如第 135 页的双亲查找：

| 课件名称 | 仓库名称 | 含义 |
| --- | --- | --- |
| `t` | `subtreeRoot` | 当前搜索子树的根 |
| `p` | `targetNode` | 要找双亲的目标结点 |
| `q` | `child` | 正在扫描的子女 |
| `s` | `parentNode` | 从子树中找到的双亲 |

线索化中的 `pre` 改为 `previousNode`；遍历的栈和队列使用 `nodeStack`、`pendingNodes` 等名字。保留 `ltag/rtag`、`RefValue` 等明确的课件术语，并在代码中解释含义。

堆调整使用 `parentIndex`、`childIndex`、`smallerChildIndex`、`savedElement`；Huffman 建树使用 `weights`、`leafCount`、`forestHeap`、`firstTree`、`secondTree`、`parentNode`。

## 为完整性增加的补充

以下不是课件逐行给出的代码，但属于使整理后的类可独立使用所需的少量补充：

- `BinaryTree` 的复制赋值运算符，避免含动态内存的类发生浅拷贝。
- `BinaryTree::Find()` 的普通递归查找实现；课件接口中列出了 `Find`，但当前 part1 未给出完整函数体。
- `ThreadTree::setRoot()`、`getRoot()` 和析构清理，用于构造和管理完整对象。
- `ThreadTree::inOrder()` 用课件 `First/Next` 接口完成遍历；`Tree` 的先根、后根遍历按课件规则补全。
- `Tree::BuildRoot()`、`InsertChild()`，用于建立子女-兄弟表示的树；命名与 5.1 抽象接口中的 `BuildRoot` 保持一致。
- `Tree::Find()` 和子树释放，课件只给出声明。`Tree` 与 `ThreadTree` 暂禁用复制构造和复制赋值，避免默认浅拷贝导致重复释放结点；`InsertChild()` 的父结点必须来自本树，`ThreadTree` 接管构造函数或 `setRoot()` 传入的独立树的所有权。
- `MinHeap::Size()` / `data()`，便于检查堆状态，不参与核心算法。
- `Element` 的默认初始化和便捷构造函数；堆与 Huffman 建树检查负容量、负元素数量及非空输入的空数组指针，非法参数抛出 `std::invalid_argument`。`MinHeap` 与 `HuffmanTree` 禁用默认复制，避免浅拷贝导致重复释放。
- `HuffmanTree::weightedPathLength()`，直接对应课件定义的 WPL。

## 暂未补全的内容

课件虽然在抽象接口中列出了某些操作，但没有给出明确的一般二叉树语义或完整实现，例如通用 `BinaryTree::Insert/Remove`。对于“普通二叉树”而言，插入位置本身需要额外规则，因此本次没有自行发明插入/删除策略。

同理，5.5 的树/森林转换和 5.7 的字符编码表生成，在目前作为依据的课件中主要是概念与规则，没有形成完整接口；本次不额外工程化扩展。

## CMake

本目录当前没有新增 `examples/main.cpp` 或普通 `.cpp` 实现。模板代码都在头文件中，因此按现有课程仓库的约定，**不需要为本目录额外添加 executable target**。

`course_code` 中列出的头文件不会自动实例化模板，所以模板实现另用临时 C++20 测试文件验证，该测试文件不放入仓库。

2026-09-30 更新已通过 `course_code` 构建，以及开启 `-Wall -Wextra -pedantic`、AddressSanitizer 和 UndefinedBehaviorSanitizer 的 C++20 编译与运行检查。验证包含课件树的三种遍历、结点访问与双亲查找、二叉树递归/非递归遍历，以及 0～7 个结点的全部 626 种二叉树形态下的线索前驱、后继、双亲和重复线索化。

2026-10-09 完整版更新已通过同样的 C++20 编译、AddressSanitizer、UndefinedBehaviorSanitizer 检查和 `course_code` 构建。堆测试包含课件数组建堆、关键码与附加数据一致性、4000 次混合操作、空堆/满堆/单元素及非法输入；Huffman 测试包含结构和双亲指针、WPL=35 的课件示例、WPL=2.15 的概率示例、80 组小规模权值与穷举最优 WPL 对照，以及 64 个叶结点和重复建树。
