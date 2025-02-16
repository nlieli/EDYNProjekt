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
        double m_bracket_di = 1e-3;
        std::function<returnType(inputArgumentTypes...)> m_function;

        NumericalDifferentiator<returnType, inputArgumentTypes...> m_nd;

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

        template <const size_t dependentArgumentSpecifier = 0>
        std::vector<returnType> Brackets(inputArgumentTypes... args, double lower_bound, double upper_bound)
        {
            std::vector<returnType> guesses;
            returnType& dependentVariable = get_argument<dependentArgumentSpecifier>(args...);

            dependentVariable = lower_bound;
            returnType f_prev = m_function(args...);
            returnType f_now;

            for (double i = lower_bound + m_bracket_di; i < upper_bound; i += m_bracket_di)
            {
                dependentVariable = i;
                f_now = m_function(args...);
                if ((f_now * f_prev) < 0)
                    guesses.push_back(i);

                f_prev = f_now;
            }

            return guesses;
        }

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
        std::vector<returnType> Nsolve(inputArgumentTypes... args, double lower_bound, double upper_bound)
        {
            std::vector<returnType> guesses = Brackets(args..., lower_bound, upper_bound);
            returnType& dependentArg = get_argument<dependentArgumentSpecifier>(args...);
            for (size_t i = 0; i < guesses.size(); ++i)
            {
                dependentArg = guesses[i];
                guesses[i] = NRsolve(args...);
            }

            return guesses;
        }
    };

    template <typename returnType, typename... inputArgumentTypes>
    RootSolver(std::function<returnType(inputArgumentTypes...)>) -> RootSolver<returnType, inputArgumentTypes...>;
}

