# 字符串（第四章 4.4）

本目录整理《Lesson 4 (2026秋)-CS - part2》中的字符串与模式匹配实现。

## 目录

```text
string/
├── include/
│   └── AString.h
├── src/
│   └── AString.cpp
└── README.md
```

## 内容

`AString` 保留课件的核心设计：

- `char* ch`：动态字符数组；
- `curLength`：实际长度；
- `maxSize`：当前容量；
- 子串提取、比较、赋值、连接、下标访问；
- BF 朴素模式匹配；
- KMP 模式匹配；
- `next` 特征向量计算。

## 必要修正

1. 课件的子串运算声明返回 `AString&`，但实现返回局部对象；这里改为按值返回，避免悬空引用。
2. 课件类声明写 `Find_KMP`，后续算法实现写 `fastFind`；仓库统一保留 `Find_KMP()` 作为公开接口，`fastFind()` 作为内部辅助函数。
3. 课件 `getNext()` 的循环边界会在最后一轮写到 `next[lengthP]`；这里修正为只写合法的 `next[0..lengthP-1]`。
4. 赋值运算补充同步 `maxSize`，并处理自赋值。

## CMake

`string/src/AString.cpp` 是普通课程代码。当前根 `CMakeLists.txt` 会自动纳入 `course_code`，因此无需手动修改。
