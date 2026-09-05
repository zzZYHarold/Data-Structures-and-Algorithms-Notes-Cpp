# 第三章 3.1 栈：代码整理

本讲课件虽然在目录页列出了 3.1~3.5，但正文从第 6 页开始直到第 53 页均围绕 **3.1 栈** 展开，没有进入 3.2「栈与递归」。

## 内容对应

| 课件内容 | 页码 | 代码 |
|---|---:|---|
| 栈的抽象数据类型 | 10 | `include/Stack.h` |
| 顺序栈 | 11~17 | `include/SeqStack.h` |
| 双栈共享一个栈空间 | 18~21 | `include/DualStack.h` |
| 链式栈 | 22~29 | `include/LinkedStack.h` |
| 括号匹配 | 30~35 | `src/StackApplications.cpp` |
| 中缀表达式求值 | 41~45 | `src/StackApplications.cpp` |
| 后缀表达式求值 | 46~50 | `src/StackApplications.cpp` |
| 中缀转后缀 | 51~53 | `src/StackApplications.cpp` |

## 与课件原代码相比做的整理

1. 保留课程中的 `Push / Pop / getTop / IsEmpty / IsFull / getSize` 命名，方便对照 PPT。
2. `Stack` 增加虚析构函数，并统一 `Push(const T&)` 接口。
3. 顺序栈补齐课件中没有给出具体值的 `stackIncrement`，默认每次扩容 20 个元素。
4. 双栈补齐完整类定义、构造/析构及基本查询操作。
5. 链式栈修正课件中 `Push` 声明与定义参数类型不一致、`return true` 缺分号等明显代码问题。
6. 课件括号匹配示例写成 `Stack<char> S`，但 `Stack` 是抽象类，不能直接实例化；这里改为 `SeqStack<char>`。
7. 课件后缀计算 `Run` 逐字符把 `ch-'0'` 当数字，因此实际上只能处理单个数字，与课件 `12`、`0.5` 的示例不一致。本整理使用空格分隔 token，可正确处理多位数和小数。
8. 课件对“中缀求值”和“中缀转后缀”主要给出算法过程，本整理按课件规则补成了可运行实现。

## 编译

```bash
cmake -S . -B build
cmake --build build
./build/stack_demo
```

`stack_demo` 中包含顺序栈、共享双栈、链式栈以及四个栈应用的基本断言测试。
