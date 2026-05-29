#ifndef DEFINES_H
#define DEFINES_H

#include <Eigen/Dense>
#include <string>

using Matrix = Eigen::MatrixXf;
using Vector = Eigen::VectorXf;
using RowMajorMatrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

bool saveBinary(const std::string& file, const Matrix& m);
bool loadBinary(const std::string& file, Matrix& m);

class WaterFallConfig
{
public:
    int x_Min = 0;
    int x_Max = 0;
    int y_Min = 0;
    int y_Max = 0;
    int color_Min = 0;
    int color_Max = 0;
};

#endif // DEFINES_H
