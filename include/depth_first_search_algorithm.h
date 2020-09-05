#ifndef STRONGLY_CONNECTED_COMPONENTS_DEPTH_FIRST_SEARCH_ALGORITHM_H_
#define STRONGLY_CONNECTED_COMPONENTS_DEPTH_FIRST_SEARCH_ALGORITHM_H_

#include "graph_containers.h"
#include "vertex_action.h"
#include "edge_action.h"
#include "graph_color.h"
#include "rooted_algorithm_base.h"

namespace Graph
{
    template <typename TGraph>
    class DepthFirstSearchAlgorithm :
        public RootedAlgorithmBase<TGraph>
    {
    public:
        using BaseType = RootedAlgorithmBase<TGraph>;
        using TVertexDescriptor = typename TGraph::TVertexDescriptor;
        using TEdge = typename TGraph::TEdge;

        explicit DepthFirstSearchAlgorithm(const TGraph& graph)
            : BaseType(graph)
            , initializeVertexAction_()
            , startVertexAction_()
            , discoverVertexAction_()
            , examineEdgeAction_()
            , treeEdgeAction_()
            , backEdgeAction_()
            , forwardOrCrossEdgeAction_()
            , finishVertexAction_()
            , colors_()
        {}

        std::shared_ptr<Dictionary<TVertexDescriptor, GraphColor>>
            VertexColors() const
        {
                return colors_;
        }

        GraphColor GetVertexColor(const TVertexDescriptor& vertex) const
        {
            return *colors_[vertex];
        }

        template <typename TFunc>
        void SetInitializeVertexAction(TFunc action)
        {
            initializeVertexAction_ = VertexAction<TVertexDescriptor>(action);
        }

        void ResetInitializeVertexAction()
        {
            initializeVertexAction_ = VertexAction<TVertexDescriptor>();
        }

        template <typename TFunc>
        void SetStartVertexAction(TFunc action)
        {
            startVertexAction_ = VertexAction<TVertexDescriptor>(action);
        }

        void ResetStartVertexAction()
        {
            startVertexAction_ = VertexAction<TVertexDescriptor>();
        }

        template <typename TFunc>
        void SetDiscoverVertexAction(TFunc action)
        {
            discoverVertexAction_ = VertexAction<TVertexDescriptor>(action);
        }

        void ResetDiscoverVertexAction()
        {
            discoverVertexAction_ = VertexAction<TVertexDescriptor>();
        }

        template <typename TFunc>
        void SetExamineEdgeAction(TFunc action)
        {
            examineEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>(action);
        }

        void ResetExamineEdgeAction()
        {
            examineEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>();
        }

        template <typename TFunc>
        void SetTreeEdgeAction(TFunc action)
        {
            treeEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>(action);
        }

        void ResetTreeEdgeAction()
        {
            treeEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>();
        }

        template <typename TFunc>
        void SetBackEdgeAction(TFunc action)
        {
            backEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>(action);
        }

        void ResetBackEdgeAction()
        {
            backEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>();
        }

        template <typename TFunc>
        void SetForwardOrCrossEdgeAction(TFunc action)
        {
            forwardOrCrossEdgeAction_ =
                EdgeAction<TVertexDescriptor, TEdge>(action);
        }

        void ResetForwardOrCrossEdgeAction()
        {
            forwardOrCrossEdgeAction_ = EdgeAction<TVertexDescriptor, TEdge>();
        }

        template <typename TFunc>
        void SetFinishVertexAction(TFunc action)
        {
            finishVertexAction_ = VertexAction<TVertexDescriptor>(action);
        }

        void ResetFinishVertexAction()
        {
            finishVertexAction_ = VertexAction<TVertexDescriptor>();
        }

    protected:
        void Initialize() override
        {
            colors_ = std::make_shared < Dictionary < TVertexDescriptor,
                GraphColor >> ();
            auto& colors = *colors_.get();
            for (const auto& vertex : BaseType::GetGraph().Vertices())
            {
                colors[vertex] = GraphColor::WHITE;
                initializeVertexAction_(vertex);
            }
        }

        void InternalCompute() override
        {
            TVertexDescriptor root;
            if (BaseType::TryGetRoot(root))
            {
                startVertexAction_(root);
                Visit(root);
            }
            else
            {
                auto& colors = *colors_.get();
                for (const auto& vertex : BaseType::GetGraph().Vertices())
                {
                    if (colors[vertex] == GraphColor::WHITE)
                    {
                        startVertexAction_(vertex);
                        Visit(vertex);
                    }
                }
            }
        }

    private:
        struct SearchFrame
        {
        public:
            SearchFrame(const TVertexDescriptor& vertex,
                IteratorRange<typename TGraph::ConstEdgeIterator> edges)
                : vertex(vertex)
                , edges(edges)
            {}

            TVertexDescriptor vertex;
            IteratorRange<typename TGraph::ConstEdgeIterator> edges;
        };

        void Visit(const TVertexDescriptor& root)
        {
            auto& colors = *colors_.get();
            Stack<SearchFrame> todo;
            colors[root] = GraphColor::GRAY;
            discoverVertexAction_(root);

            todo.push(SearchFrame(root, BaseType::GetGraph().OutEdges(root)));
            while (!todo.empty())
            {
                auto& frame = todo.top();
                auto vertex = frame.vertex;
                auto edgesBegin = frame.edges.begin();
                auto edgesEnd = frame.edges.end();
                todo.pop();

                while (edgesBegin != edgesEnd)
                {
                    auto target = edgesBegin->Target();
                    examineEdgeAction_(*edgesBegin);
                    auto color = colors[target];
                    if (color == GraphColor::WHITE)
                    {
                        treeEdgeAction_(*edgesBegin);
                        todo.push(SearchFrame(vertex, IteratorRange < typename
                            TGraph::ConstEdgeIterator >(++edgesBegin, edgesEnd)));
                        vertex = target;
                        colors[vertex] = GraphColor::GRAY;
                        discoverVertexAction_(vertex);
                        auto newEdgeRange = BaseType::GetGraph().OutEdges(vertex);
                        edgesBegin = newEdgeRange.begin();
                        edgesEnd = newEdgeRange.end();
                    }
                    else if (color == GraphColor::GRAY)
                    {
                        backEdgeAction_(*edgesBegin);
                        ++edgesBegin;
                    }
                    else
                    {
                        forwardOrCrossEdgeAction_(*edgesBegin);
                        ++edgesBegin;
                    }
                }

                colors[vertex] = GraphColor::BLACK;
                finishVertexAction_(vertex);
            }
        }

    private:
        VertexAction<TVertexDescriptor> initializeVertexAction_;
        VertexAction<TVertexDescriptor> startVertexAction_;
        VertexAction<TVertexDescriptor> discoverVertexAction_;
        EdgeAction<TVertexDescriptor, TEdge> examineEdgeAction_;
        EdgeAction<TVertexDescriptor, TEdge> treeEdgeAction_;
        EdgeAction<TVertexDescriptor, TEdge> backEdgeAction_;
        EdgeAction<TVertexDescriptor, TEdge> forwardOrCrossEdgeAction_;
        VertexAction<TVertexDescriptor> finishVertexAction_;
        std::shared_ptr<Dictionary<TVertexDescriptor, GraphColor>> colors_;
    };
}

#endif
