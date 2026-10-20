#pragma once

// 补充：2026 秋正式课件第 60 页的 <关键码, 属性> 表项，比较只使用关键码。
// 可替换为具有相同 E 与 K 比较运算的课程自定义记录类型。
template <class K, class A>
struct DictionaryEntry {
    K key{};
    A attribute{};

    bool operator==(const K& targetKey) const { return key == targetKey; }
    bool operator<(const K& targetKey) const { return key < targetKey; }
    bool operator>(const K& targetKey) const { return key > targetKey; }
};
