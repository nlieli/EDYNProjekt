#include "VectorField.h"

namespace ep
{
    VectorField::VectorField(size_t x_size, size_t y_size, size_t z_size /*= 1*/)
        : values(x_size, y_size, z_size), positions(x_size, y_size, z_size)
    {
    }

    void VectorField::createTestPositions(double x_LBound, double x_UBound, double y_LBound, double y_UBound, double z_LBound /*= 0*/, double z_UBound /*= 0*/)
    {
        size_t x_number_of_points = positions.size(1);
        size_t y_number_of_points = positions.size(2);
        size_t z_number_of_points = positions.size(3);

        std::vector<double> x_points = ep::linspace(x_LBound, x_UBound, x_number_of_points);
        std::vector<double> y_points = ep::linspace(y_LBound, y_UBound, y_number_of_points);
        std::vector<double> z_points = ep::linspace(z_LBound, z_UBound, z_number_of_points);

        for (size_t k = 0; k < z_number_of_points; ++k)
        {
            for (size_t i = 0; i < x_number_of_points; ++i)
            {
                for (size_t j = 0; j < y_number_of_points; ++j)
                {
                    positions(i, j, k) = vec3<double>(x_points[i], y_points[j], z_points[k]);
                }
            }
        }
    }

    ScalarField::ScalarField(size_t x_size, size_t y_size, size_t z_size /*= 1*/)
        : values(x_size, y_size, z_size), positions(x_size, y_size, z_size)
    {
    }

    void ScalarField::createTestPositions(double x_LBound, double x_UBound, double y_LBound, double y_UBound, double z_LBound /*= 0*/, double z_UBound /*= 0*/)
    {
        size_t x_number_of_points = positions.size(1);
        size_t y_number_of_points = positions.size(2);
        size_t z_number_of_points = positions.size(3);

        std::vector<double> x_points = ep::linspace(x_LBound, x_UBound, x_number_of_points);
        std::vector<double> y_points = ep::linspace(y_LBound, y_UBound, y_number_of_points);
        std::vector<double> z_points = ep::linspace(z_LBound, z_UBound, z_number_of_points);

        for (size_t k = 0; k < z_number_of_points; ++k)
        {
            for (size_t i = 0; i < x_number_of_points; ++i)
            {
                for (size_t j = 0; j < y_number_of_points; ++j)
                {
                    positions(i, j, k) = vec3<double>(x_points[i], y_points[j], z_points[k]);
                }
            }
        }
    }
}
