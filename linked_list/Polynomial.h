#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iosfwd>

// 2.5 多项式的链表存储表示：按指数升序保存非零项。

struct Term {
    float coef;
    int exp;
    Term* link;

    Term(float c = 0.0f, int e = -1, Term* next = nullptr)
        : coef(c), exp(e), link(next) {}

    Term* InsertAfter(float c, int e);
};

class Polynomial {
public:
    Polynomial();
    Polynomial(const Polynomial& other);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& other);

    int maxOrder() const;
    Term* getHead() const { return first; }

    void clear();
    void reverse();

    // 按指数升序插入；若已有同指数项则合并系数，为 0 时删除该项。
    void insertTerm(float coef, int exp);

    friend Polynomial operator+(const Polynomial& a, const Polynomial& b);
    friend std::ostream& operator<<(std::ostream& out, const Polynomial& p);
    friend std::istream& operator>>(std::istream& in, Polynomial& p);

private:
    Term* first; // 附加头结点，数据为 (0, -1)
};

#endif
