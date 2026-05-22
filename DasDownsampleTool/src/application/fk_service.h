#ifndef FK_SERVICE_H
#define FK_SERVICE_H

#include "../core/downsample_config.h"
#include "../infrastructure/hdf5_das_reader.h"
#include "../infrastructure/binary_das_writer.h"

class FkService {
public:
    bool execute(const DownsampleConfig& config, std::string& errorMessage);

private:
    DasMatrix computeFk(const DasMatrix& input, const DownsampleConfig& config) const;
    DasMatrix selectRange(const DasMatrix& input, const DownsampleConfig& config) const;
    DasMatrix applyWindowAndPad(const DasMatrix& input, const DownsampleConfig& config,
                                std::size_t& outRows, std::size_t& outCols) const;
    void buildVelocityMask(std::vector<double>& mask,
                           std::size_t rows, std::size_t cols,
                           const DownsampleConfig& config) const;
};

#endif
