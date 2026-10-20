#pragma once

// 6.2 双亲数组并查集，2026 秋正式课件第 41~52 页，成员编号 0..size-1。
// 根的 parent 为负的集合大小，非根的 parent 为双亲下标。
class UFSets {
    int* parent;
    int size;

    void checkMember(int member) const;
    void checkRoots(int firstRoot, int secondRoot) const;
    void attach(int root, int childRoot);

public:
    explicit UFSets(int sz = 10);
    UFSets(const UFSets& other);
    ~UFSets();
    UFSets& operator=(const UFSets& other);

    int Find(int member) const;                  // 第 44 页迭代查找，不压缩路径
    void Union(int firstRoot, int secondRoot);   // 第二棵树挂到第一棵树
    void WeightedUnion(int firstRoot, int secondRoot);
    int CollapsingFind(int member);              // 第 52 页两趟路径压缩

    int Size() const { return size; }
    int Parent(int member) const;                // 补充：观察双亲数组
    int SetSize(int member) const;
    int SetCount() const;
};
