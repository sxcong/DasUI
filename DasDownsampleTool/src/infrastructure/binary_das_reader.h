#ifndef BINARY_DAS_READER_H
#define BINARY_DAS_READER_H

#include "../core/das_matrix.h"
#include <string>

class BinaryDasReader {
public:
    bool open(const std::string& filePath);
    void close();
    bool isOpen() const;
    bool readMatrix(DasMatrix& matrix);

private:
    std::string m_filePath;
};

#endif

