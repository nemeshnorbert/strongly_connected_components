#ifndef STRONGLY_CONNECTED_COMPONENTS_VERTEX_ACTION_H_
#define STRONGLY_CONNECTED_COMPONENTS_VERTEX_ACTION_H_

#include <functional>

namespace Graph
{
    template <typename VertexDescriptor>
    class VertexAction
    {
    public:
        using TVertexDescriptor = VertexDescriptor;

        VertexAction()
            : action_()
        {}

        template <typename TFunc>
        explicit VertexAction(TFunc func)
            : action_(func)
        {}

        void operator()(const TVertexDescriptor& vertex) const
        {
            if (action_)
            {
                action_(vertex);
            }
        }

    private:
        std::function<void(const TVertexDescriptor&)> action_;
    };
}

#endif