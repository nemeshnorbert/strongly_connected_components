// StronglyConnectedComponents.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "AdjacencyGraph.h"
#include "StronglyConnectedComponentAlgorithm.h"

int main()
{
    using namespace Graph;

    AdjacencyGraph<int, Edge<int>> graph;

    graph.AddVerticesAndEdge(Edge<int>(1, 2));
    graph.AddVerticesAndEdge(Edge<int>(2, 1));
    graph.AddVerticesAndEdge(Edge<int>(1, 3));

    StronglyConnectedComponentAlgorithm<AdjacencyGraph<int, Edge<int>>> algo(graph);
    algo.Compute();
    auto components = algo.GetComponents();

    for (const auto& vertex : graph.Vertices())
    {
        std::cout << "Vertex " << vertex << " belongs to component # " 
            << (*components)[vertex] << '\n';
    }

	return 0;
}
