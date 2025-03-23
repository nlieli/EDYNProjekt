#pragma once
#include <functional>
#include <memory>

#include "math/Vec3.h"
#include "math/NumericalDifferentiator.h"

/*
    example: 

    std::function<ep::vec3<double>, double> r_prime_func = [](double t) -> ep::vec3<double>
        {
            double x = std::cos(t);
            double y = std::sin(t);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::Particle p(r_prime_func);
    ep::vec3<double> pos = p.calculateCurrentPosition(2.343);
    ep::print(pos);
    ep::vec3<double> vel = p.calculateCurrentVelocity(2.343);
    ep::print(vel);
*/

namespace ep
{
    template <typename... inputArgumentTypes>
    class Particle
    {
    private:
        std::function<vec3<double>(inputArgumentTypes...)> m_position_func;
        vec3<double> m_velocity;
        vec3<double> m_acceleration;

        NumericalDifferentiator<vec3<double>, inputArgumentTypes...> m_nb;

    public:
        double charge = -1;

        template <typename Callable>
        Particle(Callable&& position_func)
            : m_position_func(position_func), m_velocity({ 0, 0, 0 }), m_nb(position_func)
        {
        }

        vec3<double> calculateCurrentPosition(inputArgumentTypes... args)
        {
            return m_position_func(args...);
        }

        template <const size_t differentialArgumentSpecifier = 0>
        vec3<double> calculateCurrentVelocity(inputArgumentTypes... args)
        {
            m_velocity = m_nb.evaluate<differentialArgumentSpecifier>(args...);

            return m_velocity;
        }

        template <const size_t differentialArgumentSpecifier = 0>
        vec3<double> calculateCurrentAcceleration(inputArgumentTypes... args)
        {
            m_acceleration = m_nb.evaluateSecondDer<differentialArgumentSpecifier>(args...);

            return m_acceleration;
        }
    };

    template <typename returnType, typename... inputArgumentTypes>
    Particle(std::function<returnType(inputArgumentTypes...)>) -> Particle<inputArgumentTypes...>;
}

