#ifndef STRONGLY_CONNECTED_COMPONENTS_ALGORITHM_BASE_H_
#define STRONGLY_CONNECTED_COMPONENTS_ALGORITHM_BASE_H_

namespace Graph
{
    template <typename TGraph>
    class AlgorithmBase
    {
    protected:
        explicit AlgorithmBase(const TGraph& graph)
            : graph_(graph)
        {}

    public:
        const TGraph& GetGraph() const
        {
            return graph_;
        }

        void Compute()
        {
            Initialize();
            InternalCompute();
            Clear();
        }

    protected:
        virtual void Initialize()
        {}

        virtual void InternalCompute() = 0;

        virtual void Clear()
        {}

    private:
        const TGraph& graph_;
    };
}

#endif