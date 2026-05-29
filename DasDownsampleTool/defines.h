#ifndef DEFINES_H
#define DEFINES_H

#include <QTextStream>
#include <Eigen/Dense>
#include <hdf5.h>
#include <fftw3.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#define M_PI 3.14159265358979323846

using Matrix = Eigen::MatrixXf;
using Vector = Eigen::VectorXf;
using RowMajorMatrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

bool saveBinary(const std::string& file, const Matrix& m);
bool loadBinary(const std::string& file, Matrix& m);
bool mergeSpectrumPowerToDb(const std::vector<std::string>& spectrumPowerFiles,
                            const std::string& outDbFile,
                            bool relativeDb = true,
                            float eps = 1e-9f);

void printInfo(const QString& msg);
void printError(const QString& msg);

class Defines
{
public:
    Defines();
};

#endif // DEFINES_H
