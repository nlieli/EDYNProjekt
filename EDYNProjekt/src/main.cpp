#include <iostream>
#include <concepts>
#include <type_traits>
#include <span>
#include <cstdlib>

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
    You may freely add input parameters but you need to change the type of std::function accordingly.
    The types marked as |this| must match. You can add as many as you want but the time dependant 
    variable MUST always be first. 

                                     !!!IMPORTANT!!!
    For the simulation to give results that make sense, the condition v(t) < c must be met for all t!
    Since the trajectory is user defined, you must always ensure that the following holds:

                          norm( d(trajectory)/dt ) < c = 1

    notice that c, as all other variables are in natural units.
    */

    //                             |this|                   |this|
    std::function<ep::vec3<double>(double)> trajectory = [](double t) 
        {
            //double x = 0.9 * std::cos(t / 1.5);
            //double y = 0.9 * std::sin(t / 1.5);
            //double y = x * x;
            //double x = 0;
            //double y = 0;
            double x;
            if (t <= 0)
                x = -4;
            else if (t > 5)
                x = (5 * 5 * 0.1 - 4) + (t - 5) * exp(-(t - 5) * 2);
            else if (t >= 10)
                x = (5 * 5 * 0.1 - 4) + (10 - 5) * (1 - exp(-(10 - 5) * 2));
            else
                x = t * t * 0.1 - 4;
            double y = 0;
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::simulation sim(trajectory);
    sim.sfGridPoints = 5;
    sim.vfGridPoints = 30;
    sim.Run();

    // plotting
    std::system("python src/data/plot_A.py");
    //std::system("python src/data/plot_phi.py");

}

