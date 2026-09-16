# 第四章：数组、串与广义表（part1：4.1–4.3）

本目录整理自《Lesson 4 (2026秋)-CS - part1.pdf》。该份 PPT 实际内容到 **4.3 稀疏矩阵的三元组表示与 `SparseMatrix` 类声明** 为止；4.4 字符串与 4.5 广义表只出现在章节目录/知识导图中，尚未展开，因此本次不实现 4.4、4.5。

## 目录

```text
chapter4_arrays_strings_generalized_lists/
├── include/
│   ├── ArrayStorage.h
│   ├── BandMatrix.h
│   ├── SparseMatrix.h
│   ├── SymmetricMatrix.h
│   └── TriDiagonalMatrix.h
├── src/
│   └── ArrayStorage.cpp
├── examples/
│   └── image_flip.cpp      # 可选；需要 OpenCV
└── README.md
```

## 4.1 一维数组与多维数组

`ArrayStorage.h/.cpp` 把课件中的地址映射公式整理为普通函数：

- 一维数组：`LOC(i) = base + i * elementSize`
- 二维数组行优先：`LOC(j,k) = base + (j * cols + k) * elementSize`
- 二维数组列优先：`LOC(j,k) = base + (k * rows + j) * elementSize`
- 三维数组按页/行/列存储
- 三维数组按列/行/页存储

课件还给出了基于 OpenCV 的灰度图和彩色图左右翻转案例。因为它依赖 OpenCV，放入 `examples/`，不应默认加入全仓库的 `course_code` 编译检查。

## 4.2 特殊矩阵

课件主要给出压缩存储的映射公式，没有提供完整 C++ 类代码。本次按这些公式补成模板类：

- `SymmetricMatrix<T>`：保存下三角，按行优先压缩，`k = i(i+1)/2 + j`。
- `TriDiagonalMatrix<T>`：保存三条对角线，共 `3n-2` 个元素，`k = 2i + j`。
- `BandMatrix<T>`：严格采用课件的过渡矩阵 `A'` 方案，存储空间为 `n(2b+1)`；映射 `k = (2b+1)i + j - i + b`。

这些类使用动态数组保存核心数据，没有用 STL 容器替代课件要讲的压缩存储结构。

## 4.3 稀疏矩阵

课件 part1 只给出了：

- `Trituple<T>` 三元组；
- `SparseMatrix<T>` 的接口；
- 数据成员 `Rows / Cols / Terms / elem / maxTerms`；
- `Transpose / Add / Multiply / << / >>` 的声明。

为了让代码可以直接长期维护，本次在 **保持上述数据结构与命名** 的基础上补全了实现：

- 构造、析构、复制；
- 三元组容量扩展；
- 转置；
- 加法；
- 乘法；
- 输入/输出；
- `get/set` 辅助接口。

这些实现属于 **补充**，不是本份 PPT 已给出的算法正文。后续如果 part2 PPT 给出老师自己的实现，应以课件实现为主，再对本文件进行对齐。

## 相较课件的必要修正 / 补充

1. 课件 `Trituple::operator=` 接受非常量引用，本次改为 `const Trituple<T>&`，以支持正常复制常量对象。
2. 课件使用 `defaultSize`，但本份 PPT 没有给出定义；本次补充 `constexpr int defaultSize = 100`。
3. 课件只声明 `SparseMatrix` 的运算接口，本次补齐实现。
4. 动态数组类补充了复制构造/赋值，避免浅拷贝导致重复释放。
5. `BandMatrix` 保留课件“过渡矩阵”方法，因此会有 `(b+1)b` 个额外零存储位置；没有擅自改成更紧凑的对角线数组方案。

## CMake

核心普通源文件只有：

```cmake
chapter4_arrays_strings_generalized_lists/src/ArrayStorage.cpp
```

模板类都完整放在 `include/*.h` 中，不需要单独 `.cpp`。

如果你的根目录已经有 `course_code` OBJECT library，只需把上面的 `ArrayStorage.cpp` 加入其源文件列表，并保持项目根目录为头文件搜索起点即可，例如代码中使用：

```cpp
#include "chapter4_arrays_strings_generalized_lists/include/SymmetricMatrix.h"
```

### 可选 OpenCV 示例

只有你确实要运行 `examples/image_flip.cpp` 时，再单独增加 executable；不要把它加入 `course_code`：

```cmake
find_package(OpenCV REQUIRED)

add_executable(chapter4_image_flip
    chapter4_arrays_strings_generalized_lists/examples/image_flip.cpp
)

target_link_libraries(chapter4_image_flip PRIVATE ${OpenCV_LIBS})
target_include_directories(chapter4_image_flip PRIVATE ${OpenCV_INCLUDE_DIRS})
```
