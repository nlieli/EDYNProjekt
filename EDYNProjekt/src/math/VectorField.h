#pragma once
#include <memory>

#include "Array.h"
#include "Vec3.h"
#include "../Utility.h"

namespace ep
{
    struct VectorField
    {
        Array<vec3<double>> values;
        Array<vec3<double>> positions;

        VectorField(size_t x_size, size_t y_size, size_t z_size = 1);

        void createTestPositions(double x_LBound, double x_UBound, double y_LBound, double y_UBound, double z_LBound = 0, double z_UBound = 0);
        void createTestPosE(vec3<double> hotspot, double x_LBound, double x_UBound, double y_LBound, double y_UBound, double z_LBound = 0, double z_UBound = 0);
    };

    struct ScalarField
    {
        Array<double> values;
        Array<vec3<double>> positions;

        ScalarField(size_t x_size, size_t y_size, size_t z_size = 1);

        void createTestPositions(double x_LBound, double x_UBound, double y_LBound, double y_UBound, double z_LBound = 0, double z_UBound = 0);
    };
}



