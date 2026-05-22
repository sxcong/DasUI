#ifndef HD5READER_H
#define HD5READER_H

#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include <H5Cpp.h>

class Hd5Reader {
public:
    Hd5Reader();
    explicit Hd5Reader(const std::string& filepath);
    ~Hd5Reader();

    bool open(const std::string& filepath);
    void close();
    bool isOpen() const;

    std::vector<std::string> listDatasets(const std::string& groupPath = "/") const;
    bool hasDataset(const std::string& datasetName) const;
    bool getDims(const std::string& datasetName, hsize_t dims[2]) const;

    bool readDataset(const std::string& datasetName, Eigen::MatrixXf& matrix);
    std::map<std::string, std::string> readAttributes(const std::string& datasetName);
    std::string readAttribute(const std::string& datasetName, const std::string& attrName);

private:
    H5::H5File* m_file;
    bool m_isOpen;
};

#endif // HD5READER_H
