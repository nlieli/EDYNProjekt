#pragma once
#include <memory>
#include <concepts>
#include "../Utility.h"

namespace ep
{
    template <typename container>
    class Array
    {
    private:
        std::unique_ptr<container[]> m_LocalBuffer;
        size_t m_dim1_size, m_dim2_size, m_dim3_size;

        // overloads allow the array to hold different types of data (either numbers or vectors)
        void Init(container* data, size_t size) requires Number<container>
        {
            for (size_t i = 0; i < size; ++i)
                data[i] = 0;
        }
         
        void Init(container* data, size_t size) requires Vector<container> 
        {
            for (size_t i = 0; i < size; ++i)
            {
                for (int j = 0; j < data[i].size(); ++j)
                    data[i][j] = 0;
            }
        }

    public:
        // constructors
        Array(size_t dim1_size, size_t dim2_size)
        : m_dim1_size(dim1_size), m_dim2_size(dim2_size), m_dim3_size(1)
        {
            size_t data_size = dim1_size * dim2_size;
            m_LocalBuffer = std::make_unique<container[]>(data_size);
            Init(m_LocalBuffer.get(), data_size);
        }

        Array(size_t dim1_size, size_t dim2_size, size_t dim3_size)
        : m_dim1_size(dim1_size), m_dim2_size(dim2_size), m_dim3_size(dim3_size)
        {
            size_t data_size = dim1_size * dim2_size * dim3_size;
            m_LocalBuffer = std::make_unique<container[]>(data_size);
            Init(m_LocalBuffer.get(), data_size);
        }

        // array access operators --> currently not implemented correctly
        container& operator()(size_t dim1_index, size_t dim2_index)
        {
            // out of bounds checks
            if (m_dim1_size < dim1_index + 1) { __debugbreak(); }
            if (m_dim2_size < dim2_index + 1) { __debugbreak(); }

            return m_LocalBuffer[(dim1_index * dim2_index) + dim2_index];
        }

        container& operator()(size_t dim1_index, size_t dim2_index, size_t dim3_index)
        {
            // out of bounds checks
            if (m_dim1_size < dim1_index + 1) { __debugbreak(); }
            if (m_dim2_size < dim2_index + 1) { __debugbreak(); }
            if (m_dim3_size < dim3_index + 1) { __debugbreak(); }

            return m_LocalBuffer[(dim1_index * dim2_index * dim3_index) + (dim2_index * dim3_index) + dim3_index];
        }

        container& operator[](size_t linear_index)
        {
            // out of bounds check
            if (m_dim1_size * m_dim2_size * m_dim3_size < linear_index + 1) { __debugbreak(); }

            return m_LocalBuffer[linear_index];
        }

        // getters
        size_t size() const
        {
            return m_dim1_size * m_dim2_size * m_dim3_size;
        }

    };
}



