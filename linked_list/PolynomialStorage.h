#ifndef POLYNOMIAL_STORAGE_H
#define POLYNOMIAL_STORAGE_H

#include <cstddef>
#include <memory>

// 2.5 多项式的几种顺序存储表示。
// 这些结构对应 PPT 第 100~105 页，重点是“如何存”，不是完整运算类。

constexpr int maxDegree = 100;
constexpr int maxTerms = 100;

// 第一种：静态数组，指数连续时 coef[i] 对应 x^i 的系数。
struct StaticPolynomial {
    int degree = 0;
    float coef[maxDegree + 1]{};
};

// 第二种：动态数组。
class DynamicPolynomial {
public:
    explicit DynamicPolynomial(int degree)
        : degree_(degree), coef_(std::make_unique<float[]>(degree + 1)) {}

    int degree() const { return degree_; }
    float& operator[](int i) { return coef_[i]; }
    const float& operator[](int i) const { return coef_[i]; }

private:
    int degree_;
    std::unique_ptr<float[]> coef_;
};

// 第三种：稀疏多项式，同时存储系数和指数。
struct SparseTerm {
    float coef = 0.0f;
    int exp = 0;
};

struct SparsePolynomialView {
    int start = 0;
    int finish = -1;
};

#endif
