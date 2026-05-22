#ifndef DOWNSAMPLE_CONFIG_H
#define DOWNSAMPLE_CONFIG_H

#include <string>

enum class ProcessMode {
    Downsample,
    Fbe,
    Fk,
    LfDas
};

struct DownsampleConfig {
    ProcessMode mode = ProcessMode::Downsample;
    std::string inputFile;
    std::string inputDataset = "/das/raw";
    std::string outputFile;
    std::size_t rowStep = 1;
    std::size_t colStep = 1;
    double fbeLowHz = 10.0;
    double fbeHighHz = 100.0;
    std::size_t fbeWindowSize = 2000;
    std::size_t fbeFftSize = 1024;
    std::size_t medianWindowSize = 5;
    double lfdasCutoffHz = 0.05;
    std::size_t fkTimePadFactor = 1;
    std::size_t fkDepthPadFactor = 1;
    double fkSampleRateHz = 2000.0;
    double fkDepthSpacing = 1.0;
    double fkVelocityMin = 0.0;
    double fkVelocityMax = 10000.0;
    bool fkCenterData = true;
    std::size_t fkTimeStart = 0;
    std::size_t fkTimeCount = 2000;
    std::size_t fkDepthStart = 0;
    std::size_t fkDepthCount = 0;
};

#endif
