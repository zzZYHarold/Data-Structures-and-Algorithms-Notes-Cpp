#pragma once

#include <iosfwd>

namespace queue_app {
    // 利用链式队列逐行输出杨辉三角形的第 1~n 行。
    // 课件中的第 1 行为：1 1。
    void printYangHui(int n, std::ostream &os);
} // namespace queue_app
