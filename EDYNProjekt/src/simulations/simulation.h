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
        virtual void Run();
    };
}

