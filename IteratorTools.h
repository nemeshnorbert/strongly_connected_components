#ifndef STRONGLY_CONNECTED_COMPONENTS_ITERATOR_TOOLS_H_
#define STRONGLY_CONNECTED_COMPONENTS_ITERATOR_TOOLS_H_

#include <iterator>

template <typename Iterator>
class IteratorRange
{
public:
    IteratorRange()
        : begin_()
        , end_()
    {}

    IteratorRange(Iterator begin, Iterator end)
        : begin_(begin)
        , end_(end)
    {}

    Iterator begin() const
    {
        return begin_;
    }

    Iterator end() const
    {
        return end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

template <typename Collection>
IteratorRange<typename Collection::iterator> MakeRange(Collection& data)
{
    return IteratorRange<typename Collection::iterator>(data.begin(), data.end());
}

template <typename Collection>
IteratorRange<typename Collection::const_iterator> MakeRange(const Collection& data)
{
    return IteratorRange<typename Collection::const_iterator>(data.begin(), data.end());
}

template <typename KeyValueIterator, bool UseKey>
class KeyValueIteratorAdaptor
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename std::conditional<UseKey,
        const typename KeyValueIterator::value_type::first_type,
        const typename KeyValueIterator::value_type::second_type>::type;
    using reference = typename std::conditional<UseKey,
        const typename KeyValueIterator::value_type::first_type&,
        const typename KeyValueIterator::value_type::second_type&>::type;
    using difference_type = typename KeyValueIterator::difference_type;
    using pointer = const typename KeyValueIterator::pointer;

    KeyValueIteratorAdaptor()
        : iter_()
    {}

    explicit KeyValueIteratorAdaptor(KeyValueIterator iter)
        : iter_(iter)
    {}

    reference operator * () const
    {
        return Dereference<UseKey>();
    }

    pointer operator -> () const
    {
        return std::addressof(**this);
    }

    KeyValueIteratorAdaptor<KeyValueIterator, UseKey>& operator ++()
    {
        ++iter_;
        return *this;
    }

    KeyValueIteratorAdaptor<KeyValueIterator, UseKey>& operator --()
    {
        --iter_;
        return *this;
    }

    KeyValueIteratorAdaptor<KeyValueIterator, UseKey> operator ++(int dummy)
    {
        auto aCopy = *this;
        ++*this;
        return aCopy;
    }

    KeyValueIteratorAdaptor<KeyValueIterator, UseKey> operator --(int dummy)
    {
        auto aCopy = *this;
        --*this;
        return aCopy;
    }

    bool operator == (const KeyValueIteratorAdaptor<KeyValueIterator, UseKey>& other)
    {
        return iter_ == other.iter_;
    }

    bool operator != (const KeyValueIteratorAdaptor<KeyValueIterator, UseKey>& other)
    {
        return iter_ != other.iter_;
    }

private:
    template <bool KeyFlag>
    typename std::enable_if<KeyFlag,
        const typename KeyValueIterator::value_type::first_type&>::type
        Dereference() const
    {
            return iter_->first;
        }

    template <bool KeyFlag>
    typename std::enable_if<!KeyFlag,
        const typename KeyValueIterator::value_type::second_type&>::type
        Dereference() const
    {
            return iter_->second;
        }

private:
    KeyValueIterator iter_;
};

template <typename KeyValueIterator>
using KeyIterator = KeyValueIteratorAdaptor<KeyValueIterator, true>;

template <typename KeyValueIterator>
using ValueIterator = KeyValueIteratorAdaptor<KeyValueIterator, false>;

#endif