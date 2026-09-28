#include "string/include/AString.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace chapter4 {

AString::AString(int sz)
    : ch(nullptr), curLength(0), maxSize(sz > 0 ? sz : 0) {
    ch = new char[maxSize + 1];
    ch[0] = '\0';
}

AString::AString(const char* init)
    : ch(nullptr), curLength(0), maxSize(defaultSize) {
    if (init == nullptr)
        init = "";

    curLength = static_cast<int>(std::strlen(init));
    maxSize = std::max(curLength, defaultSize);
    ch = new char[maxSize + 1];
    std::strcpy(ch, init);
}

AString::AString(const AString& ob)
    : ch(new char[ob.maxSize + 1]),
      curLength(ob.curLength),
      maxSize(ob.maxSize) {
    std::strcpy(ch, ob.ch);
}

AString::~AString() {
    delete[] ch;
}

AString AString::operator()(int pos, int len) const {
    AString temp;

    if (pos < 0 || pos >= curLength || len <= 0)
        return temp;

    if (pos + len > curLength)
        len = curLength - pos;

    // temp 默认有 defaultSize 容量；若子串更长则重新分配。
    if (len > temp.maxSize) {
        delete[] temp.ch;
        temp.maxSize = len;
        temp.ch = new char[temp.maxSize + 1];
    }

    temp.curLength = len;
    for (int i = 0, j = pos; i < len; ++i, ++j)
        temp.ch[i] = ch[j];
    temp.ch[len] = '\0';

    return temp;
}

bool AString::operator==(const AString& ob) const {
    return std::strcmp(ch, ob.ch) == 0;
}

bool AString::operator!=(const AString& ob) const {
    return !(*this == ob);
}

AString& AString::operator=(const AString& ob) {
    if (this == &ob)
        return *this;

    char* newCh = new char[ob.maxSize + 1];
    std::strcpy(newCh, ob.ch);

    delete[] ch;
    ch = newCh;
    curLength = ob.curLength;
    maxSize = ob.maxSize;  // 课件实现遗漏了这一句，这里修正。

    return *this;
}

AString& AString::operator+=(const AString& ob) {
    const int oldLength = curLength;
    const int appendLength = ob.curLength;
    const int newLength = oldLength + appendLength;
    const int newMaxSize = std::max(maxSize, newLength);

    char* oldCh = ch;
    ch = new char[newMaxSize + 1];

    for (int i = 0; i < oldLength; ++i)
        ch[i] = oldCh[i];

    // 自身连接时，ob.ch 已随 this->ch 改变，因此必须从 oldCh 读取原串。
    const char* source = (&ob == this) ? oldCh : ob.ch;
    for (int i = 0; i < appendLength; ++i)
        ch[oldLength + i] = source[i];

    ch[newLength] = '\0';
    curLength = newLength;
    maxSize = newMaxSize;

    delete[] oldCh;
    return *this;
}

char& AString::operator[](int i) {
    checkIndex(i);
    return ch[i];
}

const char& AString::operator[](int i) const {
    checkIndex(i);
    return ch[i];
}

int AString::Find_BF(const AString& pat, int k) const {
    const int n = curLength;
    const int m = pat.curLength;

    if (m == 0 || k < 0 || k > n)
        return -1;

    for (int i = k; i <= n - m; ++i) {
        int j = 0;
        for (; j < m; ++j) {
            if (ch[i + j] != pat.ch[j])
                break;
        }
        if (j == m)
            return i;
    }

    return -1;
}

int AString::fastFind(const AString& pat, int k, const int pi[]) const {
    int posP = 0;
    const int lengthP = pat.curLength;

    for (int posT = k; posT < curLength; ++posT) {
        while (posP > 0 && pat.ch[posP] != ch[posT])
            posP = pi[posP - 1];

        if (pat.ch[posP] == ch[posT])
            ++posP;

        if (posP == lengthP)
            return posT - lengthP + 1;
    }

    return -1;
}

int AString::Find_KMP(const AString& pat, int k) const {
    if (pat.curLength == 0 || k < 0 || k > curLength)
        return -1;

    int* pi = new int[pat.curLength];
    pat.getPi(pi);
    const int result = fastFind(pat, k, pi);
    delete[] pi;
    return result;
}

void AString::getPi(int pi[]) const {
    if (pi == nullptr)
        throw std::invalid_argument("pi array must not be null");
    if (curLength == 0)
        return;

    pi[0] = 0;
    for (int i = 1; i < curLength; ++i) {
        int j = pi[i - 1];
        while (j > 0 && ch[i] != ch[j])
            j = pi[j - 1];
        if (ch[i] == ch[j])
            ++j;
        pi[i] = j;
    }
}

void AString::checkIndex(int i) const {
    if (i < 0 || i >= curLength)
        throw std::out_of_range("AString index out of range");
}

}  // namespace chapter4
