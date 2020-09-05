#ifndef STRONGLY_CONNECTED_COMPONENTS_EDGE_H_
#define STRONGLY_CONNECTED_COMPONENTS_EDGE_H_

namespace Graph
{
    template <typename VertexDescriptor>
    class Edge
    {
    public:
        using TVertexDescriptor = VertexDescriptor;

        Edge(const TVertexDescriptor& source,
            const TVertexDescriptor& target)
            : source_(source)
            , target_(target)
        {}

        TVertexDescriptor Source() const
        {
            return source_;
        }

        TVertexDescriptor Target() const
        {
            return target_;
        }

    private:
        TVertexDescriptor source_;
        TVertexDescriptor target_;
    };
}

#endif