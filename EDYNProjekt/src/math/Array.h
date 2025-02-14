#pragma once
#include <memory>
#include <concepts>
#include "../Utility.h"
/*
    example:

    const unsigned int k = 2;
    ep::Array<double> scalarField(k, k);
    ep::Array<ep::vec3<double>> K(k, k);
*/

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
        Array(size_t dim1_size, size_t dim2_size, size_t dim3_size = 1)
        : m_dim1_size(dim1_size), m_dim2_size(dim2_size), m_dim3_size(dim3_size)
        {
            size_t data_size = dim1_size * dim2_size * dim3_size;
            m_LocalBuffer = std::make_unique<container[]>(data_size);
            Init(m_LocalBuffer.get(), data_size);
        }

        container& operator()(size_t dim1_index, size_t dim2_index, size_t dim3_index = 0)
        {
            // out of bounds checks
            if (m_dim1_size < dim1_index + 1) { __debugbreak(); }
            if (m_dim2_size < dim2_index + 1) { __debugbreak(); }
            if (m_dim3_size < dim3_index + 1) { __debugbreak(); }

            return m_LocalBuffer[(dim1_index * m_dim1_size * m_dim3_size) + (dim2_index * m_dim3_size) + dim3_index];
        }

        container& operator[](size_t linear_index)
        {
            // out of bounds check
            if (m_dim1_size * m_dim2_size * m_dim3_size < linear_index + 1) { __debugbreak(); }

            return m_LocalBuffer[linear_index];
        }

        // returns linear size by default
        size_t size(size_t dimension = 0) const
        {
            switch (dimension)
            {
            case 0: return m_dim1_size * m_dim2_size * m_dim3_size;
            case 1: return m_dim1_size;
            case 2: return m_dim2_size;
            case 3: return m_dim3_size;
            default: __debugbreak(); // Array can only have linear + 3 dimensions
            }

        }

    };
}



