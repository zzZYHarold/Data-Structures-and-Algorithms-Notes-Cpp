#ifndef CHAPTER4_ASTRING_H
#define CHAPTER4_ASTRING_H

namespace chapter4 {

// 4.4 字符串：按课件设计，用动态字符数组 ch 保存字符串。
// curLength 表示实际长度，maxSize 表示当前可容纳的最大字符数。
class AString {
public:
    static constexpr int defaultSize = 128;

    explicit AString(int sz = defaultSize);
    AString(const char* init);
    AString(const AString& ob);
    ~AString();

    int Length() const { return curLength; }

    // 提取从 pos 开始、最长 len 个字符的子串。
    // 课件声明为返回 AString&，但实现返回局部对象 temp；
    // 这里修正为按值返回，避免悬空引用。
    AString operator()(int pos, int len) const;

    bool operator==(const AString& ob) const;
    bool operator!=(const AString& ob) const;
    bool operator!() const { return curLength == 0; }

    AString& operator=(const AString& ob);
    AString& operator+=(const AString& ob);

    char& operator[](int i);
    const char& operator[](int i) const;

    // 朴素模式匹配（BF）：从目标串当前对象的第 k 个字符开始匹配。
    int Find_BF(const AString& pat, int k = 0) const;

    // KMP 模式匹配。课件类声明与后续实现的函数名不一致：
    // 声明为 Find_KMP，后续算法写作 fastFind；这里统一对外保留 Find_KMP。
    int Find_KMP(const AString& pat, int k = 0) const;

    // 计算“当前串作为模式串”时的 next 特征向量。
    // 调用者需提供至少 Length() 个 int 的数组。
    void getNext(int next[]) const;

    // 补充：便于示例输出和调试，不改变字符串的核心存储方式。
    const char* c_str() const { return ch; }

private:
    char* ch;
    int curLength;
    int maxSize;

    int fastFind(const AString& pat, int k, const int next[]) const;
    void checkIndex(int i) const;
};

}  // namespace chapter4

#endif
