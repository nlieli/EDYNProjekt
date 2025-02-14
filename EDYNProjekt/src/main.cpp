#include <iostream>
#include <concepts>
#include <type_traits>

#include "math/Vec3.h"
#include "math/Array.h"
#include "math/NumericalDifferentiator.h"
//#include "math/VectorField.h"
//#include "math/matplotlibcpp.h"

#include "Utility.h"
#include "Particle.h"

namespace constant
{
    constexpr double pi = 3.14159265359;
    constexpr double e_si = 1.60217663e-19;
    constexpr double epsilon_0_si = 8.8541878188e-12;
    constexpr double mu_0_si = 1.25663706127e-6;
    constexpr double epsilon_0_n = 1 / mu_0_si;
    constexpr double mu_0_n = 1 / epsilon_0_si;
    constexpr double c_si = 299792458.0;
    constexpr double c_n = 1.0;
}

int main()
{
    auto testfunc = [](double x, double a) -> double
        {
            double result = a * x * x;
            return result;
        };

    ep::NumericalDifferentiator<double, double, double> nd(testfunc);
    //constexpr size_t spec = 1;
    double resk = nd.evaluate(2.0, 2.0);
    //std::cout << resk;

    auto r_prime_func = [](double t) -> ep::vec3<double>
        {
            double x = std::cos(t);
            double y = std::sin(t);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::NumericalDifferentiator<ep::vec3<double>, double> nd1(r_prime_func);
    ep::vec3<double> resv = nd1.evaluate(2.343);
    ep::print(resv);
    ep::Particle<double> p(r_prime_func);
    ep::vec3<double> pos = p.calculateCurrentPosition(0);
    ep::print(pos);
    ep::vec3<double> vel = p.calculateCurrentVelocity(0);
    ep::print(vel);

#if false
    auto r_prime_func = [](double t) -> ep::vec3<double>
        {
            double x = std::cos(t);
            double y = std::sin(t);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    double t = 0.0;
    double dt = 1e-2;
    double q = -1 * constant::e_si;
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

        scalarField(0, 0) = q / (4 * constant::pi * constant::epsilon_0_si * (ep::norm(R) - (R * v)));
        std::cout << scalarField(0, 0) << std::endl;
    }
#endif

}
