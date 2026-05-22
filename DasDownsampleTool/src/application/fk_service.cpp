#include "fk_service.h"
#include <fftw3.h>
#include <algorithm>
#include <cmath>
#include <vector>

static std::size_t nextPow2(std::size_t v)
{
    if (v <= 1) return 1;
    --v;
    for (std::size_t shift = 1; shift < sizeof(std::size_t) * 8; shift <<= 1) {
        v |= v >> shift;
    }
    return v + 1;
}

bool FkService::execute(const DownsampleConfig& config, std::string& errorMessage) {
    Hdf5DasReader reader;
    if (!reader.open(config.inputFile)) {
        errorMessage = "failed to open input file";
        return false;
    }

    DasMatrix input;
    if (!reader.readDataset(config.inputDataset, input)) {
        errorMessage = "failed to read input dataset";
        return false;
    }

    DasMatrix selected = selectRange(input, config);
    if (selected.empty()) {
        errorMessage = "empty fk selected range";
        return false;
    }

    DasMatrix output = computeFk(selected, config);

    BinaryDasWriter writer;
    if (!writer.open(config.outputFile)) {
        errorMessage = "failed to open output file";
        return false;
    }
    if (!writer.writeMatrix(output)) {
        errorMessage = "failed to write output file";
        return false;
    }
    return true;
}

DasMatrix FkService::selectRange(const DasMatrix& input, const DownsampleConfig& config) const
{
    DasMatrix output;
    if (input.empty() || config.fkTimeStart >= input.rows || config.fkDepthStart >= input.cols) {
        return output;
    }

    const std::size_t timeCount = std::min(config.fkTimeCount, input.rows - config.fkTimeStart);
    const std::size_t depthCount = config.fkDepthCount == 0
        ? input.cols - config.fkDepthStart
        : std::min(config.fkDepthCount, input.cols - config.fkDepthStart);

    if (timeCount == 0 || depthCount == 0) {
        return output;
    }

    output.rows = timeCount;
    output.cols = depthCount;
    output.data.resize(output.rows * output.cols);

    for (std::size_t r = 0; r < output.rows; ++r) {
        for (std::size_t c = 0; c < output.cols; ++c) {
            output.data[r * output.cols + c] = input.at(config.fkTimeStart + r, config.fkDepthStart + c);
        }
    }

    return output;
}

DasMatrix FkService::applyWindowAndPad(const DasMatrix& input, const DownsampleConfig& config,
                                       std::size_t& outRows, std::size_t& outCols) const
{
    const std::size_t timePad = std::max<std::size_t>(1, config.fkTimePadFactor);
    const std::size_t depthPad = std::max<std::size_t>(1, config.fkDepthPadFactor);
    outRows = nextPow2(input.rows * timePad);
    outCols = nextPow2(input.cols * depthPad);

    DasMatrix padded;
    padded.rows = outRows;
    padded.cols = outCols;
    padded.data.assign(outRows * outCols, 0.0);

    const double twoPi = 2.0 * std::acos(-1.0);
    const double rowDen = std::max<std::size_t>(1, input.rows - 1);
    const double colDen = std::max<std::size_t>(1, input.cols - 1);

    for (std::size_t r = 0; r < input.rows; ++r) {
        const double wr = 0.5 - 0.5 * std::cos(twoPi * static_cast<double>(r) / rowDen);
        for (std::size_t c = 0; c < input.cols; ++c) {
            const double wc = 0.5 - 0.5 * std::cos(twoPi * static_cast<double>(c) / colDen);
            padded.data[r * outCols + c] = input.at(r, c) * wr * wc;
        }
    }
    return padded;
}

void FkService::buildVelocityMask(std::vector<double>& mask,
                                  std::size_t rows, std::size_t cols,
                                  const DownsampleConfig& config) const
{
    mask.assign(rows * cols, 0.0);
    const double sr = config.fkSampleRateHz;
    const double dz = config.fkDepthSpacing;
    const double vMin = std::max(0.0, config.fkVelocityMin);
    const double vMax = std::max(vMin, config.fkVelocityMax);

    const std::size_t rowCenter = rows / 2;
    const std::size_t colCenter = cols / 2;

    for (std::size_t r = 0; r < rows; ++r) {
        const double freqIndex = static_cast<double>(static_cast<long long>(r) - static_cast<long long>(rowCenter));
        const double freq = freqIndex * sr / static_cast<double>(rows);
        for (std::size_t c = 0; c < cols; ++c) {
            const double waveIndex = static_cast<double>(static_cast<long long>(c) - static_cast<long long>(colCenter));
            const double waveNum = waveIndex / (static_cast<double>(cols) * dz);

            double vel = 0.0;
            if (std::abs(waveNum) > 1e-12) {
                vel = std::abs(freq) / std::abs(waveNum);
            }

            const bool pass = (std::abs(waveNum) < 1e-12 && vMax > 0.0) || (vel >= vMin && vel <= vMax);
            mask[r * cols + c] = pass ? 1.0 : 0.0;
        }
    }
}

DasMatrix FkService::computeFk(const DasMatrix& input, const DownsampleConfig& config) const {
    DasMatrix output;
    if (input.empty()) {
        return output;
    }

    std::size_t rows = 0;
    std::size_t cols = 0;
    DasMatrix work = applyWindowAndPad(input, config, rows, cols);
    const std::size_t n = rows * cols;

    fftw_complex* in = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * n));
    fftw_complex* out = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * n));
    if (!in || !out) {
        if (in) fftw_free(in);
        if (out) fftw_free(out);
        return output;
    }

    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            const std::size_t idx = r * cols + c;
            double v = work.data[idx];
            if (config.fkCenterData && ((r + c) & 1U)) {
                v = -v;
            }
            in[idx][0] = v;
            in[idx][1] = 0.0;
        }
    }

    fftw_plan forward = fftw_plan_dft_2d(static_cast<int>(rows), static_cast<int>(cols), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    if (!forward) {
        fftw_free(in);
        fftw_free(out);
        return output;
    }
    fftw_execute(forward);

    std::vector<double> mask;
    buildVelocityMask(mask, rows, cols, config);
    for (std::size_t i = 0; i < n; ++i) {
        out[i][0] *= mask[i];
        out[i][1] *= mask[i];
    }

    fftw_plan inverse = fftw_plan_dft_2d(static_cast<int>(rows), static_cast<int>(cols), out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
    if (!inverse) {
        fftw_destroy_plan(forward);
        fftw_free(in);
        fftw_free(out);
        return output;
    }
    fftw_execute(inverse);

    output.rows = input.rows;
    output.cols = input.cols;
    output.data.resize(output.rows * output.cols);
    const double scale = 1.0 / static_cast<double>(rows * cols);
    for (std::size_t r = 0; r < input.rows; ++r) {
        for (std::size_t c = 0; c < input.cols; ++c) {
            const std::size_t idx = r * cols + c;
            double v = in[idx][0] * scale;
            if (config.fkCenterData && ((r + c) & 1U)) {
                v = -v;
            }
            output.data[r * output.cols + c] = v;
        }
    }

    fftw_destroy_plan(forward);
    fftw_destroy_plan(inverse);
    fftw_free(in);
    fftw_free(out);
    return output;
}
