#ifndef STRONGLY_CONNECTED_COMPONENTS_ADJACENCY_GRAPH_H_
#define STRONGLY_CONNECTED_COMPONENTS_ADJACENCY_GRAPH_H_

#include "iterator_tools.h"
#include "graph_containers.h"
#include "edge.h"

namespace Graph
{
    template <typename VertexDescriptor, typename Edge>
    class AdjacencyGraph
    {
    public:
        using TVertexDescriptor = VertexDescriptor;
        using TEdge = Edge;

        using ConstVertexIterator = KeyIterator < typename Dictionary < TVertexDescriptor,
            List<TEdge >> ::const_iterator>;
        using ConstEdgeIterator = typename List<TEdge>::const_iterator;

        AdjacencyGraph()
            : AdjacencyGraph(false)
        {}

        explicit AdjacencyGraph(bool allowParallelEdges)
            : allowParallelEdges_(allowParallelEdges)
            , vertexEdges_()
            , edgeCount_(0)
        {}

        bool IsDirected() const
        {
            return true;
        }

        bool AllowParallelEdges() const
        {
            return allowParallelEdges_;
        }

        size_t VertexCount()  const
        {
            return vertexEdges_.size();
        }

        bool IsVerticesEmpty() const
        {
            return VertexCount() == 0;
        }

        IteratorRange<ConstVertexIterator> Vertices() const
        {
            return IteratorRange<ConstVertexIterator>(
                ConstVertexIterator(vertexEdges_.begin()),
                ConstVertexIterator(vertexEdges_.end()));
        }

        bool ContainsVertex(const TVertexDescriptor& vertex) const
        {
            return vertexEdges_.find(vertex) != vertexEdges_.end();
        }

        bool IsOutEdgesEmpty(const TVertexDescriptor& vertex) const
        {
            auto iList = vertexEdges_.find(vertex);
            return iList->second.empty();
        }

        size_t OutDegree(const TVertexDescriptor& vertex) const
        {
            auto iList = vertexEdges_.find(vertex);
            return iList->second.size();
        }

        IteratorRange<ConstEdgeIterator> OutEdges(const TVertexDescriptor& vertex) const
        {
            auto iList = vertexEdges_.find(vertex);
            return MakeRange(iList->second);
        }

        bool TryGetEdges(const TVertexDescriptor& vertex,
            IteratorRange<ConstEdgeIterator>& range) const
        {
            auto iList = vertexEdges_.find(vertex);
            if (iList != vertexEdges_.end())
            {
                range = MakeRange(iList->second);
                return true;
            }
            return false;
        }

        size_t EdgeCount() const
        {
            return edgeCount_;
        }

        List<TEdge> GetEdges() const
        {
            List<TEdge> edges;
            for (const auto& vertexEdgesPairs : vertexEdges_)
            {
                for (const auto& edge : vertexEdgesPairs.second)
                {
                    edges.push_back(edge);
                }
            }
            return edges;
        }

        bool ContainsEdge(const TVertexDescriptor& source,
            const TVertexDescriptor& target) const
        {
            IteratorRange<ConstEdgeIterator> range;
            if (!TryGetEdges(source, range))
            {
                return false;
            }
            for (const auto& edge : range)
            {
                if (edge.Target() == target)
                {
                    return true;
                }
            }
            return false;
        }

        bool ContainsEdge(const TEdge& edge) const
        {
            return ContainsEdge(edge.Source(), edge.Target());
        }

        bool TryGetEdges(const TVertexDescriptor& source,
            const TVertexDescriptor& target,
            List<TEdge>& edges) const
        {
            edges.clear();
            IteratorRange<ConstEdgeIterator> range;
            if (TryGetEdges(source, range))
            {
                for (const auto& edge : range)
                {
                    edges.push_back(edge);
                }
                return true;
            }
            return false;
        }

        bool AddVertex(const TVertexDescriptor& vertex)
        {
            if (ContainsVertex(vertex))
            {
                return false;
            }
            vertexEdges_[vertex] = List<TEdge>();
            return true;
        }

        template <typename TIterator>
        size_t AddVertexRange(TIterator begin, TIterator end)
        {
            size_t count = 0;
            for (auto ivertex = begin; ivertex != end; ++ivertex)
            {
                if (AddVertex(*ivertex))
                {
                    ++count;
                }
            }
            return count;
        }

        bool RemoveVertex(const TVertexDescriptor& vertex)
        {
            if (!ContainsVertex(vertex))
            {
                return false;
            }

            for (auto& vertexEdgesPiar : vertexEdges_)
            {
                if (vertexEdgesPiar.first != vertex)
                {
                    auto& edges = vertexEdgesPiar.second;
                    for (auto iedge = edges.begin(); iedge != edges.end();)
                    {
                        if (iedge->Target() == vertex)
                        {
                            iedge = edges.erase(iedge);
                            --edgeCount_;
                        }
                        else
                        {
                            ++iedge;
                        }
                    }
                }
                else
                {
                    auto& edges = vertexEdges_[vertex];
                    edgeCount_ -= edges.size();
                    edges.clear();
                }
            }
            return true;
        }

        template <typename Predicate>
        size_t RemoveVertexIf(Predicate pred)
        {
            size_t count = 0;
            for (const auto& vertex : Vertices())
            {
                if (pred(vertex))
                {
                    RemoveVertex(vertex);
                    ++count;
                }
            }
            return count;
        }

        bool AddVerticesAndEdge(const TEdge& edge)
        {
            AddVertex(edge.Source());
            AddVertex(edge.Target());
            return AddEdge(edge);
        }

        template <typename TIterator>
        size_t AddVerticesAndEdgeRange(TIterator begin, TIterator end)
        {
            size_t count = 0;
            for (auto iedge = begin; iedge != end; ++iedge)
            {
                if (AddVerticesAndEdge(*iedge))
                {
                    ++count;
                }
            }
            return count;
        }

        bool AddEdge(const TEdge& edge)
        {
            if (!allowParallelEdges_)
            {
                if (ContainsEdge(edge))
                {
                    return false;
                }
            }
            vertexEdges_[edge.Source()].push_back(edge);
            ++edgeCount_;
            return true;
        }

        template <typename TIterator>
        size_t AddEdgeRange(TIterator begin, TIterator end)
        {
            size_t count = 0;
            for (auto iedge = begin; iedge != end; ++iedge)
            {
                if (AddEdge(*iedge))
                {
                    ++count;
                }
            }
            return count;
        }

        bool RemoveEdge(const TEdge& edge)
        {
            if (ContainsVertex(edge.Source()))
            {
                auto& edges = vertexEdges_[edge.Source()];
                for (auto iedge = edges.begin(); iedge != edges.end(); ++iedge)
                {
                    if (iedge->Source() == edge.Source() &&
                        iedge->Target() == edge.Target())
                    {
                        edges.erase(iedge);
                        --edgeCount_;
                        return true;
                    }
                }
            }
            return false;
        }

        template <typename Predicate>
        size_t RemoveEdgeIf(Predicate pred)
        {
            size_t count = 0;
            for (auto& vertexEdgesPair : vertexEdges_)
            {
                for (auto& edge : vertexEdgesPair->second)
                {
                    if (pred(edge))
                    {
                        if (RemoveEdge(edge))
                        {
                            ++count;
                        }
                    }
                }
            }
            return count;
        }

        void ClearOutEdges(const TVertexDescriptor& vertex)
        {
            auto& edges = vertexEdges_[vertex];
            size_t toDelete = edges.size();
            edges.clear();
            edgeCount_ -= toDelete;
        }

        template <typename Predicate>
        size_t RemoveOutEdgesIf(const TVertexDescriptor& vertex, Predicate pred)
        {
            size_t count = 0;
            auto& edges = vertexEdges_[vertex];
            for (auto& edge : edges)
            {
                if (pred(edge))
                {
                    if (RemoveEdge(edge))
                    {
                        ++count;
                    }
                }
            }
            return count;
        }

        void Clear()
        {
            vertexEdges_.clear();
            edgeCount_ = 0;
        }
    private:
        bool allowParallelEdges_;
        Dictionary<TVertexDescriptor, List<TEdge>> vertexEdges_;
        size_t edgeCount_;
    };
}

#endif
