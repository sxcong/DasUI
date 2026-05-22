#include "hd5reader.h"
#include <QDebug>
#include <H5Cpp.h>

#ifndef H5_USE_16_API
#pragma message("H5_USE_16_API is not defined -- ensure HDF5 is compiled with C++17 compatible settings")
#endif

using namespace H5;

Hd5Reader::Hd5Reader()
    : m_file(nullptr)
    , m_isOpen(false)
{
}

Hd5Reader::Hd5Reader(const std::string& filepath)
    : m_file(nullptr)
    , m_isOpen(false)
{
    open(filepath);
}

Hd5Reader::~Hd5Reader()
{
    close();
}

bool Hd5Reader::open(const std::string& filepath)
{
    close();
    try {
        FileAccPropList fapl;
        fapl.setCache(0, 10007, 512 * 1024 * 1024, 0.0);
        m_file = new H5File(filepath, H5F_ACC_RDONLY, fapl);
        m_isOpen = true;
        return true;
    } catch (FileIException& e) {
        qDebug() << "Hd5Reader::open failed:" << e.getDetailMsg().c_str();
    }
    return false;
}

void Hd5Reader::close()
{
    if (m_file) {
        try { m_file->close(); } catch (...) {}
        delete m_file;
        m_file = nullptr;
    }
    m_isOpen = false;
}

bool Hd5Reader::isOpen() const
{
    return m_isOpen;
}

std::vector<std::string> Hd5Reader::listDatasets(const std::string& groupPath) const
{
    std::vector<std::string> datasets;
    if (!m_isOpen || !m_file) return datasets;
    try {
        Group group = m_file->openGroup(groupPath);
        hsize_t numObj = group.getNumObjs();
        for (hsize_t i = 0; i < numObj; ++i) {
            std::string name = group.getObjnameByIdx(i);
            H5G_obj_t type = group.getObjTypeByIdx(i);
            if (type == H5G_DATASET) {
                datasets.push_back(name);
            }
        }
    } catch (...) {}
    return datasets;
}

bool Hd5Reader::hasDataset(const std::string& datasetName) const
{
    if (!m_isOpen || !m_file) return false;
    try {
        m_file->openDataSet(datasetName);
        return true;
    } catch (...) {
        return false;
    }
}

bool Hd5Reader::getDims(const std::string& datasetName, hsize_t dims[2]) const
{
    if (!m_isOpen || !m_file) return false;
    try {
        DataSet dataset = m_file->openDataSet(datasetName);
        DataSpace dataspace = dataset.getSpace();
        dataspace.getSimpleExtentDims(dims, nullptr);
        return true;
    } catch (DataSetIException& e) {
        qDebug() << "Hd5Reader::getDims failed:" << e.getDetailMsg().c_str();
    }
    return false;
}

bool Hd5Reader::readDataset(const std::string& datasetName, Eigen::MatrixXf& matrix)
{
    if (!m_isOpen || !m_file) return false;
    try {
        DataSet dataset = m_file->openDataSet(datasetName);
        DataSpace dataspace = dataset.getSpace();

        hsize_t dims[2];
        dataspace.getSimpleExtentDims(dims, nullptr);
        int rows = static_cast<int>(dims[0]);// 120000
        int cols = static_cast<int>(dims[1]);// 959

        // 创建 Eigen 矩阵 (注意：Eigen 默认是列优先存储)
        // 如果 HDF5 数据是行优先(Row-major)，建议使用 Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>

        typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMajorMatrixXf;
        RowMajorMatrixXf mat(rows, cols);
        dataset.read(mat.data(), PredType::NATIVE_FLOAT);
        matrix = mat;
        //现在的结构：mat(时间点, 井深)


        // 当你需要处理某个井深时，使用 View
        //auto single_well_data = mat.col(well_index);
        //需要对这些时间序列进行频繁的频率域分析（如 FFT），那么“数据在内存中的连续性”就变得至关重要。
        //直接使用 Eigen 的默认类型 Eigen::MatrixXd。如前所述，HDF5 默认的行优先数据读入到 Eigen::MatrixXd 中时，恰好会发生“隐式转置”。
        //        结果：matrix.col(j) 在内存中正好是连续的 120,000 个采样点。
         //       优势：可以直接将 matrix.col(j).data() 传递给 FFT 库（如 FFTW 或 MKL），无需任何拷贝或重排。
        /*
        // 强制声明为 MatrixXd (列优先)
            // 此时 mat.rows() = 120000, mat.cols() = 900
            Eigen::MatrixXd mat(static_cast<int>(dims[0]), static_cast<int>(dims[1]));

            // 从 HDF5 (行优先) 读取到 Eigen (列优先)
            // 这一步读取后，内存中第 j 列即为第 j 个井深的所有时间点
            dataset.read(mat.data(), H5::PredType::NATIVE_DOUBLE);

                在执行 FFT 时，不要直接操作整个矩阵，而是对每一列进行迭代。
                Eigen::MatrixXd data = loadDataForFFT("data.h5", "/measurements");

                for (int j = 0; j < data.cols(); ++j) {
                    // 获取第 j 个井深的数据，这是连续的内存块
                    double* ptr = data.col(j).data();

                    // 示例：这里调用你的 FFT 函数
                    // MyFFTFunction(ptr, data.rows());
                }
            */


        return true;
    } catch (DataSetIException& e) {
        qDebug() << "Hd5Reader::readDataset failed:" << e.getDetailMsg().c_str();
    } catch (DataSpaceIException& e) {
        qDebug() << "Hd5Reader::readDataset failed:" << e.getDetailMsg().c_str();
    }
    return false;
}

std::map<std::string, std::string> Hd5Reader::readAttributes(const std::string& datasetName)
{
    std::map<std::string, std::string> attrs;
    if (!m_isOpen || !m_file) return attrs;
    try {
        DataSet dataset = m_file->openDataSet(datasetName);
        H5A_info_t attrInfo;
        hsize_t numAttrs = dataset.getNumAttrs();
        for (hsize_t i = 0; i < numAttrs; ++i) {
            Attribute attr = dataset.openAttribute(i);
            std::string name = attr.getName();

            H5T_class_t typeClass = attr.getTypeClass();
            switch (typeClass) {
            case H5T_STRING: {
                StrType strType(0, H5T_VARIABLE);
                char* strBuf = nullptr;
                attr.read(strType, &strBuf);
                attrs[name] = strBuf ? std::string(strBuf) : "";
                if (strBuf) free(strBuf);
                break;
            }
            case H5T_FLOAT: {
                double val = 0;
                attr.read(PredType::NATIVE_DOUBLE, &val);
                attrs[name] = std::to_string(val);
                break;
            }
            case H5T_INTEGER: {
                long long val = 0;
                attr.read(PredType::NATIVE_LLONG, &val);
                attrs[name] = std::to_string(val);
                break;
            }
            default:
                attrs[name] = "(unsupported type)";
                break;
            }
        }
    } catch (DataSetIException& e) {
        qDebug() << "Hd5Reader::readAttributes failed:" << e.getDetailMsg().c_str();
    } catch (AttributeIException& e) {
        qDebug() << "Hd5Reader::readAttributes failed:" << e.getDetailMsg().c_str();
    }
    return attrs;
}

std::string Hd5Reader::readAttribute(const std::string& datasetName, const std::string& attrName)
{
    if (!m_isOpen || !m_file) return {};
    try {
        DataSet dataset = m_file->openDataSet(datasetName);
        Attribute attr = dataset.openAttribute(attrName);

        H5T_class_t typeClass = attr.getTypeClass();
        switch (typeClass) {
        case H5T_STRING: {
            StrType strType(0, H5T_VARIABLE);
            char* strBuf = nullptr;
            attr.read(strType, &strBuf);
            std::string result = strBuf ? std::string(strBuf) : "";
            if (strBuf) free(strBuf);
            return result;
        }
        case H5T_FLOAT: {
            double val = 0;
            attr.read(PredType::NATIVE_DOUBLE, &val);
            return std::to_string(val);
        }
        case H5T_INTEGER: {
            long long val = 0;
            attr.read(PredType::NATIVE_LLONG, &val);
            return std::to_string(val);
        }
        default:
            return {};
        }
    } catch (...) {
        return {};
    }
}
