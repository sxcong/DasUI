#include "hdf5_das_writer.h"
#include "../core/hdf5_path_utils.h"

using namespace H5;

Hdf5DasWriter::Hdf5DasWriter() = default;

Hdf5DasWriter::~Hdf5DasWriter() {
    close();
}

bool Hdf5DasWriter::open(const std::string& filePath) {
    close();
    try {
        m_file = new H5File(filePath, H5F_ACC_TRUNC);
        return true;
    } catch (...) {
        close();
        return false;
    }
}

void Hdf5DasWriter::close() {
    if (m_file) {
        try { m_file->close(); } catch (...) {}
        delete m_file;
        m_file = nullptr;
    }
}

bool Hdf5DasWriter::isOpen() const {
    return m_file != nullptr;
}

bool Hdf5DasWriter::createOrOpenDataset(const std::string& datasetPath, const DasMatrix& matrix, DataSet& dataset) {
    if (!m_file || matrix.empty()) return false;
    const std::string path = Hdf5PathUtils::normalizePath(datasetPath);
    const std::string parent = Hdf5PathUtils::parentPath(path);
    if (!Hdf5PathUtils::ensureGroups(*m_file, parent)) return false;

    hsize_t dims[2] = {
        static_cast<hsize_t>(matrix.rows),
        static_cast<hsize_t>(matrix.cols)
    };
    DataSpace space(2, dims);
    try {
        dataset = m_file->createDataSet(path, PredType::NATIVE_DOUBLE, space);
        return true;
    } catch (...) {
        return false;
    }
}

bool Hdf5DasWriter::writeDataset(const std::string& datasetPath, const DasMatrix& matrix) {
    if (!m_file) return false;
    try {
        DataSet dataset(H5I_INVALID_HID);
        if (!createOrOpenDataset(datasetPath, matrix, dataset)) return false;
        dataset.write(matrix.data.data(), PredType::NATIVE_DOUBLE);
        return true;
    } catch (...) {
        return false;
    }
}

bool Hdf5DasWriter::writeSingleAttribute(DataSet& dataset, const std::string& name, const std::string& value) {
    try {
        StrType type(PredType::C_S1, H5T_VARIABLE);
        DataSpace space(H5S_SCALAR);
        Attribute attr = dataset.createAttribute(name, type, space);
        const char* ptr = value.c_str();
        attr.write(type, &ptr);
        return true;
    } catch (...) {
        return false;
    }
}

bool Hdf5DasWriter::writeAttributes(const std::string& datasetPath, const std::map<std::string, std::string>& attrs) {
    if (!m_file) return false;
    try {
        DataSet dataset = m_file->openDataSet(Hdf5PathUtils::normalizePath(datasetPath));
        for (const auto& kv : attrs) {
            if (!writeSingleAttribute(dataset, kv.first, kv.second)) {
                return false;
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}
