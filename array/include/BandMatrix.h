#ifndef CHAPTER4_BAND_MATRIX_H
#define CHAPTER4_BAND_MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace chapter4 {
    // 多对角（带状）矩阵。
    // 课件采用“过渡矩阵 A'”的方案：把 2b+1 条对角线映射到 n * (2b+1) 的 A'，再按行优先压缩到一维数组 B。
    // 对带内元素 A[i][j]：
    //   t = i, s = j - i + b
    //   k = (2b + 1) * t + s = (2b + 1) * i + j - i + b
    template<class T>
    class BandMatrix {
    public:
        BandMatrix(std::size_t n, std::size_t bandwidth)
            : n_(n), b_(bandwidth), elem_(nullptr) {
            if (n_ > 0 && b_ >= n_)
                throw std::invalid_argument("bandwidth must be smaller than matrix size");

            elem_ = new T[storageSize()]{};
        }

        ~BandMatrix() {
            delete[] elem_;
        }

        BandMatrix(const BandMatrix& other)
            : n_(other.n_), b_(other.b_), elem_(new T[other.storageSize()]{}) {
            for (std::size_t i = 0; i < storageSize(); ++i)
                elem_[i] = other.elem_[i];
        }

        BandMatrix& operator=(const BandMatrix& other) {
            if (this == &other)
                return *this;

            BandMatrix temp(other);
            swap(temp);
            return *this;
        }

        std::size_t size() const {
            return n_;
        }

        std::size_t bandwidth() const {
            return b_;
        }

        std::size_t storageSize() const {
            return n_ * (2 * b_ + 1);
        }

        void set(std::size_t i, std::size_t j, const T& value) {
            checkIndex(i, j);
            if (!inBand(i, j))
                throw std::invalid_argument("cannot store an out-of-band element in BandMatrix");
            elem_[index(i, j)] = value;
        }

        T get(std::size_t i, std::size_t j) const {
            checkIndex(i, j);
            if (!inBand(i, j))
                return T{};
            return elem_[index(i, j)];
        }

        void swap(BandMatrix& other) noexcept {
            std::swap(n_, other.n_);
            std::swap(b_, other.b_);
            std::swap(elem_, other.elem_);
        }

    private:
        std::size_t n_;
        std::size_t b_;
        T* elem_;

        bool inBand(std::size_t i, std::size_t j) const {
            return (i - b_ <= j) && (j <= i + b_);
        }

        std::size_t index(std::size_t i, std::size_t j) const {
            const std::size_t s = j - i + b_;
            return (2 * b_ + 1) * i + s;
        }

        void checkIndex(std::size_t i, std::size_t j) const {
            if (i >= n_ || j >= n_)
                throw std::out_of_range("BandMatrix index out of range");
        }
    };
} // namespace chapter4

#endif
