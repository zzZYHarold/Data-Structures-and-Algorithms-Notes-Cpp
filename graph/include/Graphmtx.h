#pragma once

#include "graph/include/Graph.h"

#include <memory>
#include <utility>

// 第 33～38 页：顶点数组 + 手写二维邻接矩阵。
template <class T, class E>
class Graphmtx : public Graph<T, E> {
    using Graph<T, E>::maxVertices;
    using Graph<T, E>::numVertices;
    using Graph<T, E>::numEdges;
    using Graph<T, E>::validVertex;
    using Graph<T, E>::checkVertex;
    using Graph<T, E>::validCost;
public:
    explicit Graphmtx(int capacity = DefaultVertices, GraphKind kind = GraphKind::Undirected)
        : Graph<T, E>(capacity, kind) {
        auto vertices = std::make_unique<T[]>(maxVertices);
        auto rows = std::make_unique<E*[]>(maxVertices);
        try {
            for (int row = 0; row < maxVertices; ++row) {
                rows[row] = new E[maxVertices];
                for (int col = 0; col < maxVertices; ++col)
                    rows[row][col] = row == col ? E{} : static_cast<E>(maxWeight);
            }
        } catch (...) {
            for (int row = 0; row < maxVertices; ++row) delete[] rows[row];
            throw;
        }
        VerticesList = vertices.release();
        Edge = rows.release();
    }

    ~Graphmtx() override {
        for (int row = 0; row < maxVertices; ++row) delete[] Edge[row];
        delete[] Edge;
        delete[] VerticesList;
    }

    T getValue(int vertex) const override {
        checkVertex(vertex);
        return VerticesList[vertex];
    }
    E getWeight(int first, int second) const override {
        checkVertex(first);
        checkVertex(second);
        return Edge[first][second];
    }

    int getFirstNeighbor(int vertex) const override {
        if (!validVertex(vertex)) return -1;
        for (int col = 0; col < numVertices; ++col)
            if (col != vertex && Edge[vertex][col] < maxWeight)
                return col;
        return -1;
    }
    int getNextNeighbor(int vertex, int neighbor) const override {
        if (!validVertex(vertex) || !validVertex(neighbor)) return -1;
        for (int col = neighbor + 1; col < numVertices; ++col)
            if (col != vertex && Edge[vertex][col] < maxWeight)
                return col;
        return -1;
    }

    // 补充：课件第 35 页只有增删操作声明。
    bool insertVertex(const T& vertex) override {
        if (numVertices == maxVertices || getVertexPos(vertex) != -1) return false;
        VerticesList[numVertices] = vertex;
        for (int index = 0; index <= numVertices; ++index) {
            Edge[numVertices][index] = Edge[index][numVertices] =
                index == numVertices ? E{} : static_cast<E>(maxWeight);
        }
        ++numVertices;
        return true;
    }
    bool insertEdge(int first, int second, E cost) override {
        if (!validVertex(first) || !validVertex(second) || first == second ||
            !validCost(cost) || Edge[first][second] < maxWeight) return false;
        Edge[first][second] = cost;
        if (!this->IsDirected()) Edge[second][first] = cost;
        ++numEdges;
        return true;
    }
    bool removeEdge(int first, int second) override {
        if (!validVertex(first) || !validVertex(second) || first == second ||
            Edge[first][second] == maxWeight) return false;
        Edge[first][second] = static_cast<E>(maxWeight);
        if (!this->IsDirected()) Edge[second][first] = static_cast<E>(maxWeight);
        --numEdges;
        return true;
    }
    bool removeVertex(int vertex) override {
        if (!validVertex(vertex)) return false;
        const int last = numVertices - 1;
        if (vertex != last) VerticesList[vertex] = VerticesList[last];
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (neighbor == vertex) continue;
            if (Edge[vertex][neighbor] < maxWeight) --numEdges;
            if (this->IsDirected() && Edge[neighbor][vertex] < maxWeight) --numEdges;
        }
        // 用末顶点填补空位，同时搬移其行、列；末顶点与被删顶点的边不保留。
        if (vertex != last) {
            for (int neighbor = 0; neighbor < last; ++neighbor)
                if (neighbor != vertex) {
                    Edge[vertex][neighbor] = Edge[last][neighbor];
                    Edge[neighbor][vertex] = Edge[neighbor][last];
                }
            Edge[vertex][vertex] = E{};
        }
        for (int index = 0; index < numVertices; ++index)
            Edge[last][index] = Edge[index][last] =
                index == last ? E{} : static_cast<E>(maxWeight);
        --numVertices;
        return true;
    }

    friend std::istream& operator>>(std::istream& input, Graphmtx& graph) {
        Graphmtx replacement(graph.maxVertices,
                             graph.IsDirected() ? GraphKind::Directed : GraphKind::Undirected);
        if (replacement.readGraph(input)) {
            std::swap(graph.VerticesList, replacement.VerticesList);
            std::swap(graph.Edge, replacement.Edge);
            std::swap(graph.numVertices, replacement.numVertices);
            std::swap(graph.numEdges, replacement.numEdges);
        }
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const Graphmtx& graph) {
        graph.writeGraph(output);
        return output;
    }

    int getVertexPos(T vertex) const override {
        for (int index = 0; index < numVertices; ++index)
            if (VerticesList[index] == vertex) return index;
        return -1;
    }

private:
    T* VerticesList = nullptr;
    E** Edge = nullptr;
};
