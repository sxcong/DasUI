#ifndef DAS_DOWNSAMPLE_SERVICE_H
#define DAS_DOWNSAMPLE_SERVICE_H

#include "../core/downsample_config.h"
#include "../infrastructure/hdf5_das_reader.h"
#include "../infrastructure/binary_das_writer.h"
#include "fk_service.h"
#include "lfdas_service.h"

class DasDownsampleService {
public:
    bool execute(const DownsampleConfig& config, std::string& errorMessage);

private:
    DasMatrix downsample(const DasMatrix& input, std::size_t rowStep, std::size_t colStep) const;
    DasMatrix computeFbe(const DasMatrix& input, double lowHz, double highHz,
                         std::size_t windowSize, std::size_t fftSize) const;
    static double calcSampleRate(std::size_t sampleCount, double durationSeconds = 60.0);
};

#endif
