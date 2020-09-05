#ifndef STRONGLY_CONNECTED_COMPONENTS_ROOTED_ALGORITHM_BASE_H_
#define STRONGLY_CONNECTED_COMPONENTS_ROOTED_ALGORITHM_BASE_H_

#include "algorithm_base.h"

namespace Graph
{
    template <typename TGraph>
    class RootedAlgorithmBase : public AlgorithmBase<TGraph>
    {
    public:
        using BaseType = AlgorithmBase<TGraph>;
        using TVertexDescriptor = typename TGraph::TVertexDescriptor;

        explicit RootedAlgorithmBase(const TGraph& graph)
            : BaseType(graph)
            , root_(nullptr)
        {}

        bool TryGetRoot(TVertexDescriptor& out)
        {
            if (root_)
            {
                out = *root_;
                return true;
            }
            return false;
        }

        void SetRoot(const TVertexDescriptor& vertex)
        {
            root_ = &vertex;
        }

        void ClearRoot()
        {
            root_ = nullptr;
        }

    private:
        const TVertexDescriptor* root_;
    };
}

#endif
