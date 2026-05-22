#ifndef IDATASET_READER_H
#define IDATASET_READER_H

#include "das_matrix.h"
#include <H5Cpp.h>
#include <map>
#include <string>
#include <vector>

class IDatasetReader {
public:
    virtual ~IDatasetReader() = default;
    virtual bool open(const std::string& filePath) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual std::vector<std::string> listDatasets(const std::string& groupPath = "/") const = 0;
    virtual bool readDataset(const std::string& datasetPath, DasMatrix& matrix) = 0;
    virtual std::map<std::string, std::string> readAttributes(const std::string& datasetPath) = 0;
};

#endif
