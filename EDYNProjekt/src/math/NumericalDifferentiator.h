#pragma once
#include <functional>
#include <type_traits>
#include "../Utility.h"

/*
    example usage: 

    auto testfunc = [](double x, double a) -> double
    {
        double result = a * x * x;
        return result;
    };

    //                          Return, Input_1, Input_2
    ep::NumericalDifferentiator<double, double, double> nd(testfunc);
    
    constexpr size_t spec = 1; // per default spec is set to 0
    // ----> when spec set to 0 differentiates with respect to x;  when set to 1 differentiates with respect to a

    double result = nd.calculate<spec>(2, 2);
    double result2 = nd.calculate(2, 2); // forms derivative of first input argument passed to function
*/

namespace ep
{
    template <typename returnType, typename... inputArgumentTypes>
    class NumericalDifferentiator
    {
    private:
        double m_h = 1e-8;
        std::function<returnType(inputArgumentTypes...)> m_function;


    public:
        NumericalDifferentiator(std::function<returnType(inputArgumentTypes...)> function) : m_function(function) {}

        void SetParameterh(double h)
        {
            m_h = h;
        }

        // make sure differentialArgumentSpecifier is constexpr and the number of arguments matches the Specifier
        template <size_t differentialArgumentSpecifier = 0>
        returnType evaluate(inputArgumentTypes... args) requires Numbers<inputArgumentTypes...>
        {
            returnType function_at_args = m_function(args...);
            
            // gets input argument that is supposed to be differentiated
            auto& var = get_argument<differentialArgumentSpecifier>(args...);
            var += m_h; 
            returnType function_at_args_plus_h = m_function(args...);
            //var -= m_h; // redundant because we dont need the variable anymore and we are taking in a copy of args

            // evaluates numeric differential
            return (function_at_args_plus_h - function_at_args) / m_h;
        }
    };
}
