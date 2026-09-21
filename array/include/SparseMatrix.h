#ifndef CHAPTER4_SPARSE_MATRIX_H
#define CHAPTER4_SPARSE_MATRIX_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace chapter4 {

constexpr int defaultSize = 100;

// 三元组：一个非零元素由“行、列、值”唯一确定。
template <class T>
struct Trituple {
    int row = 0;
    int col = 0;
    T value{};

    // 课件参数为非常量引用；这里修正为 const 引用，便于正常复制。
    Trituple<T>& operator=(const Trituple<T>& x) {
        if (this != &x) {
            row = x.row;
            col = x.col;
            value = x.value;
        }
        return *this;
    }
};

// 三元组顺序表表示的稀疏矩阵。
// 保留课件的核心数据成员：Rows / Cols / Terms / elem / maxTerms。
template <class T>
class SparseMatrix {
public:
    explicit SparseMatrix(int maxSz = defaultSize)
        : Rows(0), Cols(0), Terms(0),
          elem(new Trituple<T>[maxSz > 0 ? maxSz : 1]),
          maxTerms(maxSz > 0 ? maxSz : 1) {}

    // 补充：便于直接指定矩阵规格；不改变课件的数据结构设计。
    SparseMatrix(int rows, int cols, int maxSz = defaultSize)
        : Rows(rows), Cols(cols), Terms(0),
          elem(new Trituple<T>[maxSz > 0 ? maxSz : 1]),
          maxTerms(maxSz > 0 ? maxSz : 1) {
        if (rows < 0 || cols < 0)
            throw std::invalid_argument("matrix dimensions must be non-negative");
    }

    SparseMatrix(const SparseMatrix<T>& x)
        : Rows(x.Rows), Cols(x.Cols), Terms(x.Terms),
          elem(new Trituple<T>[x.maxTerms]), maxTerms(x.maxTerms) {
        for (int i = 0; i < Terms; ++i)
            elem[i] = x.elem[i];
    }

    ~SparseMatrix() {
        delete[] elem;
    }

    SparseMatrix<T>& operator=(const SparseMatrix<T>& x) {
        if (this == &x)
            return *this;

        SparseMatrix<T> temp(x);
        swap(temp);
        return *this;
    }

    // 普通转置：按原矩阵的列号 0,1,...,Cols-1 依次扫描三元组表。
    // 时间复杂度与课件一致，为 O(Cols * Terms)。
    void Transpose(SparseMatrix<T>& b) const {
        b.Rows = Cols;
        b.Cols = Rows;
        b.Terms = Terms;
        b.ensureCapacity(Terms > 0 ? Terms : 1);

        if (Terms <= 0)
            return;

        int CurrentB = 0;  // 转置矩阵三元组表当前写入位置
        for (int k = 0; k < Cols; ++k) {  // 第 k 次处理原矩阵第 k 列
            for (int i = 0; i < Terms; ++i) {
                if (elem[i].col == k) {
                    b.elem[CurrentB].row = k;
                    b.elem[CurrentB].col = elem[i].row;
                    b.elem[CurrentB].value = elem[i].value;
                    ++CurrentB;
                }
            }
        }
    }

    // 快速转置：rowSize 记录原矩阵各列非零元素个数，
    // rowStart 记录这些列转置成“行”以后在结果三元组表中的起始位置。
    // 时间复杂度 O(Cols + Terms)。
    void FastTranspose(SparseMatrix<T>& b) const {
        b.Rows = Cols;
        b.Cols = Rows;
        b.Terms = Terms;
        b.ensureCapacity(Terms > 0 ? Terms : 1);

        if (Terms <= 0)
            return;

        int* rowSize = new int[Cols];
        int* rowStart = new int[Cols];

        for (int i = 0; i < Cols; ++i)
            rowSize[i] = 0;

        for (int i = 0; i < Terms; ++i)
            ++rowSize[elem[i].col];

        rowStart[0] = 0;
        for (int i = 1; i < Cols; ++i)
            rowStart[i] = rowStart[i - 1] + rowSize[i - 1];

        for (int i = 0; i < Terms; ++i) {
            const int j = rowStart[elem[i].col];
            b.elem[j].row = elem[i].col;
            b.elem[j].col = elem[i].row;
            b.elem[j].value = elem[i].value;
            ++rowStart[elem[i].col];
        }

        delete[] rowSize;
        delete[] rowStart;
    }

    // 稀疏矩阵加法：利用两个按“行优先、同一行内列递增”排列的
    // 三元组表，按类似归并的方式一次扫描完成。
    void Add(const SparseMatrix<T>& b, SparseMatrix<T>& c) const {
        if (Rows != b.Rows || Cols != b.Cols)
            throw std::invalid_argument("matrix dimensions do not match for addition");

        c.Rows = Rows;
        c.Cols = Cols;
        c.Terms = 0;
        c.ensureCapacity(Terms + b.Terms + 1);

        int i = 0;
        int j = 0;
        int index_a = 0;
        int index_b = 0;

        while (i < Terms && j < b.Terms) {
            // 将二维位置 (row, col) 映射成行优先的一维位置，便于比较先后。
            index_a = Cols * elem[i].row + elem[i].col;
            index_b = Cols * b.elem[j].row + b.elem[j].col;

            if (index_a < index_b) {
                c.elem[c.Terms++] = elem[i++];
            } else if (index_a > index_b) {
                c.elem[c.Terms++] = b.elem[j++];
            } else {
                const T value = elem[i].value + b.elem[j].value;
                if (!(value == T{})) {
                    c.elem[c.Terms] = elem[i];
                    c.elem[c.Terms].value = value;
                    ++c.Terms;
                }
                ++i;
                ++j;
            }
        }

        while (i < Terms)
            c.elem[c.Terms++] = elem[i++];
        while (j < b.Terms)
            c.elem[c.Terms++] = b.elem[j++];
    }

    // 补充：part2 仍只声明 Multiply，没有给出课件实现。
    // 这里保留一个直接三元组版本，方便课程仓库完整使用。
    void Multiply(const SparseMatrix<T>& b, SparseMatrix<T>& c) const {
        if (Cols != b.Rows)
            throw std::invalid_argument("matrix dimensions do not match for multiplication");

        SparseMatrix<T> result(Rows, b.Cols, defaultSize);

        // A(i,k) 与 B(k,j) 匹配时，对 C(i,j) 累加乘积。
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

    // 补充辅助接口：便于示例和测试，不改变核心三元组存储结构。
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

    friend std::ostream& operator<<(std::ostream& out, const SparseMatrix<T>& M) {
        out << M.Rows << ' ' << M.Cols << ' ' << M.Terms << '\n';
        for (int i = 0; i < M.Terms; ++i)
            out << M.elem[i].row << ' ' << M.elem[i].col << ' ' << M.elem[i].value << '\n';
        return out;
    }

    friend std::istream& operator>>(std::istream& in, SparseMatrix<T>& M) {
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
    int Rows;              // 矩阵行数
    int Cols;              // 矩阵列数
    int Terms;             // 当前非零元素个数
    Trituple<T>* elem;     // 三元组表
    int maxTerms;          // 三元组表容量

    void swap(SparseMatrix<T>& other) noexcept {
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

    void append(int row, int col, const T& value) {
        ensureCapacity(Terms + 1);
        elem[Terms].row = row;
        elem[Terms].col = col;
        elem[Terms].value = value;
        ++Terms;
    }

    void accumulate(int row, int col, const T& delta) {
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
                  [](const Trituple<T>& a, const Trituple<T>& b) {
                      if (a.row != b.row)
                          return a.row < b.row;
                      return a.col < b.col;
                  });
    }

    void checkPosition(int row, int col) const {
        if (row < 0 || row >= Rows || col < 0 || col >= Cols)
            throw std::out_of_range("SparseMatrix index out of range");
    }
};

}  // namespace chapter4

#endif
