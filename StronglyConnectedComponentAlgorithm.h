#ifndef STRONGLY_CONNECTED_COMPONENTS_STRONGLY_CONNECTED_COMPONENT_ALGORITHM_H_
#define STRONGLY_CONNECTED_COMPONENTS_STRONGLY_CONNECTED_COMPONENT_ALGORITHM_H_

#include <memory>
#include "VertexAction.h"
#include "EdgeAction.h"
#include "GraphContainers.h"
#include "AlgorithmBase.h"
#include "DepthFirstSearchAlgorithm.h"

namespace Graph
{
    template <typename TGraph>
    class StronglyConnectedComponentAlgorithm : public AlgorithmBase<TGraph>
    {
    public:
        using BaseType = AlgorithmBase<TGraph>;
        using TVertexDescriptor = typename TGraph::TVertexDescriptor;
        using TEdge = typename TGraph::TEdge;

        explicit StronglyConnectedComponentAlgorithm(const TGraph& graph)
            : BaseType(graph)
            , components_()
            , discoverTimes_()
            , roots_()
            , stack_()
            , componentsCount_(0)
            , dfsTime_(0)
        {}

        std::shared_ptr<Dictionary<TVertexDescriptor, size_t>>
            GetComponents() const
        {
                return components_;
            }

        std::shared_ptr<Dictionary<TVertexDescriptor, TVertexDescriptor>>
            GetRoots() const
        {
                return roots_;
            }

        std::shared_ptr<Dictionary<TVertexDescriptor, size_t>>
            GetDiscoverTimes() const
        {
                return discoverTimes_;
            }

        size_t GetComponentsCount() const
        {
            return componentsCount_;
        }

    protected:
        void Initialize() override
        {
            components_ = std::make_shared < Dictionary < TVertexDescriptor,
                size_t >> ();
            discoverTimes_ = std::make_shared < Dictionary < TVertexDescriptor,
                size_t >> ();
            roots_ = std::make_shared < Dictionary < TVertexDescriptor,
                TVertexDescriptor >> ();
            componentsCount_ = 0;
            dfsTime_ = 0;
        }

        void InternalCompute() override
        {
            auto dfs = DepthFirstSearchAlgorithm<TGraph>(BaseType::GetGraph());
            dfs.SetDiscoverVertexAction(
                [this](const TVertexDescriptor& vertex)
            {
                (*roots_)[vertex] = vertex;
                (*components_)[vertex] = std::numeric_limits<size_t>::max();
                (*discoverTimes_)[vertex] = dfsTime_++;
                stack_.push(vertex);
            });
            dfs.SetFinishVertexAction(
                [this](const TVertexDescriptor& vertex)
            {
                auto& components = *components_.get();
                auto& roots = *roots_.get();
                auto& discoverTimes = *discoverTimes_.get();

                auto outEdges = BaseType::GetGraph().OutEdges(vertex);
                for (auto iedge = outEdges.begin();
                    iedge != outEdges.end();
                    ++iedge)
                {
                    const auto& next = iedge->Target();
                    if (components[next] == std::numeric_limits<size_t>::max())
                    {
                        const auto& vertexRoot = roots[vertex];
                        const auto& nextRoot = roots[next];
                        roots[vertex] = discoverTimes[vertexRoot] <
                            discoverTimes[nextRoot] ?
                        vertexRoot :
                                   nextRoot;
                    }
                }

                if (roots[vertex] == vertex)
                {
                    TVertexDescriptor otherVertex;
                    do
                    {
                        otherVertex = stack_.top();
                        stack_.pop();
                        components[otherVertex] = componentsCount_;
                    } while (otherVertex != vertex);
                    ++componentsCount_;
                }
            });
            dfs.Compute();
        }

    private:
        std::shared_ptr<Dictionary<TVertexDescriptor, size_t>> components_;
        std::shared_ptr<Dictionary<TVertexDescriptor, size_t>> discoverTimes_;
        std::shared_ptr<Dictionary<TVertexDescriptor, TVertexDescriptor>> roots_;
        Stack<TVertexDescriptor> stack_;
        size_t componentsCount_;
        size_t dfsTime_;
    };
}

#endif