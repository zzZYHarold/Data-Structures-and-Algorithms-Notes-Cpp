#pragma once

#include "graph/include/Graph.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

namespace graph_detail {
// 最终版第 133～140 页：count[] 同时存入度与零入度栈的下标链接。
template <class T, class E>
int topologicalOrder(const Graph<T, E>& graph, int order[]) {
    if (!graph.IsDirected()) throw std::invalid_argument("activity network requires a directed graph");
    const int size = graph.NumberOfVertices();
    auto count = std::make_unique<int[]>(size);
    for (int vertex = 0; vertex < size; ++vertex)
        for (int next = graph.getFirstNeighbor(vertex); next != -1;
             next = graph.getNextNeighbor(vertex, next))
            ++count[next];

    int top = -1;
    for (int vertex = 0; vertex < size; ++vertex) {
        if (count[vertex] == 0) {
            count[vertex] = top;
            top = vertex;
        }
    }
    int emitted = 0;
    while (top != -1) {
        const int vertex = top;
        top = count[vertex];
        order[emitted++] = vertex;
        for (int next = graph.getFirstNeighbor(vertex); next != -1;
             next = graph.getNextNeighbor(vertex, next)) {
            if (--count[next] == 0) {
                count[next] = top;
                top = next;
            }
        }
    }
    return emitted;
}
}

// 对已建好的 AOV 网排序。输出顶点值，存在环时返回 false。
template <class T, class E>
bool TopologicalSort(const Graph<T, E>& graph, std::ostream& output = std::cout) {
    const int count = graph.NumberOfVertices();
    auto order = std::make_unique<int[]>(count);
    const int emitted = graph_detail::topologicalOrder(graph, order.get());
    for (int index = 0; index < emitted; ++index)
        output << graph.getValue(order[index]) << '\n';
    if (emitted == count) return true;
    output << "网络中有回路！\n";
    return false;
}

// 最终版第 153～155 页：按拓扑序求 Ve，逆序求 Vl，输出关键活动并返回工期。
template <class T, class E>
GraphDistance<E> CriticalPath(const Graph<T, E>& graph, std::ostream& output = std::cout) {
    using Time = GraphDistance<E>;
    const int count = graph.NumberOfVertices();
    auto order = std::make_unique<int[]>(count);
    if (graph_detail::topologicalOrder(graph, order.get()) != count)
        throw std::domain_error("CriticalPath: network has a cycle");

    auto earliest = std::make_unique<Time[]>(count); // Ve
    auto latest = std::make_unique<Time[]>(count);   // Vl
    Time duration{};
    for (int index = 0; index < count; ++index) {
        const int vertex = order[index];
        duration = std::max(duration, earliest[vertex]);
        for (int next = graph.getFirstNeighbor(vertex); next != -1;
             next = graph.getNextNeighbor(vertex, next)) {
            const auto weight = static_cast<Time>(graph.getWeight(vertex, next));
            if (weight < 0) throw std::invalid_argument("activity duration must be nonnegative");
            earliest[next] = std::max(earliest[next], earliest[vertex] + weight);
        }
    }

    // 所有汇点共用工程截止时间，单汇点时就是课件中的 Ve[n-1]。
    std::fill_n(latest.get(), count, duration);
    for (int index = count; index > 0; --index) {
        const int vertex = order[index - 1];
        for (int next = graph.getFirstNeighbor(vertex); next != -1;
             next = graph.getNextNeighbor(vertex, next)) {
            const auto weight = static_cast<Time>(graph.getWeight(vertex, next));
            latest[vertex] = std::min(latest[vertex], latest[next] - weight);
        }
    }

    for (int vertex = 0; vertex < count; ++vertex) {
        for (int next = graph.getFirstNeighbor(vertex); next != -1;
             next = graph.getNextNeighbor(vertex, next)) {
            const Time activityEarliest = earliest[vertex];
            const Time activityLatest = latest[next] - static_cast<Time>(graph.getWeight(vertex, next));
            bool critical;
            if constexpr (std::is_integral_v<E>) {
                critical = activityLatest == activityEarliest;
            } else {
                // 浮点时容许正向加法、逆向减法积累的舍入误差。
                const Time tolerance = 4 * std::numeric_limits<Time>::epsilon() * duration * count;
                critical = std::abs(activityLatest - activityEarliest) <= tolerance;
            }
            if (critical)
                output << '<' << graph.getValue(vertex) << ',' << graph.getValue(next)
                       << ">是关键活动\n";
        }
    }
    return duration;
}
