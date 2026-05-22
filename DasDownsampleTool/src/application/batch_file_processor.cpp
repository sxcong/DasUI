#include "batch_file_processor.h"

bool BatchFileProcessor::popNextFile(std::vector<std::string>& files, std::string& filePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (files.empty()) {
        return false;
    }

    filePath = std::move(files.back());
    files.pop_back();
    return true;
}

bool BatchFileProcessor::run(std::vector<std::string>& files, std::size_t threadCount, const ProcessFn& processFn) {
    if (processFn == nullptr || files.empty()) {
        return false;
    }

    if (threadCount == 0) {
        threadCount = 1;
    }
    if (threadCount > files.size()) {
        threadCount = files.size();
    }

    std::atomic<bool> ok{true};
    std::vector<std::thread> workers;
    workers.reserve(threadCount);

    for (std::size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back([this, &files, &processFn, &ok]() {
            while (ok.load(std::memory_order_relaxed)) {
                std::string filePath;
                if (!popNextFile(files, filePath)) {
                    break;
                }

                if (!processFn(filePath)) {
                    ok.store(false, std::memory_order_relaxed);
                    break;
                }
            }
        });
    }

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    return ok.load(std::memory_order_relaxed);
}

