#pragma once
#include "../math/Vec3.h"
#include "../math/Array.h"
#include "../math/NumericalDifferentiator.h"
#include "../math/VectorField.h"
#include "../math/RootSolver.h"

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
        // dt indirectly prop to file size (1e-3 ~ 100 000 lines -> 1e-4 ~ 1 000 000 lines) per VectorField
        double dt = 1e-3;

        // wtf
        std::function<double(double, double, ep::vec3<double>, std::function<ep::vec3<double>(double)>)> m_ftret =
            [](double t_ret, double t, ep::vec3<double> r, std::function<ep::vec3<double>(double)> r_prime_func)
                { return t_ret - t + ep::norm(r - r_prime_func(t_ret)) / constant::c_n; };

        std::vector<ScalarField> m_phi;
        std::vector<VectorField> m_A;
        std::vector<VectorField> m_E;
        std::vector<VectorField> m_B;

        std::vector<double> timeStamps;

        void SaveVectorField(const std::string& fileName, const std::vector<VectorField>& field);
        void SaveScalarField(const std::string& fileName, const std::vector<ScalarField>& field);

    public:
        virtual void Run();
        void Save();
    };
}

