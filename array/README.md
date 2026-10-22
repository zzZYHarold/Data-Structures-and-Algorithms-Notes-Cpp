# 第四章：数组与矩阵

**4.1–4.3 依据《Lesson 4 (2026秋)-CS》整理。** 内容包括一维/多维数组、特殊矩阵和稀疏矩阵；4.4 字符串见 [string/README.md](../string/README.md)，4.5 广义表见 [generalized_list/README.md](../generalized_list/README.md)。

## 文件与课件对应

| 文件 | 课件内容 | 实现来源 |
| --- | --- | --- |
| `include/ArrayStorage.h`、`src/ArrayStorage.cpp` | 4.1，一维/多维数组的地址映射 | 将课件地址公式整理为普通函数 |
| `examples/image_flip.cpp` | 4.1，灰度图/彩色图左右翻转 | 课件案例的可选示例，依赖 OpenCV |
| `include/SymmetricMatrix.h` | 4.2，对称矩阵压缩存储 | 按课件公式补成模板实现 |
| `include/TriDiagonalMatrix.h` | 4.2，三对角矩阵压缩存储 | 按课件公式补全，核心映射为 `k = 2*i + j` |
| `include/BandMatrix.h` | 4.2，多对角（带状）矩阵 | 保留课件 `A -> A' -> B` 过渡矩阵方案 |
| `include/SparseMatrix.h` | 4.3，稀疏矩阵三元组表示 | 转置、快速转置与加法按 Part 2 算法；乘法为补充实现 |

## 4.1 一维数组与多维数组

`ArrayStorage.h/.cpp` 将课件中的地址映射公式整理为普通函数，包括一维数组、二维数组行/列优先、三维数组按页/行/列与按列/行/页的地址计算。

`examples/image_flip.cpp` 对应课件中的灰度图/彩色图左右翻转案例，依赖 OpenCV，因此仅作为可选示例。

## 4.2 特殊矩阵

- `SymmetricMatrix.h`：对称矩阵压缩存储。
- `TriDiagonalMatrix.h`：三对角矩阵压缩存储，核心映射 `k = 2*i + j`。
- `BandMatrix.h`：多对角（带状）矩阵，保留课件 `A -> A' -> B` 的过渡矩阵方案。

这些类属于根据课件公式补成的模板实现，核心存储没有用 STL 容器替代。

## 4.3 稀疏矩阵

`SparseMatrix.h` 保留课件的三元组结构与数据成员：

- `Trituple<T>`
- `Rows / Cols / Terms`
- `elem`
- `maxTerms`

Part 2 已正式给出普通转置、快速转置和稀疏矩阵加法，因此当前实现按课件算法对齐：

- `Transpose()`：按列重复扫描三元组表；
- `FastTranspose()`：使用 `rowSize` 与 `rowStart`；
- `Add()`：将 `(row, col)` 映射成行优先位置后，按两个有序三元组表归并。

`Multiply()` 在课件类声明中出现，但本份 PPT 未继续给出实现；仓库中保留一个直接三元组乘法作为补充，并在代码中明确标注。

课件还介绍了稀疏矩阵的正交链表表示，但只给出结点结构和示意图，没有形成完整 C++ 类/算法，因此当前不单独增加 `OrthogonalList` 文件。

## 修正与补充

1. `Trituple::operator=` 的参数改为 `const` 引用，避免不必要的复制限制。
2. 补充安全的复制构造、赋值与容量扩展，避免动态数组浅拷贝。
3. 补充 `get/set` 和输入输出接口，便于验证与长期维护。
4. `Multiply()` 属于补充实现，不视为课件已讲代码。

## CMake

`array/src/ArrayStorage.cpp` 已属于普通课程代码。模板类均完整放在头文件中。

当前根 `CMakeLists.txt` 会递归收集普通 `.cpp/.h` 并排除 `examples/*.cpp`，因此本目录本身不需要新增 CMake 配置。
