#include <iostream>
#include <concepts>
#include <type_traits>

#include "math/Vec3.h"
#include "math/Array.h"
#include "math/NumericalDifferentiator.h"
#include "math/VectorField.h"
//#include "math/matplotlibcpp.h"

#include "Utility.h"
#include "Particle.h"

#include "simulations/simulation.h"

#define DISABLE_NSTD_OVERLOADS
#include "misc/nstd.h"
#include "misc/Constants.h"


int main()
{
    //double t_ret = 0;
    //auto f = [](double t_ret, double t, const ep::vec3<double>& r, const auto& r_prime_func) -> double
    //    {
    //        return t_ret - t + ep::norm(r - r_prime_func(t_ret)) / constant::c_n;
    //    };

    simulation sim;
    sim.Run();


#if false
    ep::Array<double> arr(3, 3);
    ep::Array<ep::vec3<double>> pos(3, 3);
    
    ep::ScalarField sf(10, 10);
    sf.createTestPositions(-5, 5, -5, 5);

    for (size_t i = 0; i < sf.positions.size(); ++i)
        ep::print(sf.positions[i]);



#elseif false
    auto r_prime_func = [](double t) -> ep::vec3<double>
        {
            double x = std::cos(t);
            double y = std::sin(t);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::Particle<double> p(r_prime_func);
    ep::vec3<double> pos = p.calculateCurrentPosition(2.343);
    ep::print(pos);
    ep::vec3<double> vel = p.calculateCurrentVelocity(2.343);
    ep::print(vel);

#elseif false
    auto r_prime_func = [](double t) -> ep::vec3<double>
        {
            double x = std::cos(t);
            double y = std::sin(t);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    double t = 0.0;
    double dt = 1e-2;
    double q = -1 * constant::e_n;
    double phi = 0;
    double omega = 1;
    ep::vec3<double> r = { 0.0, 0.5, 1.0 };
    ep::vec3<double> r_prime = { std::cos(phi), std::sin(phi), 0.0 };
    ep::vec3<double> R = r - r_prime;
    ep::vec3<double> v = { 0.0, 0.0, 0.0 };

    const unsigned int k = 2;
    ep::Array<ep::vec3<double>> K(k, k);
    ep::Array<double> scalarField(k, k);

    for (; t < 1.0; t += dt)
    {
        phi = omega * t; // incorrect for now, t_ret needs to be here
        r_prime = { std::cos(phi), std::sin(phi), 0.0 };
        R = r - r_prime;
        v = { -std::sin(phi), std::cos(phi), 0.0 };
        double pr = q / (4 * constant::pi * constant::epsilon_0_si) * 1 / ep::norm(R) - R * v;

        scalarField(0, 0) = q / (4 * constant::pi * constant::epsilon_0_n * (ep::norm(R) - (R * v)));
        std::cout << scalarField(0, 0) << std::endl;
    }
#endif

}
