#include <iostream>

#include "adjacency_graph.h"
#include "strongly_connected_component_algorithm.h"


Graph::AdjacencyGraph<int, Graph::Edge<int>> ReadGraph()
{
    Graph::AdjacencyGraph<int, Graph::Edge<int>> graph;
    std::cout << "Enter vertex count: ";
    int vertexCount = 0;
    std::cin >> vertexCount;
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        int edgesCount = 0;
        std::cout << "Enter outgoing edges count for vertex " << vertex << ": ";
        std::cin >> edgesCount;
        std::cout << "Enter adjacent vertices for vertex " << vertex << ": ";
        while (edgesCount > 0)
        {
            int destination = 0;
            std::cin >> destination;
            if (!(0 <= destination && destination < vertexCount))
            {
                std::cout << "Vertex must be an integer in the range [0" << vertexCount << ")\n";
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
    std::cout << "This application detects strongly connected components of a given graph\n";
    using AlgorithmType = Graph::StronglyConnectedComponentAlgorithm<
        Graph::AdjacencyGraph<int, Graph::Edge<int>>>;

    Graph::AdjacencyGraph<int, Graph::Edge<int>> graph = ReadGraph();
    AlgorithmType algo(graph);
    algo.Compute();
    auto components = algo.GetComponents();

    for (const auto& vertex : graph.Vertices())
    {
        std::cout << "Vertex " << vertex << " belongs to component # "
            << (*components)[vertex] << '\n';
    }
}


int main()
{
    Run();
    return 0;
}


