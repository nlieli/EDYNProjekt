#pragma once
#include <iostream>

/*
    example:

    vec3<double> k = { 0, 0, 0 };
*/

namespace ep
{
    template <typename T>
    struct vec3
    {
        static constexpr size_t m_size = 3;
        T x, y, z;

        T& operator[](size_t index)
        {
            switch (index)
            {
            default: __debugbreak(); // out of bounds
            case 0: return x;
            case 1: return y;
            case 2: return z; 
            }

        }

        const T& operator[](size_t index) const
        {
            switch (index)
            {
            default: __debugbreak(); // out of bounds
            case 0: return x;
            case 1: return y;
            case 2: return z; 
            }
        }

        size_t size() const
        {
            return vec3<T>::m_size;
        }
    };

    // math functions
    
    template <typename T>
    vec3<T> add(const vec3<T>& v1, const vec3<T>& v2)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] + v2[i];
        return result;
    }

    template <typename T>
    vec3<T> sub(const vec3<T>& v1, const vec3<T>& v2)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] - v2[i];
        return result;
    }

    template <typename T, typename U>
    vec3<T> add(const vec3<T>& v1, const U& scalar)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] + scalar;

        return result;
    }

    template <typename T, typename U>
    vec3<T> sub(const vec3<T>& v1, const U& scalar)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] - scalar;

        return result;
    }

    template <typename T>
    T prod(const vec3<T>& v1, const vec3<T>& v2)
    {
        T result = 0;
        for (size_t i = 0; i < v1.size(); ++i)
            result += v1[i] * v2[i];

        return result;
    }

    template <typename T, typename U>
    vec3<T> prod(const vec3<T>& v1, const U& scalar)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] * scalar;

        return result;
    }

    template <typename T, typename U>
    vec3<T> div(const vec3<T>& v1, const U& scalar)
    {
        vec3<T> result;
        for (size_t i = 0; i < v1.size(); ++i)
            result[i] = v1[i] / scalar;

        return result;
    }

    template <typename T>
    T norm(const vec3<T> vector)
    {
        T result = 0.0;
        for (size_t i = 0; i < vector.size(); ++i)
            result = std::sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

        return result;
    }

    template <typename T>
    vec3<T> cross(const vec3<T> v1, const vec3<T> v2)
    {
        vec3<T> result;
        result.x = v1.y * v2.z - v2.y * v1.z;
        result.y = v1.z * v2.x - v2.z * v1.x;
        result.z = v1.x * v2.y - v2.x * v1.y;

        return result;
    }

    // overloads
    template <typename T>
    vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2)
    {
        return add(v1, v2);
    }

    template <typename T>
    vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2)
    {
        return sub(v1, v2);
    }

    template <typename T, typename U>
    vec3<T> operator+(const vec3<T>& v1, const U& scalar)
    {
        return add(v1, scalar);
    }

    template <typename T, typename U>
    vec3<T> operator-(const vec3<T>& v1, const U& scalar)
    {
        return sub(v1, scalar);
    }

    template <typename T>
    T operator*(const vec3<T>& v1, const vec3<T> v2)
    {
        return prod(v1, v2);
    }

    template <typename T, typename U>
    vec3<T> operator*(const vec3<T>& v1, const U& scalar)
    {
        return prod(v1, scalar);
    }

    template <typename T, typename U>
    vec3<T> operator*(const U& scalar, const vec3<T>& v1)
    {
        return prod(v1, scalar);
    }

    template <typename T, typename U>
    vec3<T> operator/(const vec3<T>& v1, const U& scalar)
    {
        return div(v1, scalar);
    }


}

