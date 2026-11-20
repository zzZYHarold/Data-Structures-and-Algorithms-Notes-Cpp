#pragma once

#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <type_traits>

inline constexpr int maxWeight = 1'000'000'000; // 课件未指定数值，作为无边的哨兵。
inline constexpr int DefaultVertices = 30;

enum class GraphKind { Undirected, Directed };

// 单条边的权值与多条边的累计值使用不同的范围。
template <class E>
using GraphDistance = std::conditional_t<std::is_integral_v<E>, long long, long double>;

// 2026 秋完整课件第 25～28 页，T 为顶点值，E 为边权。默认带权无向图。
template <class T, class E>
class Graph {
    static_assert(std::is_arithmetic_v<E> && std::numeric_limits<E>::max() >= maxWeight,
                  "E must be a numeric type able to represent maxWeight");
public:
    explicit Graph(int capacity = DefaultVertices, GraphKind kind = GraphKind::Undirected)
        : maxVertices(capacity), kind(kind) {
        if (capacity < 0)
            throw std::invalid_argument("negative graph capacity");
    }
    virtual ~Graph() = default;
    // 课件的裸指针存储不可浅拷贝；本部分不额外扩展复制/移动接口。
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    bool GraphEmpty() const { return numEdges == 0; } // 第 27 页的“无边”语义。
    bool IsEmpty() const { return numVertices == 0; } // 第 23 页 ADT 的“无顶点”。
    int NumberOfVertices() const { return numVertices; }
    int NumberOfEdges() const { return numEdges; }
    bool IsDirected() const { return kind == GraphKind::Directed; }

    // part2 的 DFS/BFS/Prim 是类外函数，需要公开按值定位接口。
    virtual int getVertexPos(T vertex) const = 0;
    virtual T getValue(int vertex) const = 0;
    virtual E getWeight(int first, int second) const = 0;
    virtual int getFirstNeighbor(int vertex) const = 0;
    virtual int getNextNeighbor(int vertex, int neighbor) const = 0;
    virtual bool insertVertex(const T& vertex) = 0;
    virtual bool insertEdge(int first, int second, E cost) = 0;
    virtual bool removeVertex(int vertex) = 0;
    virtual bool removeEdge(int first, int second) = 0;

protected:
    int maxVertices;
    int numEdges = 0;
    int numVertices = 0;

    bool validVertex(int vertex) const { return vertex >= 0 && vertex < numVertices; }
    void checkVertex(int vertex) const {
        if (!validVertex(vertex))
            throw std::out_of_range("graph vertex index out of range");
    }
    bool validCost(E cost) const {
        if (!IsDirected()) return E{} < cost && cost < maxWeight;
        if constexpr (std::is_unsigned_v<E>) return cost < maxWeight;
        else return -maxWeight < cost && cost < maxWeight;
    }

    // 补充流格式：顶点数 边数，随后顶点值，随后各边的 下标 下标 权值。
    // 仅在新建的临时图上读取，派生类成功后才交换存储。
    bool readGraph(std::istream& input) {
        int vertexCount, edgeCount;
        if (!(input >> vertexCount >> edgeCount)) return false;
        if (vertexCount < 0 || vertexCount > maxVertices || edgeCount < 0 ||
            edgeCount > 1LL * vertexCount * (vertexCount - 1) / (IsDirected() ? 1 : 2)) {
            input.setstate(std::ios::failbit);
            return false;
        }
        for (int index = 0; index < vertexCount; ++index) {
            T value{};
            if (!(input >> value)) return false;
            if (!insertVertex(value)) {
                input.setstate(std::ios::failbit);
                return false;
            }
        }
        for (int index = 0; index < edgeCount; ++index) {
            int first, second;
            E cost{};
            if (!(input >> first >> second >> cost)) return false;
            if (!insertEdge(first, second, cost)) {
                input.setstate(std::ios::failbit);
                return false;
            }
        }
        return true;
    }

    void writeGraph(std::ostream& output) const {
        output << numVertices << ' ' << numEdges << '\n';
        for (int vertex = 0; vertex < numVertices; ++vertex)
            output << getValue(vertex) << '\n';
        for (int vertex = 0; vertex < numVertices; ++vertex)
            for (int neighbor = getFirstNeighbor(vertex); neighbor != -1;
                 neighbor = getNextNeighbor(vertex, neighbor))
                if (IsDirected() || vertex < neighbor)
                    output << vertex << ' ' << neighbor << ' '
                           << getWeight(vertex, neighbor) << '\n';
    }

private:
    const GraphKind kind;
};
