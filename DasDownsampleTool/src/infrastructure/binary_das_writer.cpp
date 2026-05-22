#include "binary_das_writer.h"
#include <cstdint>
#include <fstream>

bool BinaryDasWriter::open(const std::string& filePath) {
    m_filePath = filePath;
    return !m_filePath.empty();
}

void BinaryDasWriter::close() {
    m_filePath.clear();
}

bool BinaryDasWriter::isOpen() const {
    return !m_filePath.empty();
}

bool BinaryDasWriter::writeMatrix(const DasMatrix& matrix) {
    if (!isOpen() || matrix.empty()) return false;

    std::ofstream out(m_filePath, std::ios::binary | std::ios::trunc);
    if (!out) return false;

    const std::int32_t rows = static_cast<std::int32_t>(matrix.rows);
    const std::int32_t cols = static_cast<std::int32_t>(matrix.cols);
    out.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    out.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
    out.write(reinterpret_cast<const char*>(matrix.data.data()),
              static_cast<std::streamsize>(matrix.data.size() * sizeof(double)));
    return static_cast<bool>(out);
}

