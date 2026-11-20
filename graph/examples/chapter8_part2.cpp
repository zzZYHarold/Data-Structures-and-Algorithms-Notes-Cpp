#include "graph/include/Graphlnk.h"
#include "graph/include/Graphmtx.h"
#include "graph/include/GraphTraversal.h"
#include "graph/include/MinimumSpanningTree.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

void require(bool condition) {
    if (!condition) throw std::logic_error("chapter 8 part2 example check failed");
}

template <class Exception, class Operation>
void requireThrows(Operation operation) {
    bool caught = false;
    try { operation(); } catch (const Exception&) { caught = true; }
    require(caught);
}

template <class GraphType>
void traversalChecks() {
    GraphType graph(7);
    for (char name : {'A', 'B', 'C', 'D', 'E', 'F', 'G'}) require(graph.insertVertex(name));
    // 最终版第 93 页作业；逆序头插后，两种存储均按编号从小到大访问。
    const int edges[][2] = {{5, 6}, {4, 5}, {3, 5}, {2, 4}, {1, 6},
                           {0, 4}, {0, 3}, {0, 2}, {0, 1}};
    for (const auto& edge : edges) require(graph.insertEdge(edge[0], edge[1], 1));
    require(graph.getVertexPos('E') == 4 && graph.getVertexPos('Z') == -1);
    std::ostringstream depth, breadth, components;
    DFS(graph, 'A', depth);
    BFS(graph, 'E', breadth);
    Components(graph, components);
    require(depth.str() == "A B G F D E C ");
    require(breadth.str() == "E A C F B D G ");
    require(components.str() == depth.str() + '\n');
    requireThrows<std::invalid_argument>([&] { DFS(graph, 'Z', depth); });
    requireThrows<std::invalid_argument>([&] { BFS(graph, 'Z', breadth); });

    bool visited[7]{};
    std::ostringstream helper;
    DFS(graph, 0, visited, helper);
    DFS(graph, 0, visited, helper); // 已访问的起点不再输出。
    require(helper.str() == depth.str());
    requireThrows<std::out_of_range>([&] { DFS(graph, 7, visited, helper); });
    requireThrows<std::invalid_argument>([&] { DFS(graph, 0, nullptr, helper); });

    // 三个分量，包括孤立顶点；单起点搜索不会访问其它分量。
    GraphType disconnected(6);
    for (char name : {'A', 'B', 'C', 'D', 'E', 'F'}) require(disconnected.insertVertex(name));
    require(disconnected.insertEdge(0, 1, 1) && disconnected.insertEdge(1, 2, 1));
    require(disconnected.insertEdge(3, 4, 1));
    std::ostringstream all, one;
    Components(disconnected, all);
    BFS(disconnected, 'D', one);
    require(all.str() == "A B C \nD E \nF \n" && one.str() == "D E ");
    std::ostringstream isolated;
    DFS(disconnected, 'F', isolated);
    require(isolated.str() == "F ");
    GraphType empty(0);
    std::ostringstream nothing;
    Components(empty, nothing);
    require(nothing.str().empty());
    requireThrows<std::invalid_argument>([&] { DFS(empty, 'A', nothing); });
    requireThrows<std::invalid_argument>([&] { BFS(empty, 'A', nothing); });
    std::cout << "8.3 exercise DFS(A): " << depth.str()
              << "\n8.3 exercise BFS(E): " << breadth.str() << '\n';
}

template <class T, class E>
void checkTree(Graph<T, E>& graph, const MinSpanTree<T, E>& mst, long double expectedCost) {
    const int count = graph.NumberOfVertices();
    require(mst.Size() == (count ? count - 1 : 0));
    UFSets connected(count);
    long double total = 0;
    for (int index = 0; index < mst.Size(); ++index) {
        const auto& edge = mst.getEdge(index);
        require(edge.tail >= 0 && edge.tail < count && edge.head >= 0 && edge.head < count);
        require(edge.cost == graph.getWeight(edge.tail, edge.head));
        int firstRoot = connected.Find(edge.tail), secondRoot = connected.Find(edge.head);
        require(firstRoot != secondRoot);
        connected.Union(firstRoot, secondRoot);
        total += edge.cost;
    }
    require(count == 0 || connected.SetCount() == 1);
    require(total == expectedCost);
}

template <class GraphType>
void mstChecks() {
    GraphType graph(7);
    for (int value = 0; value < 7; ++value) require(graph.insertVertex(value));
    // 完整课件第 73～74、80、83～87 页的同一网络。
    const int edges[][3] = {{0, 1, 28}, {0, 5, 10}, {1, 2, 16}, {1, 6, 14},
                           {2, 3, 12}, {3, 4, 22}, {3, 6, 18}, {4, 5, 25}, {4, 6, 24}};
    for (const auto& edge : edges) require(graph.insertEdge(edge[0], edge[1], edge[2]));
    MinSpanTree<int, int> mst(6);
    Kruskal(graph, mst);
    checkTree(graph, mst, 99);
    const int kruskal[][3] = {{0, 5, 10}, {2, 3, 12}, {1, 6, 14},
                             {1, 2, 16}, {3, 4, 22}, {4, 5, 25}};
    for (int index = 0; index < 6; ++index) {
        const auto& edge = mst.getEdge(index);
        require(edge.tail == kruskal[index][0] && edge.head == kruskal[index][1] &&
                edge.cost == kruskal[index][2]);
    }
    Prim(graph, 0, mst); // 重用输出容器，必须替换而非追加。
    checkTree(graph, mst, 99);
    const int prim[][3] = {{0, 5, 10}, {5, 4, 25}, {4, 3, 22},
                          {3, 2, 12}, {2, 1, 16}, {1, 6, 14}};
    for (int index = 0; index < 6; ++index) {
        const auto& edge = mst.getEdge(index);
        require(edge.tail == prim[index][0] && edge.head == prim[index][1] &&
                edge.cost == prim[index][2]);
    }
    for (int start = 1; start < 7; ++start) {
        Prim(graph, start, mst);
        checkTree(graph, mst, 99);
    }
    requireThrows<std::invalid_argument>([&] { Prim(graph, 99, mst); });
    require(mst.Size() == 6); // 参数校验失败保留旧结果。
    MinSpanTree<int, int> tooSmall(5);
    requireThrows<std::length_error>([&] { Kruskal(graph, tooSmall); });
    requireThrows<std::length_error>([&] { Prim(graph, 0, tooSmall); });
    requireThrows<std::out_of_range>([&] { mst.getEdge(-1); });
    requireThrows<std::out_of_range>([&] { mst.getEdge(6); });
    require(mst.Insert({0, 1, 28}) == -1);

    GraphType disconnected(4);
    for (int value : {10, 20, 30, 40}) require(disconnected.insertVertex(value));
    require(disconnected.insertEdge(0, 1, 1) && disconnected.insertEdge(1, 2, 2));
    require(disconnected.insertEdge(0, 2, 3)); // 留下一条过期候选边，另有孤立点。
    MinSpanTree<int, int> incomplete(3);
    requireThrows<std::runtime_error>([&] { Kruskal(disconnected, incomplete); });
    require(incomplete.Size() == 2);
    requireThrows<std::runtime_error>([&] { Prim(disconnected, 10, incomplete); });
    require(incomplete.Size() == 2);
    requireThrows<std::runtime_error>([&] { Prim(disconnected, 40, incomplete); });
    require(incomplete.Size() == 0);
    std::ostringstream numericDepth;
    DFS(disconnected, 20, numericDepth); // 20 是顶点值，而非下标。
    require(numericDepth.str() == "20 10 30 " || numericDepth.str() == "20 30 10 ");

    GraphType single(1);
    require(single.insertVertex(42));
    MinSpanTree<int, int> zero(0);
    Kruskal(single, zero);
    Prim(single, 42, zero);
    checkTree(single, zero, 0);
    GraphType empty(0);
    Kruskal(empty, zero);
    require(zero.Size() == 0);
    requireThrows<std::invalid_argument>([&] { Prim(empty, 0, zero); });
    requireThrows<std::invalid_argument>([] { MinSpanTree<int, int> invalid(-1); });

    GraphType tied(5);
    for (int vertex = 0; vertex < 5; ++vertex) require(tied.insertVertex(vertex));
    for (int first = 0; first < 5; ++first)
        for (int second = first + 1; second < 5; ++second)
            require(tied.insertEdge(first, second, 1));
    MinSpanTree<int, int> tiedTree(4);
    Kruskal(tied, tiedTree);
    checkTree(tied, tiedTree, 4);
    Prim(tied, 0, tiedTree);
    checkTree(tied, tiedTree, 4); // 等权时只要求正确的树与总权，不固定边顺序。

    std::cout << "8.4 Kruskal/Prim: 6 edges, total cost 99.\n";
}

template <class GraphType>
void mstHomeworkChecks() {
    // 最终版第 94 页第 2 题。顶点值为 v1～v5 的数字部分。
    GraphType primGraph(5);
    for (int value = 1; value <= 5; ++value) require(primGraph.insertVertex(value));
    // 课件矩阵对角线的 ∞ 表示无自环，只录入上三角的有限边权。
    const int primEdges[][3] = {{1, 2, 1}, {1, 3, 12}, {1, 4, 5}, {1, 5, 10},
                               {2, 3, 8}, {2, 4, 9}, {3, 5, 2}, {4, 5, 4}};
    for (const auto& edge : primEdges)
        require(primGraph.insertEdge(edge[0] - 1, edge[1] - 1, edge[2]));
    MinSpanTree<int, int> primTree(4);
    Prim(primGraph, 1, primTree); // 按顶点值从 v1 开始。
    checkTree(primGraph, primTree, 12);
    const int expectedPrim[][3] = {{1, 2, 1}, {1, 4, 5}, {4, 5, 4}, {5, 3, 2}};
    for (int index = 0; index < primTree.Size(); ++index) {
        const auto& edge = primTree.getEdge(index);
        require(primGraph.getValue(edge.tail) == expectedPrim[index][0]);
        require(primGraph.getValue(edge.head) == expectedPrim[index][1]);
        require(edge.cost == expectedPrim[index][2]);
    }

    // 最终版第 94 页第 3 题。顶点标号 1～7，数组下标 0～6。
    GraphType kruskalGraph(7);
    for (int value = 1; value <= 7; ++value) require(kruskalGraph.insertVertex(value));
    const int kruskalEdges[][3] = {{1, 2, 1}, {1, 3, 4}, {2, 3, 2}, {2, 4, 5},
                                  {2, 5, 3}, {3, 4, 4}, {3, 6, 8}, {4, 5, 2},
                                  {4, 6, 3}, {4, 7, 6}, {5, 7, 4}, {6, 7, 5}};
    for (const auto& edge : kruskalEdges)
        require(kruskalGraph.insertEdge(edge[0] - 1, edge[1] - 1, edge[2]));
    MinSpanTree<int, int> kruskalTree(6);
    Kruskal(kruskalGraph, kruskalTree);
    checkTree(kruskalGraph, kruskalTree, 15);
    // 等权边的出堆先后可不同，按无向边集合检查最终结果。
    const int expectedKruskal[][3] = {{1, 2, 1}, {2, 3, 2}, {4, 5, 2},
                                     {2, 5, 3}, {4, 6, 3}, {5, 7, 4}};
    for (const auto& expected : expectedKruskal) {
        bool found = false;
        for (int index = 0; index < kruskalTree.Size(); ++index) {
            const auto& edge = kruskalTree.getEdge(index);
            int first = kruskalGraph.getValue(edge.tail);
            int second = kruskalGraph.getValue(edge.head);
            if (first > second) std::swap(first, second);
            if (first == expected[0] && second == expected[1] && edge.cost == expected[2])
                found = true;
        }
        require(found);
    }
    std::cout << "8.4 final homework: Prim total 12, Kruskal total 15.\n";
}

int main() {
    traversalChecks<Graphmtx<char, int>>();
    traversalChecks<Graphlnk<char, int>>();
    mstChecks<Graphmtx<int, int>>();
    mstChecks<Graphlnk<int, int>>();
    mstHomeworkChecks<Graphmtx<int, int>>();
    mstHomeworkChecks<Graphlnk<int, int>>();
    Graphlnk<char, double> floating(3);
    for (char name : {'A', 'B', 'C'}) require(floating.insertVertex(name));
    require(floating.insertEdge(0, 1, 1.5) && floating.insertEdge(1, 2, 2.25));
    require(floating.insertEdge(0, 2, 9.0));
    MinSpanTree<char, double> fractional(2);
    Kruskal(floating, fractional);
    checkTree(floating, fractional, 3.75);
    Prim(floating, 'B', fractional);
    checkTree(floating, fractional, 3.75);
    std::cout << "Chapter 8 part2 (2026 final slides) checks passed.\n";
}
