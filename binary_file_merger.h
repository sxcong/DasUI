#ifndef BINARY_FILE_MERGER_H
#define BINARY_FILE_MERGER_H

#include <string>
#include <vector>

//物理上增加文件长度
//Concatenation（动词：Concatenate）将两个或多个文件首尾相连（End-to-end），变成一个更大的文件。
bool mergeBinaryFiles(const std::vector<std::string>& files,
                      const std::string& outputFile,
                      std::string* errorMessage = nullptr);


//按中位数合并文件内容
//文件合并 (Merging / Aggregation)Merging 或 Aggregation 指的是将多个数据源的数值，按照某种逻辑进行“重合”处理，输出维度不变。
bool mergeBinaryFilesByMedian(const std::vector<std::string>& files,
                              const std::string& outputFile,
                              std::string* errorMessage);

#endif

