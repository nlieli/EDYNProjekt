#pragma once
#include "../math/Vec3.h"
#include "../math/Array.h"
#include "../math/NumericalDifferentiator.h"
#include "../math/VectorField.h"
#include "../math/RootSolver.h"

#include "../Utility.h"
#include "../Particle.h"

#include "../misc/Constants.h"

#include <fstream>
#include "omp.h"

namespace ep
{
    template <typename returnType, typename... inputArgumentTypes>
    class simulation
    {
    private:
        std::function<returnType(inputArgumentTypes...)> m_function;

        double sim_max_time = 2 * constant::pi * 2;
        double t = 0.0;
        // dt indirectly prop to file size (1e-3 ~ 100 000 lines -> 1e-4 ~ 1 000 000 lines) per VectorField
        double dt = 0.1;

        // wtf
        std::function<double(double, double, ep::vec3<double>, std::function<ep::vec3<double>(double)>)> m_ftret =
            [](double t_ret, double t, ep::vec3<double> r, std::function<ep::vec3<double>(double)> r_prime_func)
            { return t_ret - t + ep::norm(r - r_prime_func(t_ret)) / constant::c_n; };

        std::vector<ScalarField> m_phi;
        std::vector<VectorField> m_A;
        std::vector<VectorField> m_E;
        std::vector<VectorField> m_B;

        std::vector<ep::vec3<double>> m_trajectory;

        std::vector<double> timeStamps;

        void SaveTrajectory(const std::string& fileName, const std::vector<vec3<double>> trajectory)
        {
            std::ofstream ofs("src/data/" + fileName);
            if (!ofs.is_open())
                __debugbreak(); 

            std::ostringstream dataStream;
            dataStream << "time,x,y,z\n";
            for (size_t j = 0; j < trajectory.size(); ++j)
            {
                dataStream << timeStamps[j] << ","
                    << trajectory[j].x << ","
                    << trajectory[j].y << ","
                    << trajectory[j].z << "\n";
            }

            ofs << dataStream.str();
            ofs.close();
        }

        void SaveVectorField(const std::string& fileName, const std::vector<VectorField>& field)
        {
            std::ofstream ofs("src/data/" + fileName);
            if (!ofs.is_open())
                __debugbreak(); 

            std::ostringstream dataStream;

            dataStream << "time,x,y,z,xvalue,yvalue,zvalue\n";
            for (size_t j = 0; j < field.size(); ++j)
            {
                for (size_t i = 0; i < field[j].positions.size(); ++i)
                {
                    dataStream << timeStamps[j] << ","
                        << field[j].positions[i].x << ","
                        << field[j].positions[i].y << ","
                        << field[j].positions[i].z << ","
                        << field[j].values[i].x << ","
                        << field[j].values[i].y << ","
                        << field[j].values[i].z << "\n";
                }
            }

            ofs << dataStream.str();
            ofs.close();
        }

        void SaveScalarField(const std::string& fileName, const std::vector<ScalarField>& field)
        {
            std::ofstream ofs("src/data/" + fileName);
            if (!ofs.is_open())
                __debugbreak();

            std::ostringstream dataStream;

            dataStream << "time,x,y,z,value\n";
            for (size_t j = 0; j < field.size(); ++j)
            {
                for (size_t i = 0; i < field[j].positions.size(); ++i)
                {
                    dataStream << timeStamps[j] << ","
                        << field[j].positions[i].x << ","
                        << field[j].positions[i].y << ","
                        << field[j].positions[i].z << ","
                        << field[j].values[i] << "\n";
                }
            }

            ofs << dataStream.str();
            ofs.close();
        }

    public:
        size_t sfGridPoints = 10;
        double sfLlim = -5;
        double sfUlim = 5;

        size_t vfGridPoints = 10;
        double vfLlim = -5;
        double vfUlim = 5;

        template <typename Callable>
        simulation(Callable& function) : m_function(function) {}

        virtual void Run()
        {
            Timer timer;
            ep::Particle p(m_function);

            // rounding is intended here
            int iterations = static_cast<int>(sim_max_time - t) / dt;

            ep::ScalarField phi(sfGridPoints, sfGridPoints);
            phi.createTestPositions(sfLlim, sfUlim, sfLlim, sfUlim);
            ep::VectorField A(vfGridPoints, vfGridPoints);
            A.createTestPositions(vfLlim, vfUlim, vfLlim, vfUlim);
            ep::VectorField E(vfGridPoints, vfGridPoints);
            E.createTestPositions(vfLlim, vfUlim, vfLlim, vfUlim);
            ep::VectorField B(vfGridPoints, vfGridPoints);
            B.createTestPositions(vfLlim, vfUlim, vfLlim, vfUlim);

            // reserve space for each frame
            timeStamps.reserve(iterations);
            m_phi.reserve(iterations);
            m_A.reserve(iterations);
            m_E.reserve(iterations);
            m_B.reserve(iterations);

            // initialization
            ep::RootSolver rs(m_ftret);
            ep::vec3<double> r = phi.positions[0];
            ep::vec3<double> r_prime = p.calculateCurrentPosition(t);
            ep::vec3<double> v = p.calculateCurrentVelocity(t);
            ep::vec3<double> R = r - r_prime;

            // t_ret_init_guess is completely irrelevant - is overwritten in Nsolve function anyway
            double t_ret_init_guess = 0; 

            // t_ret may have more than one solution - correct solution is filtered for later
            std::vector<double> t_ret_solutions;
            double t_ret = 0;

            for (int j = 0; j < iterations; ++j)
            {
                //Timer timer;
                for (int i = 0; i < phi.positions.size(); ++i)
                {
                    r = phi.positions[i];
                    t_ret_solutions = rs.Nsolve(t_ret_init_guess, t, r, m_function, -10, t);

                    // only results for t_ret < t are meaningful 
                    for (size_t i = 0; i < t_ret_solutions.size(); ++i)
                    {
                        if (t_ret_solutions[i] < t)
                            t_ret = t_ret_solutions[i];
                    }

                    r_prime = p.calculateCurrentPosition(t_ret);
                    v = p.calculateCurrentVelocity(t_ret);
                    R = r - r_prime;

                    phi.values[i] = p.charge / (4 * constant::pi * constant::mu_0_n) * 1 / (ep::norm(R) - R * v);
                }

                // maybe combine everything?!
                for (int i = 0; i < A.positions.size(); ++i)
                {
                    r = A.positions[i];
                    t_ret_solutions = rs.Nsolve(t_ret_init_guess, t, r, m_function, -10, t);

                        // only results for t_ret < t are meaningful 
                        for (size_t i = 0; i < t_ret_solutions.size(); ++i)
                        {
                            if (t_ret_solutions[i] < t)
                                t_ret = t_ret_solutions[i];
                        }

                    r_prime = p.calculateCurrentPosition(t_ret);
                    v = p.calculateCurrentVelocity(t_ret);
                    R = r - r_prime;

                    A.values[i] = constant::mu_0_n * p.charge * v / (4 * constant::pi) * 1 / (ep::norm(R) - R * v);
                }

                timeStamps.push_back(t);
                m_phi.push_back(phi);
                m_A.push_back(A);
                m_trajectory.push_back(p.calculateCurrentPosition(t));

                t += dt;
            }

            Save();
        }

        void Save()
        {
            SaveTrajectory("particle_traj.csv", m_trajectory);
            SaveScalarField("phi_field.csv", m_phi);
            SaveVectorField("A_field.csv", m_A);
            //SaveVectorField("E_field.txt", m_B);
            //SaveVectorField("B_field.txt", m_B);
        }

    };

    template <typename returnType, typename... inputArgumentTypes>
    simulation(std::function<returnType(inputArgumentTypes...)>) -> simulation<returnType, inputArgumentTypes...>;
}

