#pragma once

#include <iosfwd>

// 3.2 栈与递归
// 对应课件中的阶乘、汉诺塔、斐波那契数列以及尾递归示例。
namespace recursion_app {
    // 阶乘：n! = n * (n - 1)!，0! = 1。
    long Factorial(long n);

    // 汉诺塔：将 n 个圆盘从 A 借助 B 移到 C。
    // A：源柱子（起始柱） B：辅助柱子（中转柱） C：目标柱子
    // 每次移动以 "move A to C" 的形式写入 os。
    void Hanoi(int n, char A, char B, char C, std::ostream &os);

    // 斐波那契数列的递归版本。
    long Fib(long n);

    // 斐波那契数列的迭代版本，对应课件“递归改非递归”。
    long FibIter(long n);

    // 逆向输出 A[0..n]。
    // n 表示最后一个有效下标，与课件 recfunc(A, n) 的含义一致。
    void printReverseRecursive(const int A[], int n, std::ostream &os);

    void printReverseIterative(const int A[], int n, std::ostream &os);
} // namespace recursion_app
