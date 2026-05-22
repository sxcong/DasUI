#include "hdf5_das_reader.h"
#include "../core/hdf5_path_utils.h"
#include <limits>
#include <QDebug>

using namespace H5;

Hdf5DasReader::Hdf5DasReader() = default;

Hdf5DasReader::~Hdf5DasReader() {
    close();
}

bool Hdf5DasReader::open(const std::string& filePath) {
    close();
    try {
        m_file = new H5File(filePath, H5F_ACC_RDONLY);
        return true;
    } catch (...) {
        close();
        return false;
    }
}

void Hdf5DasReader::close() {
    if (m_file) {
        try { m_file->close(); } catch (...) {}
        delete m_file;
        m_file = nullptr;
    }
}

bool Hdf5DasReader::isOpen() const {
    return m_file != nullptr;
}

std::vector<std::string> Hdf5DasReader::listDatasetsRecursive(const Group& group, const std::string& prefix) const {
    std::vector<std::string> result;
    const hsize_t count = group.getNumObjs();
    for (hsize_t i = 0; i < count; ++i) {
        std::string name = group.getObjnameByIdx(i);
        H5G_obj_t type = group.getObjTypeByIdx(i);
        std::string full = prefix == "/" ? "/" + name : prefix + "/" + name;
        if (type == H5G_DATASET) {
            result.push_back(full);
        } else if (type == H5G_GROUP) {
            try {
                Group child = group.openGroup(name);
                auto nested = listDatasetsRecursive(child, full);
                result.insert(result.end(), nested.begin(), nested.end());
            } catch (...) {
            }
        }
    }
    return result;
}

std::vector<std::string> Hdf5DasReader::listDatasets(const std::string& groupPath) const {
    std::vector<std::string> result;
    if (!m_file) return result;
    try {
        Group group = m_file->openGroup(Hdf5PathUtils::normalizePath(groupPath));
        return listDatasetsRecursive(group, Hdf5PathUtils::normalizePath(groupPath));
    } catch (...) {
        return result;
    }
}

bool Hdf5DasReader::readDataset(const std::string& datasetPath, DasMatrix& matrix) {
    if (!m_file) return false;
    try {
        DataSet dataset = m_file->openDataSet(Hdf5PathUtils::normalizePath(datasetPath));
        DataSpace space = dataset.getSpace();
        const int rank = space.getSimpleExtentNdims();
        if (rank < 1 || rank > 2) return false;

        std::vector<hsize_t> dims(static_cast<std::size_t>(rank), 0);
        space.getSimpleExtentDims(dims.data(), nullptr);
        matrix.rows = static_cast<std::size_t>(dims[0]);
        matrix.cols = (rank == 2) ? static_cast<std::size_t>(dims[1]) : 1;
        qDebug()<<"readDataset"<<matrix.rows<<matrix.cols;
        matrix.data.resize(matrix.rows * matrix.cols);
        dataset.read(matrix.data.data(), PredType::NATIVE_DOUBLE);
        return true;
    } catch (...) {
        return false;
    }
}

bool Hdf5DasReader::readScalarAttribute(const Attribute& attr, std::string& value) const {
    try {
        const H5T_class_t typeClass = attr.getTypeClass();
        if (typeClass == H5T_STRING) {
            attr.read(attr.getStrType(), value);
            return true;
        }
        if (typeClass == H5T_FLOAT) {
            double v = 0.0;
            attr.read(PredType::NATIVE_DOUBLE, &v);
            value = std::to_string(v);
            return true;
        }
        if (typeClass == H5T_INTEGER) {
            long long v = 0;
            attr.read(PredType::NATIVE_LLONG, &v);
            value = std::to_string(v);
            return true;
        }
    } catch (...) {
    }
    return false;
}

std::map<std::string, std::string> Hdf5DasReader::readAttributes(const std::string& datasetPath) {
    std::map<std::string, std::string> attrs;
    if (!m_file) return attrs;
    try {
        DataSet dataset = m_file->openDataSet(Hdf5PathUtils::normalizePath(datasetPath));
        const hsize_t count = dataset.getNumAttrs();
        for (hsize_t i = 0; i < count; ++i) {
            Attribute attr = dataset.openAttribute(i);
            std::string value;
            if (readScalarAttribute(attr, value)) {
                attrs[attr.getName()] = value;
            }
        }
    } catch (...) {
    }
    return attrs;
}
