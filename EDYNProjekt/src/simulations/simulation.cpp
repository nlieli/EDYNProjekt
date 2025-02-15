#include "simulation.h"
#include "omp.h"

#include <fstream>

namespace ep
{
    void simulation::Run()
    {
        std::function<ep::vec3<double>(double)> r_prime_func = [](double t) -> ep::vec3<double>
            {
                double x = std::cos(t);
                double y = std::sin(t);
                double z = 0;

                return ep::vec3<double>({ x, y, z });
            };

        ep::Particle p(r_prime_func);

        size_t sfGridPoints = 10;
        double sfLlim = -5;
        double sfUlim = 5;

        size_t vfGridPoints = 10;
        double vfLlim = -5;
        double vfUlim = 5;

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

        // guess needs to be updated as well
        double t_ret_init_guess = t - ep::norm(R) / constant::c_n;
        double t_ret = rs.NRsolve(t_ret_init_guess, t, r, r_prime_func);


        #pragma omp parallel for 
        for (int j = 0; j < iterations; ++j)
        {
            #pragma omp parallel for
            for (int i = 0; i < phi.positions.size(); ++i)
            {
                r = phi.positions[i];
                t_ret = rs.NRsolve(t_ret_init_guess, t, r, r_prime_func);
                r_prime = p.calculateCurrentPosition(t_ret);
                v = p.calculateCurrentVelocity(t_ret);
                R = r - r_prime;

                phi.values[i] = p.charge / (4 * constant::pi * constant::epsilon_0_n) * 1 / (ep::norm(R) - R * v);
            }

            // maybe combine everything?!
            #pragma omp parallel for
            for (int i = 0; i < A.positions.size(); ++i)
            {
                r = A.positions[i];
                t_ret = rs.NRsolve(t_ret_init_guess, t, r, r_prime_func);
                r_prime = p.calculateCurrentPosition(t_ret);
                v = p.calculateCurrentVelocity(t_ret);
                R = r - r_prime;

                A.values[i] = constant::mu_0_n * p.charge * v / (4 * constant::pi) * 1 / (ep::norm(R) - R * v);
            }

            // might cause race condition if not omp crit
            #pragma omp critical
            {
                timeStamps.push_back(t);
                m_phi.push_back(phi);
                m_A.push_back(A);
            }

            t += dt;
        }

        Save();
    }

    void simulation::Save()
    {
        Timer timer;
        SaveScalarField("phi_field.txt", m_phi);
        SaveVectorField("A_field.txt", m_A);
    }

    void simulation::SaveVectorField(const std::string& fileName, const std::vector<VectorField>& field)
    {
        std::ofstream ofs("src/data/" + fileName);
        if (!ofs.is_open())
            std::cout << "Error"; 

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

    void simulation::SaveScalarField(const std::string& fileName, const std::vector<ScalarField>& field)
    {
        std::ofstream ofs("src/data/" + fileName);
        if (!ofs.is_open())
            std::cout << "Error"; 

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
                    << field[j].values[i] << "\n";
            }
        }

        ofs << dataStream.str();
        ofs.close();
    }
}
