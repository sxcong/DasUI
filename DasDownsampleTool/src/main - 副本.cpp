#include "application/das_downsample_service.h"
#include <QCoreApplication>
#include <QDebug>
#include <stdexcept>
#include <iostream>

static void printUsage() {
    std::cout
        << "DasDownsampleTool\n"
        << "Usage:\n"
        << "  DasDownsampleTool --mode downsample|fbe --input in.h5 --dataset /path --output out.bin\n"
        << "                   [--row-step N] [--col-step N]\n"
        << "                   [--fbe-low 10] [--fbe-high 100] [--fbe-band 10]\n";
}

static bool parseArgs(int argc, char* argv[], DownsampleConfig& config) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto needValue = [&](const char* name) -> std::string {
            if (i + 1 >= argc) {
                throw std::runtime_error(std::string("missing value for ") + name);
            }
            return argv[++i];
        };

        if (arg == "--mode") {
            std::string mode = needValue("--mode");
            if (mode == "downsample") config.mode = ProcessMode::Downsample;
            else if (mode == "fbe") config.mode = ProcessMode::Fbe;
            else throw std::runtime_error("unknown mode: " + mode);
        }
        else if (arg == "--input") config.inputFile = needValue("--input");
        else if (arg == "--dataset") config.inputDataset = needValue("--dataset");
        else if (arg == "--output") config.outputFile = needValue("--output");
        else if (arg == "--row-step") config.rowStep = static_cast<std::size_t>(std::stoul(needValue("--row-step")));
        else if (arg == "--col-step") config.colStep = static_cast<std::size_t>(std::stoul(needValue("--col-step")));
        else if (arg == "--fbe-low") config.fbeLowHz = std::stod(needValue("--fbe-low"));
        else if (arg == "--fbe-high") config.fbeHighHz = std::stod(needValue("--fbe-high"));
        else if (arg == "--fbe-band") config.fbeBandWidthHz = std::stod(needValue("--fbe-band"));
        else if (arg == "--help" || arg == "-h") return false;
        else throw std::runtime_error("unknown argument: " + arg);
    }
    return true;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    DownsampleConfig config;
#if 0
    config.mode = ProcessMode::Downsample;
    config.inputFile = "D:/HDF5/1.h5";
    config.outputFile = "D:/HDF5/output.bin";
    config.inputDataset = "/default";
    config.rowStep = 2000;
    config.colStep = 1;
    config.fbeLowHz = 10;
    config.fbeHighHz = 100;
    config.fbeBandWidthHz = 10;
#else
    config.mode = ProcessMode::Fbe;
    config.inputFile = "D:/HDF5/1.h5";
    config.outputFile = "D:/HDF5/fbeoutput.bin";
    config.inputDataset = "/default";
    config.rowStep = 2000;
    config.colStep = 1;
    config.fbeLowHz = 10;
    config.fbeHighHz = 100;
    config.fbeBandWidthHz = 10;
#endif
    /*
     * fbeBandWidthHz 是 FBE 里每个频带的宽度，单位 Hz。

比如：

fbeLowHz = 10
fbeHighHz = 100
fbeBandWidthHz = 10
就会把 10 到 100 Hz 切成每 10 Hz 一段：

10-20
20-30
...
90-100
它越小，频带越细，输出列数越多；越大，频带越粗，输出列数越少。

现在这份代码里，FBE 输出的每一列就是一个频带的能量值。
     */

    try {
        if (!parseArgs(argc, argv, config)) {
            printUsage();
            return 0;
        }
    } catch (const std::exception& ex) {
        qCritical() << ex.what();
        printUsage();
        return 1;
    }

    if (config.inputFile.empty() || config.outputFile.empty()) {
        printUsage();
        return 1;
    }

    DasDownsampleService service;
    std::string error;
    if (!service.execute(config, error)) {
        qCritical() << error.c_str();
        return 1;
    }

    qInfo() << "done";
    return 0;
}
