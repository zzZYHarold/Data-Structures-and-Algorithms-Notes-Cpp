#ifndef CHAPTER4_ARRAY_STORAGE_H
#define CHAPTER4_ARRAY_STORAGE_H

#include <cstddef>

namespace chapter4 {
    // 4.1 一维数组：LOC(i) = base + i * elementSize
    std::size_t loc1D(std::size_t base, std::size_t i, std::size_t elementSize);

    // 二维数组 A[rows][cols] 的行优先地址：
    // LOC(j, k) = base + (j * cols + k) * elementSize
    std::size_t loc2DRowMajor(std::size_t base, std::size_t j, std::size_t k,
                              std::size_t cols, std::size_t elementSize);

    // 二维数组 A[rows][cols] 的列优先地址：
    // LOC(j, k) = base + (k * rows + j) * elementSize
    std::size_t loc2DColumnMajor(std::size_t base, std::size_t j, std::size_t k,
                                 std::size_t rows, std::size_t elementSize);

    // 三维数组 A[m1][m2][m3] 按“页 / 行 / 列”存放：
    // LOC(i, j, k) = base + (i*m2*m3 + j*m3 + k) * elementSize
    std::size_t loc3DPageRowColumn(std::size_t base, std::size_t i, std::size_t j,  std::size_t k,
                                   std::size_t m2, std::size_t m3, std::size_t elementSize);

    // 三维数组 A[m1][m2][m3] 按“列 / 行 / 页”存放：
    // LOC(i, j, k) = base + (k*m1*m2 + j*m1 + i) * elementSize
    std::size_t loc3DColumnRowPage(std::size_t base, std::size_t i, std::size_t j,  std::size_t k,
                                   std::size_t m1, std::size_t m2, std::size_t elementSize);
} // namespace chapter4

#endif
