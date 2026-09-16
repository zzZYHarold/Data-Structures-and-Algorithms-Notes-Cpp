#ifndef CHAPTER4_SYMMETRIC_MATRIX_H
#define CHAPTER4_SYMMETRIC_MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace chapter4 {
    // 对称矩阵的压缩存储：只保存下三角（含主对角线），按行优先存放。
    // 当 i >= j 时，A[i][j] 在一维数组中的下标为：
    //     k = i * (i + 1) / 2 + j
    // 当 i < j 时，利用 A[i][j] = A[j][i]。
    template<class T>
    class SymmetricMatrix {
    public:
        explicit SymmetricMatrix(std::size_t n)
            : n_(n), elem_(new T[n * (n + 1) / 2]{}) {}

        ~SymmetricMatrix() {
            delete[] elem_;
        }

        SymmetricMatrix(const SymmetricMatrix& other)
            : n_(other.n_), elem_(new T[other.storageSize()]{}) {
            for (std::size_t i = 0; i < storageSize(); ++i)
                elem_[i] = other.elem_[i];
        }

        SymmetricMatrix& operator=(const SymmetricMatrix& other) {
            if (this == &other)
                return *this;

            SymmetricMatrix temp(other);
            swap(temp);
            return *this;
        }

        std::size_t size() const {
            return n_;
        }

        std::size_t storageSize() const {
            return n_ * (n_ + 1) / 2;
        }

        void set(std::size_t i, std::size_t j, const T& value) {
            checkIndex(i, j);
            elem_[index(i, j)] = value;
        }

        const T& get(std::size_t i, std::size_t j) const {
            checkIndex(i, j);
            return elem_[index(i, j)];
        }

        void swap(SymmetricMatrix& other) noexcept {
            std::swap(n_, other.n_);
            std::swap(elem_, other.elem_);
        }

    private:
        std::size_t n_;
        T* elem_;

        std::size_t index(std::size_t i, std::size_t j) const {
            if (i < j)
                std::swap(i, j);
            return i * (i + 1) / 2 + j;
        }

        void checkIndex(std::size_t i, std::size_t j) const {
            if (i >= n_ || j >= n_)
                throw std::out_of_range("SymmetricMatrix index out of range");
        }
    };
} // namespace chapter4

#endif
