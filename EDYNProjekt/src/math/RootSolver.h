#pragma once
#include <functional>

#include "../Utility.h"
#include "NumericalDifferentiator.h"

namespace ep
{
    template <typename returnType, typename... inputArgumentTypes>
    class RootSolver
    {
    private:
        unsigned int m_iterations = 10;
        std::function<returnType(inputArgumentTypes...)> m_function;

        NumericalDifferentiator<returnType, inputArgumentTypes...> m_nd;

    public:
        template <typename Callable>
        RootSolver(Callable&& function)
            : m_function(function), m_nd(function) 
        {

        }

        void SetIterations(unsigned int iterations)
        {
            m_iterations = iterations;
        }

        template <const size_t dependentArgumentSpecifier = 0>
        returnType NRsolve(inputArgumentTypes... args)
        {
            returnType& result = get_argument<dependentArgumentSpecifier>(args...);
            returnType m_function_prime;
            for (unsigned int i = 0; i < m_iterations; ++i)
            {
                m_function_prime = m_nd.evaluate<dependentArgumentSpecifier>(args...);
                result -= m_function(args...) / m_function_prime;
            }

            return result;
        }
    };

    template <typename returnType, typename... inputArgumentTypes>
    RootSolver(std::function<returnType(inputArgumentTypes...)>) -> RootSolver<returnType, inputArgumentTypes...>;
}

