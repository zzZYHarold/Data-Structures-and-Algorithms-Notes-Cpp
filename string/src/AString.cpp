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

int AString::fastFind(const AString& pat, int k, const int next[]) const {
    int posP = 0;
    int posT = k;
    const int lengthP = pat.curLength;
    const int lengthT = curLength;

    while (posP < lengthP && posT < lengthT) {
        if (posP == -1 || pat.ch[posP] == ch[posT]) {
            ++posP;
            ++posT;
        } else {
            posP = next[posP];
        }
    }

    if (posP < lengthP)
        return -1;
    return posT - lengthP;
}

int AString::Find_KMP(const AString& pat, int k) const {
    if (pat.curLength == 0 || k < 0 || k > curLength)
        return -1;

    int* next = new int[pat.curLength];
    pat.getNext(next);
    const int result = fastFind(pat, k, next);
    delete[] next;
    return result;
}

void AString::getNext(int next[]) const {
    if (next == nullptr)
        throw std::invalid_argument("next array must not be null");
    if (curLength == 0)
        return;

    int j = 0;
    int k = -1;
    next[0] = -1;

    // 课件写作 while (j < lengthP)，随后先 ++j 再写 next[j]，
    // 最后一轮会访问 next[lengthP]。这里修正为 j < lengthP - 1。
    while (j < curLength - 1) {
        if (k == -1 || ch[k] == ch[j]) {
            ++k;
            ++j;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

void AString::checkIndex(int i) const {
    if (i < 0 || i >= curLength)
        throw std::out_of_range("AString index out of range");
}

}  // namespace chapter4
