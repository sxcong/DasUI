#ifndef BINARY_DAS_WRITER_H
#define BINARY_DAS_WRITER_H

#include "../core/das_matrix.h"
#include <string>

class BinaryDasWriter {
public:
    bool open(const std::string& filePath);
    void close();
    bool isOpen() const;
    bool writeMatrix(const DasMatrix& matrix);

private:
    std::string m_filePath;
};

#endif

