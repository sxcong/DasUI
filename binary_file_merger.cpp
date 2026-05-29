#include "binary_file_merger.h"
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>
#include <QDebug>

namespace {

struct BinaryHeader {
    std::int32_t rows = 0;
    std::int32_t cols = 0;
};

bool readHeader(std::ifstream& in, BinaryHeader& header) {
    in.read(reinterpret_cast<char*>(&header.rows), sizeof(header.rows));
    in.read(reinterpret_cast<char*>(&header.cols), sizeof(header.cols));
    return static_cast<bool>(in) && header.rows > 0 && header.cols > 0;
}

void setError(std::string* errorMessage, const std::string& message) {
    if (errorMessage) {
        *errorMessage = message;
    }
}

}

//文件内容在时间上递增了
bool mergeBinaryFiles(const std::vector<std::string>& files,
                      const std::string& outputFile,
                      std::string* errorMessage)
{
    if (files.empty()) {
        setError(errorMessage, "empty input file list");
        return false;
    }

    std::int32_t outputCols = 0;
    std::int64_t totalRows = 0;

    for (const auto& file : files) {
        std::ifstream in(file, std::ios::binary);
        if (!in) {
            setError(errorMessage, "failed to open input file: " + file);
            return false;
        }

        BinaryHeader header;
        if (!readHeader(in, header)) {
            setError(errorMessage, "failed to read binary header: " + file);
            return false;
        }

        if (outputCols == 0) {
            outputCols = header.cols;
        } else if (header.cols != outputCols) {
            setError(errorMessage, "column count mismatch: " + file);
            return false;
        }

        totalRows += header.rows;
        if (totalRows > std::numeric_limits<std::int32_t>::max()) {
            setError(errorMessage, "merged row count exceeds int32 range");
            return false;
        }
    }

    qDebug()<<"mergeBinaryFiles"<<totalRows<<outputCols;

    std::ofstream out(outputFile, std::ios::binary | std::ios::trunc);
    if (!out) {
        setError(errorMessage, "failed to open output file: " + outputFile);
        return false;
    }

    const std::int32_t mergedRows = static_cast<std::int32_t>(totalRows);
    out.write(reinterpret_cast<const char*>(&mergedRows), sizeof(mergedRows));
    out.write(reinterpret_cast<const char*>(&outputCols), sizeof(outputCols));
    qDebug()<<"mergeBinaryFiles"<<mergedRows<<outputCols;

    constexpr std::size_t chunkDoubles = 1024 * 1024;
    std::vector<double> buffer(chunkDoubles);

    for (const auto& file : files) {
        std::ifstream in(file, std::ios::binary);
        if (!in) {
            setError(errorMessage, "failed to reopen input file: " + file);
            return false;
        }

        BinaryHeader header;
        if (!readHeader(in, header)) {
            setError(errorMessage, "failed to reread binary header: " + file);
            return false;
        }

        std::int64_t remaining = static_cast<std::int64_t>(header.rows) * header.cols;
        while (remaining > 0) {
            const std::size_t count = static_cast<std::size_t>(
                std::min<std::int64_t>(remaining, static_cast<std::int64_t>(buffer.size())));
            in.read(reinterpret_cast<char*>(buffer.data()),
                    static_cast<std::streamsize>(count * sizeof(double)));
            if (!in) {
                setError(errorMessage, "failed to read matrix payload: " + file);
                return false;
            }

            out.write(reinterpret_cast<const char*>(buffer.data()),
                      static_cast<std::streamsize>(count * sizeof(double)));
            if (!out) {
                setError(errorMessage, "failed to write merged payload");
                return false;
            }
            remaining -= static_cast<std::int64_t>(count);
        }
    }

    return true;
}



bool mergeBinaryFilesByMedian(const std::vector<std::string>& files,
                              const std::string& outputFile,
                              std::string* errorMessage)
{
    if (files.empty()) return false;

    // 1. 读取基础维度 (假设所有文件 Header 一致)
    std::ifstream in0(files[0], std::ios::binary);
    BinaryHeader header;
    in0.read(reinterpret_cast<char*>(&header.rows), 4);
    in0.read(reinterpret_cast<char*>(&header.cols), 4);

    const size_t totalElements = static_cast<size_t>(header.rows) * header.cols;
    const size_t numFiles = files.size();

    // 2. 核心优化：预分配内存
    // 创建一个二维布局的 buffer：[numFiles][totalElements]
    // 这样数据在内存中是连续的，利用 CPU 缓存预取
    std::vector<std::vector<double>> allData(numFiles, std::vector<double>(totalElements));

    // 3. 一次性加载所有文件 (顺序读取，速度最快)
    for (size_t f = 0; f < numFiles; ++f) {
        std::ifstream in(files[f], std::ios::binary);
        in.seekg(8); // 跳过 Header
        in.read(reinterpret_cast<char*>(allData[f].data()), totalElements * sizeof(double));
        if (!in) {
            if (errorMessage) *errorMessage = "Failed to read file: " + files[f];
            return false;
        }
    }

    // 4. 计算中位数 (内存寻址，极快)
    std::vector<double> medianResult(totalElements);
    std::vector<double> columnStack(numFiles); // 临时工作空间

    for (size_t i = 0; i < totalElements; ++i) {
        for (size_t f = 0; f < numFiles; ++f) {
            columnStack[f] = allData[f][i];
        }

#if 0
//中值合并
        auto mid = columnStack.begin() + columnStack.size() / 2;
        std::nth_element(columnStack.begin(), mid, columnStack.end());
        medianResult[i] = *mid;
#else
        //最大值合并
        auto maxIt = std::max_element(columnStack.begin(), columnStack.end());
        medianResult[i] = *maxIt;
#endif
    }

    // 5. 写入结果
    std::ofstream out(outputFile, std::ios::binary | std::ios::trunc);
    out.write(reinterpret_cast<const char*>(&header.rows), 4);
    out.write(reinterpret_cast<const char*>(&header.cols), 4);
    out.write(reinterpret_cast<const char*>(medianResult.data()), totalElements * sizeof(double));

    return true;
}
