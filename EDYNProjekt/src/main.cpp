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
    ep::simulation sim;
    sim.Run();

    // root solver still needs to be tested

    // plotting
    //std::system("python src/test.py");
}

