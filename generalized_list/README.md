# 广义表（第四章 4.5）

本目录整理《Lesson 4 (2026秋)-CS - part2》中的广义表链式存储。

## 目录

```text
generalized_list/
├── include/
│   └── GenList.h
└── README.md
```

## 内容

`GenList.h` 保留课件的结点设计：

- `utype = 0`：附加头结点，`info.ref` 保存引用计数；
- `utype = 1`：原子结点，`info.value` 保存原子值；
- `utype = 2`：子表结点，`info.hlink` 指向子表；
- `tlink`：同层后继结点。

同时保留课件中的 `Items<T>`、`GenList<T>`、`Head()`、`Tail()` 等接口。

## 补充说明

课件本份 PPT 只具体给出了构造函数、`Head()` 与 `Tail()`，其余如复制、求长度、求深度、释放等主要以接口形式出现。为了使模板类可以长期维护，仓库补全了非共享、非递归广义表所需的深拷贝、析构、长度和深度等实现，并在代码中注明补充范围。

课件声明了 `CreateList`/输入流建表接口，但没有继续给出完整建表算法，因此当前没有凭空补一个字符串解析器。

## CMake

这是模板类，完整实现放在 `include/GenList.h` 中，不需要 `.cpp`，也不需要修改根 CMake。
