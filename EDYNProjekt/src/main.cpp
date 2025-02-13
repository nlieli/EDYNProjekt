#include <iostream>
#include <concepts>
#include <type_traits>

#include "math/Vec3.h"
//#include "math/VectorArray.h"
#include "math/Array.h"
//#include "math/VectorField.h"
//#include "math/matplotlibcpp.h"

namespace constant
{
    double pi = 3.1415926;
    double epsilon_0_si = 0.0;
    double c_si = 299792458.0;
    double c_n = 1.0;
}

int main()
{
    double t_ret = 0.0;
    double t = 0.0;
    double dt = 1e-2;
    ep::vec3<double> R = { 1.0, 0.0, 0.0 };
    ep::vec3<double> v = { 0.0, 0.0, 0.0 };
    const unsigned int k = 2;
    ep::Array<ep::vec3<double>> K(k, k);
    ep::Array<double> scalarField(k, k);

    for (; t_ret < 1.0; t_ret += dt)
    {
        double R_norm = ep::norm(R);
    }

    for (size_t i = 0; i < K.size(); ++i)
        ep::print(K[i]);
}
