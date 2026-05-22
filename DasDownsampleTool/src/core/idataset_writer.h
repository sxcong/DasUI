#ifndef IDATASET_WRITER_H
#define IDATASET_WRITER_H

#include "das_matrix.h"
#include <map>
#include <string>

class IDatasetWriter {
public:
    virtual ~IDatasetWriter() = default;
    virtual bool open(const std::string& filePath) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual bool writeDataset(const std::string& datasetPath, const DasMatrix& matrix) = 0;
    virtual bool writeAttributes(const std::string& datasetPath, const std::map<std::string, std::string>& attrs) = 0;
};

#endif

