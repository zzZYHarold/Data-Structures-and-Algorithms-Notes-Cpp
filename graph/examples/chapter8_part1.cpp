// 2026 秋 Lesson 8 最终版，第一组示例检查 8.1/8.2 的图存储。
#include "graph/include/Graphmtx.h"
#include "graph/include/Graphlnk.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

void require(bool condition) {
    if (!condition) throw std::logic_error("chapter 8 part1 example check failed");
}

template <class GraphType>
void storageChecks() {
    GraphType graph(5);
    Graph<std::string, int>& view = graph;
    require(view.IsEmpty() && view.GraphEmpty());
    for (const auto* name : {"A", "B", "C", "D", "E"})
        require(view.insertVertex(name));
    require(!view.IsEmpty() && view.GraphEmpty()); // 课件的两种“空”判定有别。
    require(!view.insertVertex("F") && !view.insertVertex("A"));
    require(view.insertEdge(0, 1, 5) && view.insertEdge(0, 3, 6));
    require(view.insertEdge(1, 2, 8) && view.insertEdge(2, 4, 2));
    require(view.insertEdge(1, 4, 9) && view.insertEdge(3, 4, 7));
    require(view.NumberOfVertices() == 5 && view.NumberOfEdges() == 6);
    require(!view.insertEdge(1, 0, 99)); // 重复边也不能悄悄改权。
    require(!view.insertEdge(0, 0, 1) && !view.insertEdge(-1, 1, 1));
    require(!view.insertEdge(0, 5, 1) && !view.insertEdge(0, 2, 0));
    require(!view.insertEdge(0, 2, -1) && !view.insertEdge(0, 2, maxWeight));
    require(view.getWeight(1, 0) == 5 && view.getWeight(0, 0) == 0);
    require(view.getWeight(0, 2) == maxWeight);
    require(view.getFirstNeighbor(-2) == -1 && view.getFirstNeighbor(5) == -1);
    require(view.getNextNeighbor(0, -1) == -1 && view.getNextNeighbor(0, 5) == -1);
    bool caught = false;
    try { view.getValue(5); } catch (const std::out_of_range&) { caught = true; }
    require(caught);
    caught = false;
    try { view.getWeight(-2, 0); } catch (const std::out_of_range&) { caught = true; }
    require(caught);

    // 核对邻接枚举包含所有实际边，每个邻接点恰出现一次。
    for (int source = 0; source < 5; ++source) {
        bool seen[5]{};
        int count = 0;
        for (int neighbor = view.getFirstNeighbor(source); neighbor != -1;
             neighbor = view.getNextNeighbor(source, neighbor)) {
            require(neighbor >= 0 && neighbor < 5 && !seen[neighbor] && count < 5);
            seen[neighbor] = true;
            ++count;
        }
        for (int destination = 0; destination < 5; ++destination)
            require(seen[destination] ==
                    (source != destination && view.getWeight(source, destination) < maxWeight));
    }

    require(!view.removeVertex(-1) && !view.removeVertex(5));
    require(!view.removeEdge(0, 2) && !view.removeEdge(5, 0));
    require(view.removeVertex(1)); // 删除 B，末顶点 E 搬到下标 1。
    require(view.NumberOfVertices() == 4 && view.NumberOfEdges() == 3);
    require(view.getValue(1) == "E" && view.getWeight(1, 2) == 2);
    require(view.getWeight(1, 3) == 7 && view.getWeight(3, 1) == 7);
    require(view.getWeight(0, 1) == maxWeight && view.getWeight(0, 3) == 6);
    require(view.removeEdge(2, 1) && !view.removeEdge(1, 2));
    require(view.NumberOfEdges() == 2 && view.getFirstNeighbor(2) == -1);
    require(view.removeVertex(3)); // 直接删除末顶点 D，移除剩余两条边。
    require(view.NumberOfEdges() == 0 && view.NumberOfVertices() == 3);
    require(view.insertVertex("F") && view.getFirstNeighbor(3) == -1);
    require(view.insertEdge(0, 3, 4)); // 复用已删除的槽位。
    while (view.NumberOfVertices()) require(view.removeVertex(0));
    require(view.IsEmpty() && view.NumberOfEdges() == 0);
    require(view.insertVertex("A") && !view.insertVertex("A"));

    GraphType zero(0);
    require(zero.IsEmpty() && !zero.insertVertex("A") && !zero.removeVertex(0));
    caught = false;
    try { GraphType invalid(-1); } catch (const std::invalid_argument&) { caught = true; }
    require(caught);

    // 输入失败不替换原图；输入成功后支持输出再读回。
    const char* invalidInputs[] = {
        "6 0", "-1 0", "2 2 A B", "2 1 A A 0 1 5", "2 1 A B 0 2 5",
        "2 1 A B 0 0 5", "2 1 A B 0 1 0", "2 1 A B 0 1",
        "3 2 A B C 0 1 5 1 0 6"
    };
    for (const auto* text : invalidInputs) {
        std::istringstream input(text);
        input >> graph;
        require(input.fail() && graph.NumberOfVertices() == 1 && graph.getValue(0) == "A");
    }
    std::istringstream input("4 3 A B C D 0 1 1 0 3 1 1 2 1"); // 第 40 页。
    input >> graph;
    require(!input.fail() && graph.NumberOfVertices() == 4 && graph.NumberOfEdges() == 3);
    std::stringstream serialized;
    serialized << graph;
    GraphType restored(5);
    serialized >> restored;
    require(!serialized.fail() && restored.NumberOfEdges() == 3);
    for (int source = 0; source < 4; ++source) {
        require(restored.getValue(source) == graph.getValue(source));
        for (int destination = 0; destination < 4; ++destination)
            require(restored.getWeight(source, destination) == graph.getWeight(source, destination));
    }
    std::istringstream emptyInput("0 0");
    emptyInput >> restored;
    require(!emptyInput.fail() && restored.IsEmpty());
}

int main() {
    storageChecks<Graphmtx<std::string, int>>();
    storageChecks<Graphlnk<std::string, int>>();

    Graphmtx<char, int> matrix(4);
    Graphlnk<char, int> list(4);
    for (char vertex : {'A', 'B', 'C', 'D'}) {
        require(matrix.insertVertex(vertex));
        require(list.insertVertex(vertex));
    }
    // 第 40 页的无向图。矩阵按列递增，链表补充实现采用头插法。
    for (const auto& edge : {std::pair{0, 1}, {0, 3}, {1, 2}}) {
        require(matrix.insertEdge(edge.first, edge.second, 1));
        require(list.insertEdge(edge.first, edge.second, 1));
    }
    require(matrix.getFirstNeighbor(0) == 1 && matrix.getNextNeighbor(0, 1) == 3);
    require(list.getFirstNeighbor(0) == 3 && list.getNextNeighbor(0, 3) == 1);
    require(matrix.getNextNeighbor(0, 3) == -1 && list.getNextNeighbor(0, 1) == -1);
    require(list.getNextNeighbor(0, 2) == -1); // w 必须确实是链表中的邻接点。
    std::cout << "8.2 adjacency matrix storage (edge list output):\n" << matrix;
    std::cout << "8.2 adjacency list storage (edge list output):\n" << list;

    std::unique_ptr<Graph<int, double>> numeric = std::make_unique<Graphlnk<int, double>>(2);
    require(numeric->insertVertex(10) && numeric->insertVertex(20));
    require(!numeric->insertEdge(0, 1, std::numeric_limits<double>::quiet_NaN()));
    require(!numeric->insertEdge(0, 1, std::numeric_limits<double>::infinity()));
    require(numeric->insertEdge(0, 1, 1.5) && numeric->getWeight(1, 0) == 1.5);
    std::unique_ptr<Graph<int, double>> numericMatrix = std::make_unique<Graphmtx<int, double>>(2);
    require(numericMatrix->insertVertex(10) && numericMatrix->insertVertex(20));
    require(numericMatrix->insertEdge(0, 1, 2.5) && numericMatrix->getWeight(1, 0) == 2.5);
    std::cout << "Chapter 8 part1 (2026 final slides) checks passed.\n";
}
