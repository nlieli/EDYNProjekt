#include <iostream>
#include <concepts>
#include <type_traits>

#include "math/Vec3.h"
#include "math/Array.h"
#include "math/NumericalDifferentiator.h"
#include "math/VectorField.h"
//#include "math/matplotlibcpp.h"

#include "Utility.h"
#include "Particle.h"

#include "simulations/simulation.h"

#define DISABLE_NSTD_OVERLOADS
#include "misc/nstd.h"
#include "misc/Constants.h"


int main()
{
    ep::simulation sim;
    sim.Run();


}
