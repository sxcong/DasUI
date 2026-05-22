#ifndef LFDAS_SERVICE_H
#define LFDAS_SERVICE_H

#include "../core/downsample_config.h"
#include "../infrastructure/binary_das_reader.h"
#include "../infrastructure/binary_das_writer.h"

class LfDasService {
public:
    bool execute(const DownsampleConfig& config, std::string& errorMessage);

private:
    DasMatrix medianFilter(const DasMatrix& input, std::size_t windowSize) const;
    DasMatrix lowPassFilter(const DasMatrix& input, double cutoffHz, double sampleRateHz) const;
};

#endif

