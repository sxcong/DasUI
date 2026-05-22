#include "binary_das_reader.h"
#include <cstdint>
#include <fstream>

bool BinaryDasReader::open(const std::string& filePath) {
    m_filePath = filePath;
    return !m_filePath.empty();
}

void BinaryDasReader::close() {
    m_filePath.clear();
}

bool BinaryDasReader::isOpen() const {
    return !m_filePath.empty();
}

bool BinaryDasReader::readMatrix(DasMatrix& matrix) {
    if (!isOpen()) return false;

    std::ifstream in(m_filePath, std::ios::binary);
    if (!in) return false;

    std::int32_t rows = 0;
    std::int32_t cols = 0;
    in.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    in.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    if (!in || rows <= 0 || cols <= 0) return false;

    matrix.rows = static_cast<std::size_t>(rows);
    matrix.cols = static_cast<std::size_t>(cols);
    matrix.data.resize(matrix.rows * matrix.cols);
    in.read(reinterpret_cast<char*>(matrix.data.data()),
            static_cast<std::streamsize>(matrix.data.size() * sizeof(double)));
    return static_cast<bool>(in);
}

