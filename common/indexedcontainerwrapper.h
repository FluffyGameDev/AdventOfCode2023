#pragma once

namespace AoC
{
    template <typename ContainerType, typename ValueType>
    class indexed_container_wrapper
    {
    public:
        class indexed_iterator
        {
        public:
            indexed_iterator(typename ContainerType::const_iterator iterator)
                : m_Iterator{ iterator }
                , m_Index{ 0 }
            {
            }

            const indexed_iterator& operator++()
            {
                ++m_Iterator;
                ++m_Index;
                return*this;
            }

            std::pair<const ValueType&, size_t> operator*() const
            {
                return { *m_Iterator, m_Index };
            }


            friend bool operator==(const indexed_iterator& lhs, const indexed_iterator& rhs) { return lhs.m_Iterator == rhs.m_Iterator; }
            friend bool operator!=(const indexed_iterator& lhs, const indexed_iterator& rhs) { return lhs.m_Iterator != rhs.m_Iterator; }

        private:
            typename ContainerType::const_iterator m_Iterator;
            size_t m_Index;
        };

        indexed_container_wrapper(const ContainerType& container)
            : m_Container{ container }
        {
        }

        indexed_iterator begin() { return { m_Container.begin() }; }
        indexed_iterator end() { return { m_Container.end() }; }

    private:
        const ContainerType& m_Container;
    };
}