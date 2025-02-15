#pragma once
#include "../math/Vec3.h"
#include "../math/Array.h"
#include "../math/NumericalDifferentiator.h"
#include "../math/VectorField.h"
#include "../math/RootSolver.h"
//#include "../math/matplotlibcpp.h"

#include "../Utility.h"
#include "../Particle.h"

#include "../misc/Constants.h"

namespace ep
{
    class simulation
    {
    private:
        double sim_max_time = 1.0;
        double t = 0.0;
        double dt = 1e-4;

        // wtf
        std::function<double(double, double, ep::vec3<double>, std::function<ep::vec3<double>(double)>)> m_ftret =
            [](double t_ret, double t, ep::vec3<double> r, std::function<ep::vec3<double>(double)> r_prime_func)
                { return t_ret - t + ep::norm(r - r_prime_func(t_ret)) / constant::c_n; };

    public:
        void Run()
        {
            std::function<ep::vec3<double>(double)> r_prime_func = [](double t) -> ep::vec3<double>
                {
                    double x = std::cos(t);
                    double y = std::sin(t);
                    double z = 0;

                    return ep::vec3<double>({ x, y, z });
                };

            ep::Particle p(r_prime_func);
            ep::ScalarField sf(5, 5);
            sf.createTestPositions(-5, 5, -5, 5);

            ep::RootSolver rs(m_ftret);

            ep::vec3<double> r = sf.positions[0];
            ep::vec3<double> r_prime = p.calculateCurrentPosition(t);
            ep::vec3<double> v = p.calculateCurrentVelocity(t);
            ep::vec3<double> R = r - r_prime;

            double t_ret_init_guess = t - ep::norm(R) / constant::c_n;
            double t_ret = rs.NRsolve(t_ret_init_guess, t, r, r_prime_func);

            for (; t < sim_max_time; t += dt)
            {
                for (size_t i = 0; i < sf.positions.size(); ++i)
                {
                    r = sf.positions[i];
                    t_ret = rs.NRsolve(t_ret_init_guess, t, r, r_prime_func);
                    r_prime = p.calculateCurrentPosition(t_ret);
                    v = p.calculateCurrentVelocity(t_ret);
                    R = r - r_prime;

                    sf.values[i] = p.charge / (4 * constant::pi * constant::epsilon_0_n) * 1 / (ep::norm(R) - R * v);
                }
            }
        }
    };
}

