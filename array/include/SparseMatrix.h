#ifndef CHAPTER4_SPARSE_MATRIX_H
#define CHAPTER4_SPARSE_MATRIX_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace chapter4 {
    constexpr int defaultSize = 100;

    // 三元组
    template<class T>
    struct Trituple {
        int row = 0;
        int col = 0;
        T value{};

        Trituple<T>& operator=(const Trituple<T>& x) {
            if (this != &x) {
                row = x.row;
                col = x.col;
                value = x.value;
            }
            return *this;
        }
    };

    // 三元组表示稀疏矩阵
    // 保留课件中的核心数据成员：Rows / Cols / Terms / elem / maxTerms，并补全课件在 part1 中只声明而未给出实现的操作。
    template<class T>
    class SparseMatrix {
    public:
        explicit SparseMatrix(int maxSz = defaultSize)
            : Rows(0), Cols(0), Terms(0),
              elem(new Trituple<T>[maxSz > 0 ? maxSz : 1]),
              maxTerms(maxSz > 0 ? maxSz : 1) {}

        SparseMatrix(int rows, int cols, int maxSz = defaultSize)
            : Rows(rows), Cols(cols), Terms(0),
              elem(new Trituple<T>[maxSz > 0 ? maxSz : 1]),
              maxTerms(maxSz > 0 ? maxSz : 1) {
            if (rows < 0 || cols < 0)
                throw std::invalid_argument("matrix dimensions must be non-negative");
        }

        SparseMatrix(const SparseMatrix<T> &x)
            : Rows(x.Rows), Cols(x.Cols), Terms(x.Terms),
              elem(new Trituple<T>[x.maxTerms]), maxTerms(x.maxTerms) {
            for (int i = 0; i < Terms; ++i)
                elem[i] = x.elem[i];
        }

        ~SparseMatrix() {
            delete[] elem;
        }

        SparseMatrix<T> &operator=(const SparseMatrix<T> &x) {
            if (this == &x)
                return *this;

            SparseMatrix<T> temp(x);
            swap(temp);
            return *this;
        }

        void Transpose(SparseMatrix<T> &b) const {
            SparseMatrix<T> result(Cols, Rows, Terms > 0 ? Terms : 1);

            // 依列扫描原矩阵，使转置后的三元组仍保持行优先顺序。
            for (int col = 0; col < Cols; ++col) {
                for (int i = 0; i < Terms; ++i) {
                    if (elem[i].col == col)
                        result.append(elem[i].col, elem[i].row, elem[i].value);
                }
            }

            b = result;
        }

        void Add(SparseMatrix<T> &b, SparseMatrix<T> &c) const {
            if (Rows != b.Rows || Cols != b.Cols)
                throw std::invalid_argument("matrix dimensions do not match for addition");

            SparseMatrix<T> result(Rows, Cols, Terms + b.Terms + 1);
            int i = 0;
            int j = 0;

            while (i < Terms && j < b.Terms) {
                const int cmp = comparePosition(elem[i], b.elem[j]);
                if (cmp < 0) {
                    result.append(elem[i].row, elem[i].col, elem[i].value);
                    ++i;
                } else if (cmp > 0) {
                    result.append(b.elem[j].row, b.elem[j].col, b.elem[j].value);
                    ++j;
                } else {
                    T value = elem[i].value + b.elem[j].value;
                    if (!(value == T{}))
                        result.append(elem[i].row, elem[i].col, value);
                    ++i;
                    ++j;
                }
            }

            while (i < Terms) {
                result.append(elem[i].row, elem[i].col, elem[i].value);
                ++i;
            }
            while (j < b.Terms) {
                result.append(b.elem[j].row, b.elem[j].col, b.elem[j].value);
                ++j;
            }

            c = result;
        }

        void Multiply(SparseMatrix<T> &b, SparseMatrix<T> &c) const {
            if (Cols != b.Rows)
                throw std::invalid_argument("matrix dimensions do not match for multiplication");

            SparseMatrix<T> result(Rows, b.Cols, defaultSize);

            // 教学优先的直接三元组实现：
            // 每个 A(i,k) 与每个 B(k,j) 配对，将乘积累加到 C(i,j)。
            for (int i = 0; i < Terms; ++i) {
                for (int j = 0; j < b.Terms; ++j) {
                    if (elem[i].col == b.elem[j].row) {
                        const T product = elem[i].value * b.elem[j].value;
                        if (!(product == T{}))
                            result.accumulate(elem[i].row, b.elem[j].col, product);
                    }
                }
            }

            result.sortTerms();
            c = result;
        }

        int rows() const { return Rows; }
        int cols() const { return Cols; }
        int terms() const { return Terms; }

        T get(int row, int col) const {
            checkPosition(row, col);
            for (int i = 0; i < Terms; ++i) {
                if (elem[i].row == row && elem[i].col == col)
                    return elem[i].value;
            }
            return T{};
        }

        void set(int row, int col, const T& value) {
            checkPosition(row, col);

            for (int i = 0; i < Terms; ++i) {
                if (elem[i].row == row && elem[i].col == col) {
                    if (value == T{}) {
                        for (int j = i; j < Terms - 1; ++j)
                            elem[j] = elem[j + 1];
                        --Terms;
                    } else {
                        elem[i].value = value;
                    }
                    sortTerms();
                    return;
                }
            }

            if (!(value == T{})) {
                append(row, col, value);
                sortTerms();
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const SparseMatrix<T> &M) {
            out << M.Rows << ' ' << M.Cols << ' ' << M.Terms << '\n';
            for (int i = 0; i < M.Terms; ++i)
                out << M.elem[i].row << ' ' << M.elem[i].col << ' ' << M.elem[i].value << '\n';
            return out;
        }

        friend std::istream &operator>>(std::istream &in, SparseMatrix<T> &M) {
            int rows = 0;
            int cols = 0;
            int terms = 0;
            in >> rows >> cols >> terms;

            if (!in)
                return in;
            if (rows < 0 || cols < 0 || terms < 0)
                throw std::invalid_argument("invalid sparse matrix header");

            SparseMatrix<T> temp(rows, cols, terms > 0 ? terms : 1);
            for (int i = 0; i < terms; ++i) {
                int row = 0;
                int col = 0;
                T value{};
                in >> row >> col >> value;
                if (!in)
                    return in;
                temp.checkPosition(row, col);
                if (!(value == T{}))
                    temp.append(row, col, value);
            }
            temp.sortTerms();
            M = temp;
            return in;
        }

    private:
        int Rows;
        int Cols;
        int Terms;
        Trituple<T>* elem;
        int maxTerms;

        void swap(SparseMatrix<T> &other) noexcept {
            std::swap(Rows, other.Rows);
            std::swap(Cols, other.Cols);
            std::swap(Terms, other.Terms);
            std::swap(elem, other.elem);
            std::swap(maxTerms, other.maxTerms);
        }

        void ensureCapacity(int required) {
            if (required <= maxTerms)
                return;

            int newCapacity = maxTerms * 2;
            if (newCapacity < required)
                newCapacity = required;

            Trituple<T>* newElem = new Trituple<T>[newCapacity];
            for (int i = 0; i < Terms; ++i)
                newElem[i] = elem[i];

            delete[] elem;
            elem = newElem;
            maxTerms = newCapacity;
        }

        void append(int row, int col, const T &value) {
            ensureCapacity(Terms + 1);
            elem[Terms].row = row;
            elem[Terms].col = col;
            elem[Terms].value = value;
            ++Terms;
        }

        void accumulate(int row, int col, const T &delta) {
            for (int i = 0; i < Terms; ++i) {
                if (elem[i].row == row && elem[i].col == col) {
                    elem[i].value = elem[i].value + delta;
                    if (elem[i].value == T{}) {
                        for (int j = i; j < Terms - 1; ++j)
                            elem[j] = elem[j + 1];
                        --Terms;
                    }
                    return;
                }
            }
            append(row, col, delta);
        }

        void sortTerms() {
            std::sort(elem, elem + Terms,
                      [](const Trituple<T> &a, const Trituple<T> &b) {
                          if (a.row != b.row)
                              return a.row < b.row;
                          return a.col < b.col;
                      });
        }

        static int comparePosition(const Trituple<T> &a, const Trituple<T> &b) {
            if (a.row != b.row)
                return a.row < b.row ? -1 : 1;
            if (a.col != b.col)
                return a.col < b.col ? -1 : 1;
            return 0;
        }

        void checkPosition(int row, int col) const {
            if (row < 0 || row >= Rows || col < 0 || col >= Cols)
                throw std::out_of_range("SparseMatrix index out of range");
        }
    };
} // namespace chapter4

#endif
