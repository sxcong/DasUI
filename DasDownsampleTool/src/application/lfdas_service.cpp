#include "lfdas_service.h"
#include <algorithm>
#include <fftw3.h>
#include <vector>

bool LfDasService::execute(const DownsampleConfig& config, std::string& errorMessage) {
    BinaryDasReader reader;
    if (!reader.open(config.inputFile)) {
        errorMessage = "failed to open input binary file";
        return false;
    }

    DasMatrix input;
    if (!reader.readMatrix(input)) {
        errorMessage = "failed to read input binary matrix";
        return false;
    }

    DasMatrix output = medianFilter(input, config.medianWindowSize);
    output = lowPassFilter(output, config.lfdasCutoffHz, 1.0);

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

DasMatrix LfDasService::medianFilter(const DasMatrix& input, std::size_t windowSize) const {
    if (input.empty() || windowSize <= 1) {
        return input;
    }
    if ((windowSize % 2) == 0) {
        ++windowSize;
    }

    const std::size_t radius = windowSize / 2;
    DasMatrix output;
    output.rows = input.rows;
    output.cols = input.cols;
    output.data.resize(output.rows * output.cols);

    std::vector<double> window;
    window.reserve(windowSize);

    for (std::size_t c = 0; c < input.cols; ++c) {
        for (std::size_t r = 0; r < input.rows; ++r) {
            window.clear();
            for (std::size_t k = 0; k < windowSize; ++k) {
                const std::size_t idx = (r + k < radius) ? 0 : std::min(input.rows - 1, r + k - radius);
                window.push_back(input.at(idx, c));
            }

            auto mid = window.begin() + static_cast<std::ptrdiff_t>(window.size() / 2);
            std::nth_element(window.begin(), mid, window.end());
            output.data[r * output.cols + c] = *mid;
        }
    }

    return output;
}

DasMatrix LfDasService::lowPassFilter(const DasMatrix& input, double cutoffHz, double sampleRateHz) const {
    if (input.empty() || cutoffHz <= 0.0 || sampleRateHz <= 0.0) {
        return input;
    }

    const std::size_t n = input.rows;
    const std::size_t nOut = n / 2 + 1;
    const std::size_t cutoffBin = static_cast<std::size_t>(cutoffHz * static_cast<double>(n) / sampleRateHz);

    DasMatrix output = input;
    std::vector<double> in(n, 0.0);
    std::vector<double> freq(2 * nOut, 0.0);
    std::vector<double> time(n, 0.0);
    fftw_plan forward = fftw_plan_dft_r2c_1d(static_cast<int>(n), in.data(),
                                             reinterpret_cast<fftw_complex*>(freq.data()),
                                             FFTW_ESTIMATE);
    fftw_plan inverse = fftw_plan_dft_c2r_1d(static_cast<int>(n),
                                             reinterpret_cast<fftw_complex*>(freq.data()),
                                             time.data(),
                                             FFTW_ESTIMATE);
    if (!forward || !inverse) {
        if (forward) fftw_destroy_plan(forward);
        if (inverse) fftw_destroy_plan(inverse);
        return input;
    }

    for (std::size_t c = 0; c < input.cols; ++c) {
        for (std::size_t r = 0; r < n; ++r) {
            in[r] = input.at(r, c);
        }

        fftw_execute(forward);

        for (std::size_t k = cutoffBin + 1; k < nOut; ++k) {
            freq[2 * k] = 0.0;
            freq[2 * k + 1] = 0.0;
        }

        fftw_execute(inverse);

        for (std::size_t r = 0; r < n; ++r) {
            output.data[r * output.cols + c] = time[r] / static_cast<double>(n);
        }
    }

    fftw_destroy_plan(forward);
    fftw_destroy_plan(inverse);
    return output;
}

