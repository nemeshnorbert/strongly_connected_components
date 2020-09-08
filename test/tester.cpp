#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>

#include "adjacency_graph.h"
#include "strongly_connected_component_algorithm.h"


template <typename ValueType>
    using UniformDistribution = typename std::enable_if<
    std::is_integral<ValueType>::value || std::is_floating_point<ValueType>::value,
    typename std::conditional<
        std::is_integral<ValueType>::value,
        std::uniform_int_distribution<ValueType>,
        std::uniform_real_distribution<ValueType>
    >::type
>::type;

template <typename ValueType>
ValueType GetRandomValue(const ValueType& min, const ValueType& max)
{
    assert(min <= max);
    static std::mt19937_64 engine;
    UniformDistribution<ValueType> generator(min, max);
    return generator(engine);
}

template <typename ValueType>
Graph::AdjacencyGraph<ValueType, Graph::Edge<ValueType>> GetRandomGraph()
{
    Graph::AdjacencyGraph<ValueType, Graph::Edge<ValueType>> graph;
    size_t vertexCount = GetRandomValue<size_t>(1, 100);
    for (size_t vertex = 0; vertex < vertexCount; ++vertex)
    {
        size_t edgesCount = GetRandomValue<size_t>(0, vertexCount - 1);
        while (edgesCount > 0)
        {
            size_t destination = GetRandomValue<size_t>(0, vertexCount - 1);
            graph.AddVerticesAndEdge(
                Graph::Edge<ValueType>(ValueType(vertex), ValueType(destination)));
            --edgesCount;
        }
    }
    return graph;
}

template <typename ValueType>
void PrintGraph(
    std::ostream& out, const Graph::AdjacencyGraph<ValueType, Graph::Edge<ValueType>>& graph)
{
    using EdgeIterator = typename Graph::AdjacencyGraph<
        ValueType, Graph::Edge<ValueType>>::ConstEdgeIterator;
    for (const auto& vertex : graph.Vertices())
    {
        IteratorRange<EdgeIterator> iedges;
        graph.TryGetEdges(vertex, iedges);
        out << vertex << " -> ";
        for (EdgeIterator iedge = iedges.begin(); iedge != iedges.end(); ++iedge)
        {
            out << iedge->Target() << " ";
        }
        out << '\n';
    }
}

template <typename ValueType>
bool RunTest(
    std::ostream& out, const Graph::AdjacencyGraph<ValueType, Graph::Edge<ValueType>>& graph)
{
    using Algorithm = Graph::StronglyConnectedComponentAlgorithm<
        Graph::AdjacencyGraph<ValueType, Graph::Edge<ValueType>>>;

    try
    {
        Algorithm algo(graph);
        algo.Compute();
        auto components = algo.GetComponents();
    }
    catch (const std::exception& exc)
    {
        out << "Test failed. Reason: " << exc.what() << '\n';
        out << "Graph: \n";
        PrintGraph(out, graph);
        return false;
    }
    out << "Test passed\n";
    return true;
}


int main()
{
    for (int attempt = 0; attempt < 10; ++attempt)
    {
        if(!RunTest(std::cout, GetRandomGraph<int>()))
        {
            return 1;
        }
    }
    return 0;
}
