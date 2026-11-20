#include "graph/include/ActivityNetwork.h"
#include "graph/include/Graphlnk.h"
#include "graph/include/Graphmtx.h"
#include "graph/include/ShortestPath.h"

#include <array>
#include <source_location>
#include <sstream>
#include <string>

void require(bool condition, std::source_location location = std::source_location::current()) {
    if (!condition) throw std::logic_error("chapter 8 check failed at line " +
                                           std::to_string(location.line()));
}

template <class Exception, class Operation>
void requireThrows(Operation operation) {
    try { operation(); } catch (const Exception&) { return; }
    throw std::logic_error("expected exception was not thrown");
}

template <class GraphType>
void shortestPathExamples() {
    GraphType dijkstra(6, GraphKind::Directed);
    for (int vertex = 0; vertex < 6; ++vertex) require(dijkstra.insertVertex(vertex));
    // 最终版第 98～108 页，另加孤立顶点 5。
    for (const auto& arc : {std::array{0, 1, 10}, {0, 3, 30}, {0, 4, 100}, {1, 2, 50},
                           {2, 4, 10}, {3, 2, 20}, {3, 4, 60}})
        require(dijkstra.insertEdge(arc[0], arc[1], arc[2]));
    GraphDistance<int> dist[6];
    int path[6];
    ShortestPath(dijkstra, 0, dist, path);
    const GraphDistance<int> expectedDist[] = {0, 10, 50, 30, 60, unreachableDistance<int>};
    const int expectedPath[] = {-1, 0, 3, 0, 2, -1};
    for (int vertex = 0; vertex < 6; ++vertex)
        require(dist[vertex] == expectedDist[vertex] && path[vertex] == expectedPath[vertex]);
    requireThrows<std::out_of_range>([&] { ShortestPath(dijkstra, 6, dist, path); });

    // 最终版第 109 页作业，按 a,b,c,d,e,f,h 编号。
    GraphType exercise(7);
    for (int vertex = 0; vertex < 7; ++vertex) require(exercise.insertVertex(vertex));
    for (const auto& edge : {std::array{0, 1, 1}, {0, 5, 2}, {1, 5, 5}, {1, 2, 5},
                            {5, 2, 6}, {2, 3, 3}, {2, 4, 4}, {2, 6, 7}, {3, 4, 8}, {4, 6, 3}})
        require(exercise.insertEdge(edge[0], edge[1], edge[2]));
    GraphDistance<int> exerciseDist[7];
    int exercisePath[7];
    ShortestPath(exercise, 0, exerciseDist, exercisePath);
    const int exerciseExpected[] = {0, 1, 6, 9, 10, 2, 13};
    for (int vertex = 0; vertex < 7; ++vertex) require(exerciseDist[vertex] == exerciseExpected[vertex]);

    // 最终版第 116 页初始矩阵和最后一轮 A(3)。
    GraphType floyd(4, GraphKind::Directed);
    for (int vertex = 0; vertex < 4; ++vertex) require(floyd.insertVertex(vertex));
    for (const auto& arc : {std::array{0, 1, 1}, {0, 3, 4}, {1, 2, 9}, {1, 3, 2},
                           {2, 0, 3}, {2, 1, 5}, {2, 3, 8}, {3, 2, 6}})
        require(floyd.insertEdge(arc[0], arc[1], arc[2]));
    GraphDistance<int> distances[4][4];
    int previous[4][4];
    GraphDistance<int>* distanceRows[] = {distances[0], distances[1], distances[2], distances[3]};
    int* previousRows[] = {previous[0], previous[1], previous[2], previous[3]};
    Floyd(floyd, distanceRows, previousRows);
    const int expected[4][4] = {{0, 1, 9, 3}, {11, 0, 8, 2}, {3, 4, 0, 6}, {9, 10, 6, 0}};
    for (int from = 0; from < 4; ++from)
        for (int to = 0; to < 4; ++to)
            require(distances[from][to] == expected[from][to]);
    require(previous[1][0] == 2 && previous[1][2] == 3 && previous[1][3] == 1);

    GraphType boundary(4, GraphKind::Directed);
    for (int vertex = 0; vertex < 4; ++vertex) require(boundary.insertVertex(vertex));
    require(boundary.insertEdge(0, 1, 0) && boundary.insertEdge(1, 2, -2));
    require(boundary.getFirstNeighbor(0) == 1 && boundary.getWeight(1, 0) == maxWeight);
    Floyd(boundary, distanceRows, previousRows);
    require(distances[0][2] == -2 && previous[0][2] == 1);
    require(distances[3][2] == unreachableDistance<int> && previous[3][2] == -1);
    requireThrows<std::invalid_argument>([&] { ShortestPath(boundary, 3, dist, path); });
    require(boundary.insertEdge(2, 0, 1));
    requireThrows<std::domain_error>([&] { Floyd(boundary, distanceRows, previousRows); });
    require(boundary.removeEdge(1, 2) && boundary.removeEdge(2, 0));
    require(boundary.insertEdge(1, 2, 900'000'000) && boundary.insertEdge(2, 3, 900'000'000));
    require(boundary.insertEdge(1, 0, 3));
    ShortestPath(boundary, 0, dist, path);
    Floyd(boundary, distanceRows, previousRows);
    require(dist[1] == 0 && path[1] == 0 && dist[3] == 1'800'000'000LL);
    require(distances[0][3] == dist[3] && distances[1][0] == 3);
    require(boundary.removeEdge(0, 1) && boundary.insertEdge(0, 1, 900'000'000));
    ShortestPath(boundary, 0, dist, path);
    Floyd(boundary, distanceRows, previousRows);
    require(dist[3] == 2'700'000'000LL && distances[0][3] == dist[3]);

    // 删除中间顶点时移除入/出弧，再搬移末顶点的入/出弧。
    require(boundary.insertEdge(3, 0, 7) && boundary.insertEdge(0, 3, 8));
    require(boundary.removeVertex(1) && boundary.getVertexPos(3) == 1);
    require(boundary.NumberOfEdges() == 3);
    require(boundary.getWeight(1, 0) == 7 && boundary.getWeight(0, 1) == 8);
    require(boundary.getWeight(2, 1) == 900'000'000 && boundary.getWeight(1, 2) == maxWeight);
    require(boundary.insertEdge(1, 2, 0)); // 3 顶点 4 条弧，超过无向图的边数上限。
    std::stringstream stream;
    stream << boundary;
    GraphType restored(4, GraphKind::Directed);
    stream >> restored;
    require(!stream.fail() && restored.NumberOfEdges() == 4 && restored.getWeight(1, 0) == 7);
}

template <class GraphType>
void activityExamples() {
    // 最终版第 132～137 页，课件的零入度栈顺序。
    GraphType aov(6, GraphKind::Directed);
    for (int vertex = 0; vertex < 6; ++vertex) require(aov.insertVertex(vertex));
    for (const auto& arc : {std::array{0, 1}, {0, 3}, {1, 5}, {2, 1},
                           {2, 5}, {4, 0}, {4, 1}, {4, 5}})
        require(aov.insertEdge(arc[0], arc[1], 1));
    std::ostringstream order;
    require(TopologicalSort(aov, order) && order.str() == "4\n0\n3\n2\n1\n5\n");
    require(aov.insertEdge(5, 0, 1));
    require(!TopologicalSort(aov, order));
    requireThrows<std::domain_error>([&] { CriticalPath(aov, order); });

    // 最终版第 151～152 页。逆序插入事件，验证算法先取得拓扑序。
    GraphType aoe(8, GraphKind::Directed);
    for (int event = 8; event >= 1; --event) require(aoe.insertVertex(event));
    const int activities[][3] = {{1, 2, 8}, {1, 3, 12}, {2, 4, 14}, {3, 4, 10},
                                 {3, 6, 28}, {4, 6, 8}, {4, 5, 6}, {5, 7, 18},
                                 {6, 7, 6}, {7, 8, 12}};
    for (const auto& edge : activities)
        require(aoe.insertEdge(8 - edge[0], 8 - edge[1], edge[2]));
    std::ostringstream critical;
    require(CriticalPath(aoe, critical) == 58);
    const std::string result = critical.str();
    require(std::count(result.begin(), result.end(), '\n') == 9);
    for (const auto& edge : activities) {
        const auto label = "<" + std::to_string(edge[0]) + "," + std::to_string(edge[1]) + ">";
        require((result.find(label) != std::string::npos) == (edge[0] != 4 || edge[1] != 6));
    }

    GraphType empty(0, GraphKind::Directed);
    std::ostringstream none;
    require(TopologicalSort(empty, none) && CriticalPath(empty, none) == 0 && none.str().empty());
    GraphType undirected;
    requireThrows<std::invalid_argument>([&] { TopologicalSort(undirected, none); });
}

int main() {
    shortestPathExamples<Graphmtx<int, int>>();
    shortestPathExamples<Graphlnk<int, int>>();
    activityExamples<Graphmtx<int, int>>();
    activityExamples<Graphlnk<int, int>>();

    // 多汇点共用工期，零时长活动也可以是关键活动。
    Graphlnk<int, double> project(5, GraphKind::Directed);
    for (int vertex = 0; vertex < 5; ++vertex) require(project.insertVertex(vertex));
    require(project.insertEdge(0, 1, 0.1) && project.insertEdge(1, 2, 0.2));
    require(project.insertEdge(2, 3, 0) && project.insertEdge(0, 4, 0.2));
    std::ostringstream activities;
    require(std::abs(CriticalPath(project, activities) - 0.3L) < 1e-15L);
    require(activities.str() == "<0,1>是关键活动\n<1,2>是关键活动\n<2,3>是关键活动\n");
    require(project.insertEdge(4, 3, -1));
    requireThrows<std::invalid_argument>([&] { CriticalPath(project, activities); });
    std::cout << "8.5 shortest paths and 8.6 activity networks checks passed.\n";
}
