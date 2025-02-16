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

    i.e.: std::function<ep::vec3<double>(double, double)> trajectory [](double a, double t){}
    this is wrong, or rather, the function will be differentiated with respect to a instead of t. 
    For it to work properly, keep double t first:
    i.e.: std::function<ep::vec3<double>(double, double)> trajectory [](double t, double a){}
                                            |Here|                          |Here|

    Also notice the change in the signature marked with |Here|. The types must match.

                                     !!!IMPORTANT!!!
    For the simulation to give results that make sense, the condition v(t) < c must be met for all t!
    Since the trajectory is user defined, you must always ensure that the following holds:

                          norm( d(trajectory)/dt ) < c = 1

    notice that c, as all other variables are in natural units.
    */

    //                             |this|                   |this|
    std::function<ep::vec3<double>(double)> trajectory = [](double t) 
        {
            double x = 1 * std::cos(t / 1.5);
            double y = 1 * std::sin(t / 1.5);
            double z = 0;

            return ep::vec3<double>({ x, y, z });
        };

    ep::simulation sim(trajectory);
    sim.sfGridPoints = 40;
    sim.vfGridPoints = 10;
    sim.Run();

    // plotting
    //std::system("python src/data/plot_A.py");
    std::system("python src/data/plot_phi.py");

}

