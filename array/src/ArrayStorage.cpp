#include "array/include/ArrayStorage.h"

namespace chapter4 {
    std::size_t loc1D(std::size_t base, std::size_t i, std::size_t elementSize) {
        return base + i * elementSize;
    }

    std::size_t loc2DRowMajor(std::size_t base, std::size_t j, std::size_t k,
                              std::size_t cols, std::size_t elementSize) {
        return base + (j * cols + k) * elementSize;
    }

    std::size_t loc2DColumnMajor(std::size_t base, std::size_t j, std::size_t k,
                                 std::size_t rows, std::size_t elementSize) {
        return base + (k * rows + j) * elementSize;
    }

    std::size_t loc3DPageRowColumn(std::size_t base, std::size_t i, std::size_t j,  std::size_t k,
                                   std::size_t m2, std::size_t m3, std::size_t elementSize) {
        return base + (i * m2 * m3 + j * m3 + k) * elementSize;
    }

    std::size_t loc3DColumnRowPage(std::size_t base, std::size_t i, std::size_t j,  std::size_t k,
                                   std::size_t m1, std::size_t m2, std::size_t elementSize) {
        return base + (k * m1 * m2 + j * m1 + i) * elementSize;
    }
} // namespace chapter4
