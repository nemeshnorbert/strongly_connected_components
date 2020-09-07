#include <iostream>

#include "adjacency_graph.h"
#include "strongly_connected_component_algorithm.h"


Graph::AdjacencyGraph<int, Graph::Edge<int>> ReadGraph()
{
    std::ostream& out = std::cout;
    std::istream& in = std::cin;
    Graph::AdjacencyGraph<int, Graph::Edge<int>> graph;
    out << "Enter vertex count: ";
    int vertexCount = 0;
    in >> vertexCount;
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        int edgesCount = 0;
        out << "Enter outgoing edges count for vertex " << vertex << ": ";
        in >> edgesCount;
        out << "Enter adjacent vertices for vertex " << vertex << ": ";
        while (edgesCount > 0)
        {
            int destination = 0;
            in >> destination;
            if (!(0 <= destination && destination < vertexCount))
            {
                out << "Vertex must be an integer in the range [0" << vertexCount << ")\n";
            }
            else
            {
                --edgesCount;
                graph.AddVerticesAndEdge(Graph::Edge<int>(vertex, destination));
            }
        }
    }
    return graph;
}

void Run()
{
    std::ostream& out = std::cout;
    out << "This application detects strongly connected components of a given graph\n";
    using AlgorithmType = Graph::StronglyConnectedComponentAlgorithm<
        Graph::AdjacencyGraph<int, Graph::Edge<int>>>;

    Graph::AdjacencyGraph<int, Graph::Edge<int>> graph = ReadGraph();
    AlgorithmType algo(graph);
    algo.Compute();
    auto components = algo.GetComponents();

    for (const auto& vertex : graph.Vertices())
    {
        out << "Vertex " << vertex << " belongs to component # "
            << (*components)[vertex] << '\n';
    }
}


int main()
{
    Run();
    return 0;
}


