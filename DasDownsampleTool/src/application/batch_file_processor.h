#ifndef BATCH_FILE_PROCESSOR_H
#define BATCH_FILE_PROCESSOR_H

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class BatchFileProcessor {
public:
    using ProcessFn = std::function<bool(const std::string& filePath)>;

    bool run(std::vector<std::string>& files, std::size_t threadCount, const ProcessFn& processFn);

private:
    bool popNextFile(std::vector<std::string>& files, std::string& filePath);

    std::mutex m_mutex;
};

#endif

