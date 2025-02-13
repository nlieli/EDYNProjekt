#pragma once
#include <memory>
#include <concepts>
#include <type_traits>

namespace ep
{
    template <typename T>
    concept ZeroAssignable = (std::is_integral_v<T> || std::is_floating_point_v<T>);

    template <typename T>
    concept BracketAndSizeAccessible = requires (T con)
    {
        con[0];
        con.size();
    };

    template <typename container>
    class Array
    {
    private:
        std::unique_ptr<container*> m_LocalBuffer;
        size_t m_dim1_size, m_dim2_size, m_dim3_size;

        void Init(container* con) requires ZeroAssignable<container>
        {
            (*con) = 0;
        }
         
        void Init(container* con) requires BracketAndSizeAccessible<container> 
        {
            for (int i = 0; i < con->size(); ++i)
                (*con)[i] = 0;
        }

    public:
        Array(size_t dim1_size, size_t dim2_size)
        : m_dim1_size(dim1_size), m_dim2_size(dim2_size), m_dim3_size(0)
        {
            container* data = new container[dim1_size * dim2_size];
            Init(data);
            m_LocalBuffer = std::make_unique<container*>(data);
        }

        Array(size_t dim1_size, size_t dim2_size, size_t dim3_size)
        : m_dim1_size(dim1_size), m_dim2_size(dim2_size), m_dim3_size(dim3_size)
        {
            container* data = new container[dim1_size * dim2_size * dim3_size];
            Init(data);
            m_LocalBuffer = std::make_unique<container*>(data);
        }

        container& operator()(size_t dim1_index, size_t dim2_index)
        {
            if (m_dim1_size < dim1_index) { __debugbreak(); }
            if (m_dim2_size < dim2_index) { __debugbreak(); }

            container result = (*m_LocalBuffer)[(dim1_index * dim2_index) + dim2_index];
            return result;
        }

        container& operator()(size_t dim1_index, size_t dim2_index, size_t dim3_index)
        {
            if (m_dim1_size < dim1_index) { __debugbreak(); }
            if (m_dim2_size < dim2_index) { __debugbreak(); }
            if (m_dim3_size < dim3_index) { __debugbreak(); }

            container result = (*m_LocalBuffer)[(dim1_index * dim2_index * dim3_index) + (dim2_index * dim3_index) + dim3_index];
            return result;
        }

        container& operator[](size_t linear_index)
        {
            if (m_dim1_size * m_dim2_size * m_dim3_size < linear_index) { __debugbreak(); }

            return (*m_LocalBuffer)[linear_index];
        }

        size_t size() const
        {
            return m_dim1_size * m_dim2_size * m_dim3_size;
        }

    };
}



