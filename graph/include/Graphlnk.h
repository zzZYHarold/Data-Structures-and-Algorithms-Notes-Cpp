#pragma once

#include "graph/include/Graph.h"

#include <memory>
#include <utility>

// 第 43～44 页：一条无向边对应两个 Edge 结点，有向弧对应一个出边结点。
template <class T, class E>
struct Edge {
    int dest = -1;
    E cost{};
    Edge<T, E>* link = nullptr;
    Edge() = default;
    Edge(int destination, E weight) : dest(destination), cost(weight) {}
    bool operator!=(const Edge& other) const { return dest != other.dest; }
};

template <class T, class E>
struct Vertex {
    T data{};
    Edge<T, E>* adj = nullptr;
};

template <class T, class E>
class Graphlnk : public Graph<T, E> {
    using Graph<T, E>::maxVertices;
    using Graph<T, E>::numVertices;
    using Graph<T, E>::numEdges;
    using Graph<T, E>::validVertex;
    using Graph<T, E>::checkVertex;
    using Graph<T, E>::validCost;
public:
    explicit Graphlnk(int capacity = DefaultVertices, GraphKind kind = GraphKind::Undirected)
        : Graph<T, E>(capacity, kind),
        NodeTable(new Vertex<T, E>[maxVertices]) {}

    ~Graphlnk() override {
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            Edge<T, E>* currentEdge = NodeTable[vertex].adj;
            while (currentEdge) {
                Edge<T, E>* nextEdge = currentEdge->link;
                delete currentEdge;
                currentEdge = nextEdge;
            }
        }
        delete[] NodeTable;
    }

    T getValue(int vertex) const override {
        checkVertex(vertex);
        return NodeTable[vertex].data;
    }
    // 第 46 页仅声明。与矩阵统一为对角 0、无边 maxWeight。
    E getWeight(int first, int second) const override {
        checkVertex(first);
        checkVertex(second);
        if (first == second) return E{};
        const auto* found = findEdge(first, second);
        return found ? found->cost : static_cast<E>(maxWeight);
    }

    int getFirstNeighbor(int vertex) const override {
        if (!validVertex(vertex)) return -1;
        const auto* firstEdge = NodeTable[vertex].adj;
        return firstEdge ? firstEdge->dest : -1;
    }
    int getNextNeighbor(int vertex, int neighbor) const override {
        if (!validVertex(vertex) || !validVertex(neighbor)) return -1;
        const auto* currentEdge = findEdge(vertex, neighbor);
        return currentEdge && currentEdge->link ? currentEdge->link->dest : -1;
    }

    // 第 46 页只有增删操作声明，补写的插边采用头插法。
    bool insertVertex(const T& vertex) override {
        if (numVertices == maxVertices || getVertexPos(vertex) != -1) return false;
        NodeTable[numVertices].data = vertex;
        NodeTable[numVertices].adj = nullptr;
        ++numVertices;
        return true;
    }
    bool insertEdge(int first, int second, E cost) override {
        if (!validVertex(first) || !validVertex(second) || first == second ||
            !validCost(cost) || findEdge(first, second)) return false;
        // 两个结点都分配成功后再挂接，避免只插入半条无向边。
        auto forward = std::make_unique<Edge<T, E>>(second, cost);
        if (!this->IsDirected()) {
            auto backward = std::make_unique<Edge<T, E>>(first, cost);
            backward->link = NodeTable[second].adj;
            NodeTable[second].adj = backward.release();
        }
        forward->link = NodeTable[first].adj;
        NodeTable[first].adj = forward.release();
        ++numEdges;
        return true;
    }
    bool removeEdge(int first, int second) override {
        if (!validVertex(first) || !validVertex(second) || first == second ||
            !findEdge(first, second)) return false;
        unlinkEdge(first, second);
        if (!this->IsDirected()) unlinkEdge(second, first);
        --numEdges;
        return true;
    }
    bool removeVertex(int vertex) override {
        if (!validVertex(vertex)) return false;
        const int last = numVertices - 1;
        if (vertex != last) NodeTable[vertex].data = NodeTable[last].data;
        while (NodeTable[vertex].adj)
            removeEdge(vertex, NodeTable[vertex].adj->dest);
        if (this->IsDirected()) {
            for (int source = 0; source < numVertices; ++source)
                if (source != vertex) removeEdge(source, vertex);
        }
        // 搬移末顶点的边链表，并修正所有指向旧末下标的 dest。
        if (vertex != last) {
            NodeTable[vertex].adj = NodeTable[last].adj;
            NodeTable[last].adj = nullptr;
            for (int source = 0; source < last; ++source)
                for (auto* edge = NodeTable[source].adj; edge; edge = edge->link)
                    if (edge->dest == last) edge->dest = vertex;
        }
        --numVertices;
        return true;
    }

    friend std::istream& operator>>(std::istream& input, Graphlnk& graph) {
        Graphlnk replacement(graph.maxVertices,
                             graph.IsDirected() ? GraphKind::Directed : GraphKind::Undirected);
        if (replacement.readGraph(input)) {
            std::swap(graph.NodeTable, replacement.NodeTable);
            std::swap(graph.numVertices, replacement.numVertices);
            std::swap(graph.numEdges, replacement.numEdges);
        }
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const Graphlnk& graph) {
        graph.writeGraph(output);
        return output;
    }

    int getVertexPos(T vertex) const override {
        for (int index = 0; index < numVertices; ++index)
            if (NodeTable[index].data == vertex) return index;
        return -1;
    }

private:
    Vertex<T, E>* NodeTable;
    const Edge<T, E>* findEdge(int source, int destination) const {
        const auto* currentEdge = NodeTable[source].adj;
        while (currentEdge && currentEdge->dest != destination)
            currentEdge = currentEdge->link;
        return currentEdge;
    }
    void unlinkEdge(int source, int destination) {
        Edge<T, E>* previousEdge = nullptr;
        auto* currentEdge = NodeTable[source].adj;
        while (currentEdge && currentEdge->dest != destination) {
            previousEdge = currentEdge;
            currentEdge = currentEdge->link;
        }
        if (!currentEdge) return;
        if (previousEdge) previousEdge->link = currentEdge->link;
        else NodeTable[source].adj = currentEdge->link;
        delete currentEdge;
    }
};
