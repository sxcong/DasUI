#include "das_downsample_service.h"
#include <fftw3.h>
#include <cmath>
#include <vector>
#include <QDebug>

bool DasDownsampleService::execute(const DownsampleConfig& config, std::string& errorMessage) {
    if (config.mode == ProcessMode::LfDas) {
        LfDasService service;
        return service.execute(config, errorMessage);
    }

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

    DasMatrix output;
    if (config.mode == ProcessMode::Fbe) {
        output = computeFbe(input, config.fbeLowHz, config.fbeHighHz,
                            config.fbeWindowSize, config.fbeFftSize);
    } else if (config.mode == ProcessMode::Fk) {
        FkService service;
        return service.execute(config, errorMessage);
    } else {
        output = downsample(input, config.rowStep, config.colStep);
    }

    BinaryDasWriter writer;
    if (!writer.open(config.outputFile)) {
        errorMessage = "failed to open output file";
        return false;
    }
    if (!writer.writeMatrix(output)) {
        errorMessage = "failed to write output dataset";
        return false;
    }
    return true;
}

DasMatrix DasDownsampleService::downsample(const DasMatrix& input, std::size_t rowStep, std::size_t colStep) const {
    qDebug()<<"downsample"<<rowStep<<colStep<<input.rows<<input.cols;
    if (rowStep == 0) rowStep = 1;
    if (colStep == 0) colStep = 1;

    DasMatrix output;
    output.rows = input.rows == 0 ? 0 : ((input.rows - 1) / rowStep + 1);
    output.cols = input.cols == 0 ? 0 : ((input.cols - 1) / colStep + 1);
    output.data.reserve(output.rows * output.cols);

    for (std::size_t r = 0; r < input.rows; r += rowStep) {
        for (std::size_t c = 0; c < input.cols; c += colStep) {
            output.data.push_back(input.at(r, c));
        }
    }

    if (output.rows * output.cols != output.data.size() && output.cols != 0) {
        output.rows = output.data.size() / output.cols;
    }
    return output;
}

double DasDownsampleService::calcSampleRate(std::size_t sampleCount, double durationSeconds) {
    if (durationSeconds <= 0.0) {
        return 1.0;
    }
    return static_cast<double>(sampleCount) / durationSeconds;
}

DasMatrix DasDownsampleService::computeFbe(const DasMatrix& input, double lowHz, double highHz,
                                           std::size_t windowSize, std::size_t fftSize) const {
    DasMatrix output;
    if (input.empty() || input.rows < 2) {
        return output;
    }

    if (lowHz < 0.0) lowHz = 0.0;
    if (highHz <= lowHz) highHz = lowHz + 1.0;
    if (windowSize == 0) windowSize = 2000;
    if (fftSize == 0) fftSize = windowSize;
    if (fftSize > windowSize) fftSize = windowSize;

    const double sampleRate = calcSampleRate(input.rows, 60.0);
    const std::size_t windowCount = input.rows / windowSize;
    const std::size_t nOut = fftSize / 2 + 1;
    if (windowCount == 0) {
        return output;
    }

    output.rows = windowCount;
    output.cols = input.cols;
    output.data.assign(output.rows * output.cols, 0.0);

    std::vector<double> in(fftSize, 0.0);
    std::vector<double> out(2 * nOut, 0.0);
    fftw_plan plan = fftw_plan_dft_r2c_1d(static_cast<int>(fftSize), in.data(),
                                          reinterpret_cast<fftw_complex*>(out.data()),
                                          FFTW_ESTIMATE);
    if (!plan) {
        return DasMatrix{};
    }

    std::vector<int> bandBins;
    bandBins.reserve(nOut);
    for (std::size_t k = 0; k < nOut; ++k) {
        const double freq = static_cast<double>(k) * sampleRate / static_cast<double>(fftSize);
        if (freq >= lowHz && freq <= highHz) {
            bandBins.push_back(static_cast<int>(k));
        }
    }
    if (bandBins.empty()) {
        fftw_destroy_plan(plan);
        return DasMatrix{};
    }

    for (std::size_t w = 0; w < windowCount; ++w) {
        const std::size_t windowOffset = w * windowSize;

        for (std::size_t depth = 0; depth < input.cols; ++depth) {
            for (std::size_t i = 0; i < fftSize; ++i) {
                in[i] = input.at(windowOffset + i, depth);
            }

            fftw_execute(plan);

            double energy = 0.0;
            for (int k : bandBins) {
                const std::size_t idx = static_cast<std::size_t>(k);
                const double re = out[2 * idx];
                const double im = out[2 * idx + 1];
                energy += re * re + im * im;
            }

            output.data[w * output.cols + depth] = energy / static_cast<double>(bandBins.size());
        }
    }

    fftw_destroy_plan(plan);
    return output;
}
