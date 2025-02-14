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

        RootSolver<double, double, ep::vec3<double>, std::function<ep::vec3<double>(double)>> rs;

        double f_t_ret(double t_ret, double t, ep::vec3<double> r, auto r_prime_func)
        {
            return t_ret - t + ep::norm(r - r_prime_func(t_ret)) / constant::c_n;
        }

    public:
        void Run()
        {
            auto r_prime_func = [](double t) -> ep::vec3<double>
                {
                    double x = std::cos(t);
                    double y = std::sin(t);
                    double z = 0;

                    return ep::vec3<double>({ x, y, z });
                };

            ep::Particle<double> p(r_prime_func);
            ep::ScalarField sf(10, 10);
            sf.createTestPositions(-5, 5, -5, 5);
            RootSolver rms(r_prime_func);

            ep::vec3<double> r = sf.positions[0];
            ep::vec3<double> r_prime = p.calculateCurrentPosition(t);
            ep::vec3<double> v = p.calculateCurrentVelocity(t);
            ep::vec3<double> R = r - r_prime;

            for (; t < sim_max_time; t += dt)
            {



            }
        }
    };
}

