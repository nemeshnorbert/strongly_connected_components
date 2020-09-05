#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>

#include "adjacency_graph.h"
#include "strongly_connected_component_algorithm.h"

template <typename ValueType>
ValueType GetRandomValue(ValueType min, ValueType max)
{
    assert(min <= max);
    static std::mt19937_64 engine;
    std::uniform_int_distribution<ValueType> generator(min, max);
    return generator(engine);
}

Graph::AdjacencyGraph<int, Graph::Edge<int>> GetRandomGraph()
{
    Graph::AdjacencyGraph<int, Graph::Edge<int>> graph;
    int vertexCount = GetRandomValue<int>(1, 100);
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        int edgesCount = GetRandomValue<int>(0, vertexCount - 1);
        while (edgesCount > 0)
        {
            int destination = GetRandomValue<int>(0, vertexCount - 1);
            graph.AddVerticesAndEdge(Graph::Edge<int>(vertex, destination));
            --edgesCount;
        }
    }
    return graph;
}

void PrintGraph(const Graph::AdjacencyGraph<int, Graph::Edge<int>>& graph)
{
    using EdgeIterator = typename Graph::AdjacencyGraph<int, Graph::Edge<int>>::ConstEdgeIterator;
    for (const auto& vertex : graph.Vertices())
    {
        IteratorRange<EdgeIterator> iedges;
        graph.TryGetEdges(vertex, iedges);
        std::cout << vertex << " -> ";
        for (EdgeIterator iedge = iedges.begin(); iedge != iedges.end(); ++iedge)
        {
            std::cout << iedge->Target() << " ";
        }
        std::cout << '\n';
    }
}

bool RunTest(const Graph::AdjacencyGraph<int, Graph::Edge<int>>& graph)
{
    using Algorithm = Graph::StronglyConnectedComponentAlgorithm<
        Graph::AdjacencyGraph<int, Graph::Edge<int>>>;

    try
    {
        Algorithm algo(graph);
        algo.Compute();
        auto components = algo.GetComponents();
    }
    catch (const std::exception& exc)
    {
        std::cout << "Test failed. Reason: " << exc.what() << '\n';
        return false;
    }
    return true;
}


int main()
{
    for (int attempt = 0; attempt < 10; ++attempt)
    {
        auto graph = GetRandomGraph();
        if(!RunTest(graph))
        {
            std::cout << "Graph: \n";
            PrintGraph(graph);
            return 1;
        }
    }
    return 0;
}
