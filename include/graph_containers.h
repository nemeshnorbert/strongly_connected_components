#ifndef STRONGLY_CONNECTED_COMPONENTS_GRAPH_CONTAINERS_H_
#define STRONGLY_CONNECTED_COMPONENTS_GRAPH_CONTAINERS_H_

#include <unordered_map>
#include <list>
#include <stack>

namespace Graph
{
    template <typename TKey, typename TValue>
    using Dictionary = std::unordered_map<TKey, TValue>;

    template <typename TValue>
    using List = std::list<TValue>;

    template <typename TValue>
    using Stack = std::stack<TValue>;
}

#endif