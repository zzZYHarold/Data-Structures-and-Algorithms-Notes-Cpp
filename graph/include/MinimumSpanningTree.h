#pragma once

#include "graph/include/MinSpanTree.h"
#include "set/include/UFSets.h"
#include "tree/include/MinHeap.h"

#include <memory>

// 第 77～79 页：按权出堆，用并查集判断是否成环。
template <class T, class E>
void Kruskal(Graph<T, E>& graph, MinSpanTree<T, E>& mst) {
    if (graph.IsDirected())
        throw std::invalid_argument("Kruskal requires an undirected graph");
    const int vertexCount = graph.NumberOfVertices();
    const int requiredEdges = vertexCount > 0 ? vertexCount - 1 : 0;
    if (mst.Capacity() < requiredEdges)
        throw std::length_error("MST capacity too small");

    mst.Clear();
    // 复用第五章的双模板参数堆：key 是 cost，other 是完整树边。
    MinHeap<E, MSTEdgeNode<T, E>> candidates(graph.NumberOfEdges());
    UFSets components(vertexCount);
    for (int tail = 0; tail < vertexCount; ++tail) {
        for (int head = tail + 1; head < vertexCount; ++head) {
            const E cost = graph.getWeight(tail, head);
            if (cost < maxWeight)
                candidates.Insert({cost, {tail, head, cost}});
        }
    }
    Element<E, MSTEdgeNode<T, E>> minimum;
    while (mst.Size() < requiredEdges && candidates.Remove(minimum)) {
        const auto& edge = minimum.other;
        const int tailRoot = components.Find(edge.tail);
        const int headRoot = components.Find(edge.head);
        if (tailRoot != headRoot) {
            components.Union(tailRoot, headRoot);
            mst.Insert(edge);
        }
    }
    if (mst.Size() != requiredEdges)
        throw std::runtime_error("Kruskal: graph is disconnected");
}

// 第 88～90 页：使用候选边堆的 Prim。
template <class T, class E>
void Prim(Graph<T, E>& graph, const T startValue, MinSpanTree<T, E>& mst) {
    if (graph.IsDirected())
        throw std::invalid_argument("Prim requires an undirected graph");
    int current = graph.getVertexPos(startValue);
    if (current == -1)
        throw std::invalid_argument("Prim start vertex not found");
    const int vertexCount = graph.NumberOfVertices();
    if (mst.Capacity() < vertexCount - 1)
        throw std::length_error("MST capacity too small");

    mst.Clear();
    auto inTree = std::make_unique<bool[]>(vertexCount);
    inTree[current] = true;
    int includedVertices = 1;
    MinHeap<E, MSTEdgeNode<T, E>> candidates(graph.NumberOfEdges());
    while (includedVertices < vertexCount) {
        for (int neighbor = graph.getFirstNeighbor(current); neighbor != -1;
             neighbor = graph.getNextNeighbor(current, neighbor)) {
            if (!inTree[neighbor]) {
                const E cost = graph.getWeight(current, neighbor);
                candidates.Insert({cost, {current, neighbor, cost}});
            }
        }
        Element<E, MSTEdgeNode<T, E>> minimum;
        // 堆中可能残留两端都已入树的旧边，逐条丢弃。
        do {
            if (!candidates.Remove(minimum))
                throw std::runtime_error("Prim: graph is disconnected");
        } while (inTree[minimum.other.head]);

        mst.Insert(minimum.other);
        current = minimum.other.head;
        inTree[current] = true;
        ++includedVertices;
    }
}
