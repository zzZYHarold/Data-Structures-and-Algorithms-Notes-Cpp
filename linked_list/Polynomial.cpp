#include "Polynomial.h"

#include <cmath>
#include <iostream>
#include <utility>

namespace {
    constexpr float EPS = 1e-6f;
}

Term* Term::InsertAfter(float c, int e) {
    link = new Term(c, e, link);
    return link;
}
/*  等效：
    Term* newNode = new Term(c, e);
    newNode->link = this->link;
    this->link = newNode;
 */

Polynomial::Polynomial() : first(new Term(0.0f, -1)) {}

Polynomial::Polynomial(const Polynomial& other) : Polynomial() {
    Term* tail = first;
    for (Term* p = other.first->link; p != nullptr; p = p->link)
        tail = tail->InsertAfter(p->coef, p->exp);
}

Polynomial::~Polynomial() {
    clear();
    delete first;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other)
        return *this;

    Polynomial temp(other);
    std::swap(first, temp.first);
    return *this;
}

void Polynomial::clear() {
    Term* p = first->link;
    first->link = nullptr;

    while (p != nullptr) {
        Term* q = p;
        p = p->link;
        delete q;
    }
}

int Polynomial::maxOrder() const {
    int order = -1;
    for (Term* p = first->link; p != nullptr; p = p->link)
        order = p->exp;
    return order;
}

void Polynomial::reverse() {
    Term* pre = nullptr;
    Term* cur = first->link;
    while (cur != nullptr) {
        Term* suc = cur->link;
        cur->link = pre;
        pre = cur;
        cur = suc;
    }
    first->link = pre;
}

void Polynomial::insertTerm(float c, int e) {
    if (std::fabs(c) < EPS)
        return;

    Term* prev = first;
    Term* cur = first->link;

    while (cur != nullptr && cur->exp < e) {
        prev = cur;
        cur = cur->link;
    }

    if (cur != nullptr && cur->exp == e) {
        cur->coef += c;
        if (std::fabs(cur->coef) < EPS) {
            prev->link = cur->link;
            delete cur;
        }
        return;
    }

    prev->link = new Term(c, e, cur);
}

Polynomial operator+(const Polynomial& a, const Polynomial& b) {
    Polynomial c;

    Term* pa = a.first->link;
    Term* pb = b.first->link;
    Term* pc = c.first;

    // PPT 第 114 页：同时扫描两个按指数升序排列的多项式。
    while (pa != nullptr && pb != nullptr) {
        if (pa->exp < pb->exp) {
            pc = pc->InsertAfter(pa->coef, pa->exp);
            pa = pa->link;
        } else if (pb->exp < pa->exp) {
            pc = pc->InsertAfter(pb->coef, pb->exp);
            pb = pb->link;
        } else {
            const float sum = pa->coef + pb->coef;
            if (std::fabs(sum) >= EPS)
                pc = pc->InsertAfter(sum, pa->exp);
            pa = pa->link;
            pb = pb->link;
        }
    }

    while (pa != nullptr) {
        pc = pc->InsertAfter(pa->coef, pa->exp);
        pa = pa->link;
    }

    while (pb != nullptr) {
        pc = pc->InsertAfter(pb->coef, pb->exp);
        pb = pb->link;
    }

    return c;
}

std::ostream& operator<<(std::ostream& out, const Polynomial& p) {
    Term* cur = p.first->link;
    if (cur == nullptr) {
        out << '0';
        return out;
    }

    bool firstTerm = true;
    while (cur != nullptr) {
        const float c = cur->coef;
        const int e = cur->exp;

        if (!firstTerm)
            out << (c >= 0 ? " + " : " - ");
        else if (c < 0)
            out << '-';

        const float absCoef = std::fabs(c);
        const bool printCoef = (e == 0) || std::fabs(absCoef - 1.0f) >= EPS;

        if (printCoef) out << absCoef;
        if (e > 0) {
            out << 'x';
            if (e != 1)
                out << '^' << e;
        }

        firstTerm = false;
        cur = cur->link;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Polynomial& p) {
    // 简单输入格式：先输入项数 n，再输入 n 组 “coef exp”。
    // 例如：3 1 0 -3 6 7 12
    int n;
    if (!(in >> n))
        return in;

    p.clear();
    for (int i = 0; i < n; ++i) {
        float c;
        int e;
        in >> c >> e;
        p.insertTerm(c, e);
    }
    return in;
}
