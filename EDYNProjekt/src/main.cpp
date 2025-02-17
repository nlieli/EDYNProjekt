#include <iostream>
#include <concepts>
#include <type_traits>
#include <span>
#include <cstdlib>
#include <cmath>

#include "math/Vec3.h"
#include "math/Array.h"
#include "math/NumericalDifferentiator.h"
#include "math/VectorField.h"

#include "Utility.h"
#include "Particle.h"

#include "simulations/simulation.h"

#define DISABLE_NSTD_OVERLOADS
#include "misc/nstd.h"
#include "misc/Constants.h"

#include <Python.h>

int main(int argc, char** argv)
{
    /*
    Define the trajectory of the particle you want to simulate in the std::function trajectory.
    You may freely add input parameters, but they need to be added as references in the capture
    list of the function. For example:
    double x = 0;
    std::function<ep::vec3<double>(double)> trajectory = [&x](double t) {...}

                                     !!!IMPORTANT!!!
    For the simulation to give results that make sense, the condition v(t) < c must be met for all t!
    Since the trajectory is user defined, you must always ensure that the following holds:

                          norm( d(trajectory)/dt ) < c = 1

    notice that c, as all other variables are in natural units.
    */

    std::function<ep::vec3<double>(double)> trajectory1 = [](double t) 
        {
            double x = 0.9 * std::cos(t / 1.5);
            double y = 0.9 * std::sin(t / 1.5);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    std::function<ep::vec3<double>(double)> trajectory2 = [](double t) 
        {
            double x = 0;
            double y = 0;
            double z = cos(t / 2);

            return ep::vec3<double>({ x, y, z });
        };

    std::function<ep::vec3<double>(double)> trajectory3 = [](double t) 
        {
            double x = 0.5 * t - 4;
            double y = 0.5 * t - 4;
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::simulation sim(trajectory2);
    //sim.SetDeltaTime(3e-17);
    //sim.SetSimTime(1e-15);
    
    // set grid layout
    sim.sfGridPoints = 10;

    // plotting is not supported: leave as default
    sim.sfGridPointsZ = 1;
    sim.sfLlimZ = 0;
    sim.sfUlimZ = 0;

    // full 3D support
    sim.vfGridPoints = 10;
    sim.vfLlim = -5;
    sim.vfUlim = 5;
    sim.vfGridPointsZ = 10; // set to 1 for default and change lim to 0 or comment out
    sim.vfLlimZ = -5; // -5
    sim.vfUlimZ = 5; // 5

    // run simulation
    sim.Run();

    // plotting
    //std::system("python src/data/plot_phi.py"); // only plot phi
    //std::system("python src/data/plot_A.py"); // plots only A field
    std::system("python src/data/plot_EB.py"); // plot E and B field
    //std::system("python src/data/plot_AEB.py"); // plot A, E and B field 

}

