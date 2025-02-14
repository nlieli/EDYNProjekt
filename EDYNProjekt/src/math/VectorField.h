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
    };

    struct ScalarField
    {
        Array<double> values;
        Array<vec3<double>> positions;

        void createTestPositions(double x_LBound, double x_UBound, double y_LBound, double y_UBound)
        {
            size_t x_number_of_points = positions.size(1);
            size_t y_number_of_points = positions.size(2);

            std::vector<double> x_points = ep::linspace(x_LBound, x_UBound, x_number_of_points);
            std::vector<double> y_points = ep::linspace(y_LBound, y_UBound, y_number_of_points);
            for (size_t i = 0; i < x_number_of_points; ++i)
            {
                for (size_t j = 0; j < y_number_of_points; j++)
                {
                    positions(i, j) = vec3<double>(x_points[i], y_points[j], 0);
                }
            }
        }
    };
}



