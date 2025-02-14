#pragma once
#include <functional>
#include <memory>

#include "math/Vec3.h"
#include "math/NumericalDifferentiator.h"

namespace ep
{
    template <typename... inputArgumentTypes>
    class Particle
    {
    private:
        std::function<vec3<double>(inputArgumentTypes...)> m_position_func;
        vec3<double> m_velocity;

        NumericalDifferentiator<vec3<double>, inputArgumentTypes...> m_nb;

    public:
        Particle(std::function<vec3<double>(inputArgumentTypes...)> position_func)
            : m_position_func(position_func), m_velocity({ 0,0,0 }), m_nb(position_func)
        {
        }

        vec3<double> calculateCurrentPosition(inputArgumentTypes... args)
        {
            return m_position_func(args...);
        }

        template <size_t differentialArgumentSpecifier = 0>
        vec3<double> calculateCurrentVelocity(inputArgumentTypes... args)
        {
            m_velocity = m_nb.evaluate<differentialArgumentSpecifier>(args...);

            return m_velocity;
        }
    };
}

