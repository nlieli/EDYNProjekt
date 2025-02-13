#pragma once
#include <memory>

#include "Array.h"
#include "Vec3.h"

namespace ep
{
    struct VectorField
    {
        Array<vec3<double>> values;
        Array<vec3<double>> positions;
    };

    struct ScalarField
    {
        Array<double> values;
        Array<vec3<double>> positions;
    };
}



