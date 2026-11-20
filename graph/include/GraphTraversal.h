#pragma once

#include "graph/include/Graph.h"
#include "queue/include/LinkedQueue.h"

#include <iostream>
#include <memory>

// 完整课件第 57 页：递归过程，vertex 是下标。
// visited 必须指向至少 NumberOfVertices() 个已初始化的 bool。
template <class T, class E>
void DFS(Graph<T, E>& graph, int vertex, bool visited[], std::ostream& output = std::cout) {
    if (vertex < 0 || vertex >= graph.NumberOfVertices())
        throw std::out_of_range("DFS vertex index out of range");
    if (!visited)
        throw std::invalid_argument("null DFS visited array");
    if (visited[vertex])
        return;

    output << graph.getValue(vertex) << ' ';

    visited[vertex] = true;
    for (int neighbor = graph.getFirstNeighbor(vertex); neighbor != -1;
         neighbor = graph.getNextNeighbor(vertex, neighbor))
        if (!visited[neighbor])
            DFS(graph, neighbor, visited, output);
}

// 第 56 页：主过程，startValue 是顶点值，仅访问起点所在的连通分量。
template <class T, class E>
void DFS(Graph<T, E>& graph, const T& startValue, std::ostream& output = std::cout) {
    const int start = graph.getVertexPos(startValue);
    if (start == -1)
        throw std::invalid_argument("DFS start vertex not found");
    auto visited = std::make_unique<bool[]>(graph.NumberOfVertices());
    DFS(graph, start, visited.get(), output);
}

// 第 61～62 页：入队时标记，避免同一顶点被重复入队。
template <class T, class E>
void BFS(Graph<T, E>& graph, const T& startValue, std::ostream& output = std::cout) {
    int current = graph.getVertexPos(startValue);
    if (current == -1) throw std::invalid_argument("BFS start vertex not found");
    auto visited = std::make_unique<bool[]>(graph.NumberOfVertices());
    LinkedQueue<int> pending;
    output << graph.getValue(current) << ' ';
    visited[current] = true;
    pending.EnQueue(current);
    while (pending.DeQueue(current)) {
        for (int neighbor = graph.getFirstNeighbor(current); neighbor != -1;
             neighbor = graph.getNextNeighbor(current, neighbor)) {
            if (!visited[neighbor]) {
                output << graph.getValue(neighbor) << ' ';
                visited[neighbor] = true;
                pending.EnQueue(neighbor);
            }
        }
    }
}

// 第 65 页：依次从尚未访问的顶点开始 DFS，每个连通分量输出一行。
template <class T, class E>
void Components(Graph<T, E>& graph, std::ostream& output = std::cout) {
    if (graph.IsDirected()) throw std::invalid_argument("Components requires an undirected graph");
    const int count = graph.NumberOfVertices();
    auto visited = std::make_unique<bool[]>(count);
    for (int vertex = 0; vertex < count; ++vertex) {
        if (!visited[vertex]) {
            DFS(graph, vertex, visited.get(), output);
            output << '\n'; // 补全课件未定义的 OutputNewComponent()。
        }
    }
}
