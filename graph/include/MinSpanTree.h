#pragma once

#include "graph/include/Graph.h"

// 完整课件第 75～76 页：生成树只存边，端点为原图下标。
template <class T, class E>
struct MSTEdgeNode {
    int tail = -1;
    int head = -1;
    E cost{};
};

template <class T, class E>
class MinSpanTree {
public:
    explicit MinSpanTree(int capacity = DefaultVertices - 1) : maxSize(capacity) {
        if (capacity < 0) throw std::invalid_argument("negative MST capacity");
        edgevalue = new MSTEdgeNode<T, E>[capacity];
    }
    ~MinSpanTree() { delete[] edgevalue; }
    MinSpanTree(const MinSpanTree&) = delete;
    MinSpanTree& operator=(const MinSpanTree&) = delete;

    // 补全课件的 Insert 声明：成功返回存储下标，满时返回 -1。
    int Insert(const MSTEdgeNode<T, E>& edge) {
        if (n == maxSize) return -1;
        edgevalue[n] = edge;
        return n++;
    }
    int Size() const { return n; }
    int Capacity() const { return maxSize; }
    void Clear() { n = 0; }
    const MSTEdgeNode<T, E>& getEdge(int index) const {
        if (index < 0 || index >= n) throw std::out_of_range("MST edge index out of range");
        return edgevalue[index];
    }

protected:
    MSTEdgeNode<T, E>* edgevalue;
    int maxSize;
    int n = 0;
};
