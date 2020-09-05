#ifndef STRONGLY_CONNECTED_COMPONENTS_EDGE_ACTION_H_
#define STRONGLY_CONNECTED_COMPONENTS_EDGE_ACTION_H_

#include <functional>

namespace Graph
{
    template <typename VertexDescriptor, typename Edge>
    class EdgeAction
    {
    public:
        using TVertexDescriptor = VertexDescriptor;
        using TEdge = Edge;

        EdgeAction()
            : action_()
        {}

        template<typename TFunc>
        explicit EdgeAction(TFunc func)
            : action_(func)
        {}

        void operator()(const TEdge& edge) const
        {
            if (action_)
            {
                action_(edge);
            }
        }

    private:
        std::function<void(const TEdge&)> action_;
    };
}

#endif