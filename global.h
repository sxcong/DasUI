#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
//#include <Eigen/Dense>
//#include <H5Cpp.h>
//#include <fftw3.h>
#include <omp.h>
#include <cmath>
//using namespace H5;
//typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMatrixXf;
class Global
{
public:
    Global();
};

bool createFullFilePath(const QString &fullPath);

/*Eigen::MatrixXf readTimeSlice(const std::string& filename,
                              const std::string& dataset_name,
                              double start_sec,
                              double duration_sec);


int readAllH5File(const std::string& h5_file_path,
                             const std::string& dataset_name,
                             Eigen::MatrixXf& eigen_mat);

RowMatrixXf loadFromNpy(const std::string& filename);
void saveToNpy(const std::string& filename, const Eigen::MatrixXf& matrix);
*/
#endif // GLOBAL_H
