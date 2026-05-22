#ifndef HDF5_DAS_WRITER_H
#define HDF5_DAS_WRITER_H

#include "../core/idataset_writer.h"
#include <H5Cpp.h>

class Hdf5DasWriter : public IDatasetWriter {
public:
    Hdf5DasWriter();
    ~Hdf5DasWriter() override;

    bool open(const std::string& filePath) override;
    void close() override;
    bool isOpen() const override;
    bool writeDataset(const std::string& datasetPath, const DasMatrix& matrix) override;
    bool writeAttributes(const std::string& datasetPath, const std::map<std::string, std::string>& attrs) override;

private:
    bool createOrOpenDataset(const std::string& datasetPath, const DasMatrix& matrix, H5::DataSet& dataset);
    bool writeSingleAttribute(H5::DataSet& dataset, const std::string& name, const std::string& value);

    H5::H5File* m_file = nullptr;
};

#endif

