#ifndef CHAPTER4_TRI_DIAGONAL_MATRIX_H
#define CHAPTER4_TRI_DIAGONAL_MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace chapter4 {
    // 三对角矩阵：仅主对角线及其上下相邻两条对角线可为非零。
    // 按课件顺序按行压缩后，共有 3n - 2 个存储位置（n > 0）。
    // 对带内元素 A[i][j]：k = 2 * i + j :
    // 0到i-1行总元素：2 + 3 * (i - 1) = 3i - 1
    // 第i行第一个元素：j = i - 1 -> 偏移量：j - (i - 1) = j - i + 1
    // 所以k = (3i - 1) + (j - i + 1) = 2 * i + j
    template<class T>
    class TriDiagonalMatrix {
    public:
        explicit TriDiagonalMatrix(std::size_t n)
            : n_(n), elem_(n == 0 ? nullptr : new T[3 * n - 2]{}) {}

        ~TriDiagonalMatrix() {
            delete[] elem_;
        }

        TriDiagonalMatrix(const TriDiagonalMatrix& other)
            : n_(other.n_), elem_(other.n_ == 0 ? nullptr : new T[other.storageSize()]{}) {
            for (std::size_t i = 0; i < storageSize(); ++i)
                elem_[i] = other.elem_[i];
        }

        TriDiagonalMatrix& operator=(const TriDiagonalMatrix& other) {
            if (this == &other)
                return *this;

            TriDiagonalMatrix temp(other);
            swap(temp);
            return *this;
        }

        std::size_t size() const {
            return n_;
        }

        std::size_t storageSize() const {
            return n_ == 0 ? 0 : 3 * n_ - 2;
        }

        void set(std::size_t i, std::size_t j, const T& value) {
            checkIndex(i, j);
            if (!inBand(i, j))
                throw std::invalid_argument("cannot store a non-band element in TriDiagonalMatrix");
            elem_[index(i, j)] = value;
        }

        T get(std::size_t i, std::size_t j) const {
            checkIndex(i, j);
            if (!inBand(i, j))
                return T{};
            return elem_[index(i, j)];
        }

        void swap(TriDiagonalMatrix& other) noexcept {
            std::swap(n_, other.n_);
            std::swap(elem_, other.elem_);
        }

    private:
        std::size_t n_;
        T* elem_;

        bool inBand(std::size_t i, std::size_t j) const {
            return (i - 1 <= j) && (j <= i + 1);
        }

        std::size_t index(std::size_t i, std::size_t j) const {
            return 2 * i + j;
        }

        void checkIndex(std::size_t i, std::size_t j) const {
            if (i >= n_ || j >= n_)
                throw std::out_of_range("TriDiagonalMatrix index out of range");
        }
    };
} // namespace chapter4

#endif
