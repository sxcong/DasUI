#ifndef HDF5_DAS_READER_H
#define HDF5_DAS_READER_H

#include "../core/idataset_reader.h"
#include <H5Cpp.h>

class Hdf5DasReader : public IDatasetReader {
public:
    Hdf5DasReader();
    ~Hdf5DasReader() override;

    bool open(const std::string& filePath) override;
    void close() override;
    bool isOpen() const override;
    std::vector<std::string> listDatasets(const std::string& groupPath = "/") const override;
    bool readDataset(const std::string& datasetPath, DasMatrix& matrix) override;
    std::map<std::string, std::string> readAttributes(const std::string& datasetPath) override;

private:
    std::vector<std::string> listDatasetsRecursive(const H5::Group& group, const std::string& prefix) const;
    bool readScalarAttribute(const H5::Attribute& attr, std::string& value) const;

    H5::H5File* m_file = nullptr;
};

#endif

