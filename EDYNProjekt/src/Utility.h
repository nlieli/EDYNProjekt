#pragma once
#include <concepts>
#include <vector>

#include "math/Vec3.h"

namespace ep
{

    template <typename T>
    std::vector<T> linspace(T start, T end, size_t num_points) {
        std::vector<T> result;
        if (num_points == 0) 
        {
            return result; 
        }

        if (num_points == 1) 
        {
            result.push_back(start); 
            return result;
        }

        result.reserve(num_points);
        T step = (end - start) / static_cast<T>(num_points - 1);
        for (size_t i = 0; i < num_points; ++i) 
        {
            result.push_back(start + i * step);
        }
        return result;
    }

    template <typename T>
    void print(const vec3<T>& vector)
    {
        std::cout << "["
            << vector[0] << ", "
            << vector[1] << ", "
            << vector[2]
            << "]" << std::endl;
    }
    
    template <typename T>
    void print(const T& value)
    {
        std::cout << value << std::endl;
    }

    template <size_t N, typename T, typename... Ts>
        T& get_argument(T& first, Ts&... rest)
    {
        if constexpr (N == 0)
            return first;
        else
            return get_argument<N - 1>(rest...);
    }

    // concepts
    template <typename T>
    concept Number = (std::is_integral_v<T> || std::is_floating_point_v<T>);

    template <typename... Ts>
    concept Numbers = (Number<Ts> && ...);

    template <typename T>
    concept Vector = requires (T con)
    {
        con[0];
        con.size();
    };

    template <typename... Ts>
    concept Vectors = (Vector<Ts> && ...);
}
