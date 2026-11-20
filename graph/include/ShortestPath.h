#pragma once

#include "graph/include/Graph.h"

#include <memory>

template <class E>
inline constexpr GraphDistance<E> unreachableDistance =
    std::is_integral_v<E> ? std::numeric_limits<GraphDistance<E>>::max()
                          : std::numeric_limits<GraphDistance<E>>::infinity();

// 最终版第 106～107 页：Dijkstra。source 和 path 的元素都是顶点下标。
template <class T, class E>
void ShortestPath(const Graph<T, E>& graph, int source, GraphDistance<E> dist[], int path[]) {
    const int count = graph.NumberOfVertices();
    if (source < 0 || source >= count)
        throw std::out_of_range("source vertex out of range");
    if (!dist || !path)
        throw std::invalid_argument("null shortest-path array");

    if constexpr (!std::is_unsigned_v<E>) {
        for (int from = 0; from < count; ++from)
            for (int to = graph.getFirstNeighbor(from); to != -1;
                 to = graph.getNextNeighbor(from, to))
                if (graph.getWeight(from, to) < E{})
                    throw std::invalid_argument("Dijkstra requires nonnegative weights");
    }

    auto settled = std::make_unique<bool[]>(count);
    for (int vertex = 0; vertex < count; ++vertex) {
        const E weight = graph.getWeight(source, vertex);
        dist[vertex] = weight < maxWeight ? weight : unreachableDistance<E>;
        path[vertex] = vertex != source && weight < maxWeight ? source : -1;
    }
    settled[source] = true;
    dist[source] = 0;
    for (int step = 1; step < count; ++step) {
        int nearest = -1;
        auto minimum = unreachableDistance<E>;
        for (int vertex = 0; vertex < count; ++vertex) {
            if (!settled[vertex] && dist[vertex] < minimum) {
                minimum = dist[vertex];
                nearest = vertex;
            }
        }
        if (nearest == -1) break; // 剩余顶点均不可达。
        settled[nearest] = true;
        for (int vertex = 0; vertex < count; ++vertex) {
            if (settled[vertex]) continue;
            const E weight = graph.getWeight(nearest, vertex);
            if (weight >= maxWeight) continue;
            const auto candidate = minimum + static_cast<GraphDistance<E>>(weight);
            if (candidate < dist[vertex]) {
                dist[vertex] = candidate;
                path[vertex] = nearest;
            }
        }
    }
}

// 最终版第 117～118 页：a、path 是 n 行指针，每行至少 n 个元素。
template <class T, class E>
void Floyd(const Graph<T, E>& graph, GraphDistance<E>* const a[], int* const path[]) {
    const int count = graph.NumberOfVertices();
    if (count == 0)
        return;
    if (!a || !path)
        throw std::invalid_argument("null Floyd matrix");
    for (int row = 0; row < count; ++row)
        if (!a[row] || !path[row])
            throw std::invalid_argument("null Floyd matrix row");

    for (int from = 0; from < count; ++from) {
        for (int to = 0; to < count; ++to) {
            const E weight = graph.getWeight(from, to);
            a[from][to] = weight < maxWeight ? weight : unreachableDistance<E>;
            path[from][to] = from != to && weight < maxWeight ? from : -1;
        }
    }
    for (int via = 0; via < count; ++via) {
        for (int from = 0; from < count; ++from) {
            if (a[from][via] == unreachableDistance<E>) continue;
            for (int to = 0; to < count; ++to) {
                if (a[via][to] == unreachableDistance<E>) continue;
                const auto candidate = a[from][via] + a[via][to];
                if (candidate < a[from][to]) {
                    if (from == to && candidate < 0)
                        throw std::domain_error("Floyd: negative cycle");
                    a[from][to] = candidate;
                    path[from][to] = path[via][to];
                }
            }
        }
    }
}
