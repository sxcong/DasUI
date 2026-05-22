#include "application/das_downsample_service.h"
#include "application/batch_file_processor.h"
#include <QCoreApplication>
#include <QDebug>
#include <stdexcept>
#include <iostream>
#include <vector>
static std::mutex hdf5Mutex;
static void printUsage() {
    std::cout
        << "DasDownsampleTool\n"
        << "Usage:\n"
        << "  DasDownsampleTool --mode downsample|fbe|fk|lfdas --input in.h5|in.bin --dataset /path --output out.bin\n"
        << "                   [--row-step N] [--col-step N]\n"
        << "                   [--fbe-low 10] [--fbe-high 100] [--fbe-window 2000] [--fbe-fft 1024]\n"
        << "                   [--median-window 5] [--lowpass 0.05]\n"
        << "                   [--fk-sr 2000] [--fk-dz 1] [--fk-vmin 0] [--fk-vmax 10000]\n"
        << "                   [--fk-time-start 0] [--fk-time-count 2000] [--fk-depth-start 100] [--fk-depth-count 101]\n";
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
            else if (mode == "fk") config.mode = ProcessMode::Fk;
            else if (mode == "lfdas") config.mode = ProcessMode::LfDas;
            else throw std::runtime_error("unknown mode: " + mode);
        }
        else if (arg == "--input") config.inputFile = needValue("--input");
        else if (arg == "--dataset") config.inputDataset = needValue("--dataset");
        else if (arg == "--output") config.outputFile = needValue("--output");
        else if (arg == "--row-step") config.rowStep = static_cast<std::size_t>(std::stoul(needValue("--row-step")));
        else if (arg == "--col-step") config.colStep = static_cast<std::size_t>(std::stoul(needValue("--col-step")));
        else if (arg == "--fbe-low") config.fbeLowHz = std::stod(needValue("--fbe-low"));
        else if (arg == "--fbe-high") config.fbeHighHz = std::stod(needValue("--fbe-high"));
        else if (arg == "--fbe-window") config.fbeWindowSize = static_cast<std::size_t>(std::stoul(needValue("--fbe-window")));
        else if (arg == "--fbe-fft") config.fbeFftSize = static_cast<std::size_t>(std::stoul(needValue("--fbe-fft")));
        else if (arg == "--median-window") config.medianWindowSize = static_cast<std::size_t>(std::stoul(needValue("--median-window")));
        else if (arg == "--lowpass") config.lfdasCutoffHz = std::stod(needValue("--lowpass"));
        else if (arg == "--fk-sr") config.fkSampleRateHz = std::stod(needValue("--fk-sr"));
        else if (arg == "--fk-dz") config.fkDepthSpacing = std::stod(needValue("--fk-dz"));
        else if (arg == "--fk-vmin") config.fkVelocityMin = std::stod(needValue("--fk-vmin"));
        else if (arg == "--fk-vmax") config.fkVelocityMax = std::stod(needValue("--fk-vmax"));
        else if (arg == "--fk-time-start") config.fkTimeStart = static_cast<std::size_t>(std::stoul(needValue("--fk-time-start")));
        else if (arg == "--fk-time-count") config.fkTimeCount = static_cast<std::size_t>(std::stoul(needValue("--fk-time-count")));
        else if (arg == "--fk-depth-start") config.fkDepthStart = static_cast<std::size_t>(std::stoul(needValue("--fk-depth-start")));
        else if (arg == "--fk-depth-count") config.fkDepthCount = static_cast<std::size_t>(std::stoul(needValue("--fk-depth-count")));
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
    config.fbeWindowSize = 2000;
    config.fbeFftSize = 1024;
    config.medianWindowSize = 5;
    config.lfdasCutoffHz = 0.05;
    config.fkTimePadFactor = 1;
    config.fkDepthPadFactor = 1;
    config.fkSampleRateHz = 2000.0;
    config.fkDepthSpacing = 1.0;
    config.fkVelocityMin = 0.0;
    config.fkVelocityMax = 10000.0;
    config.fkCenterData = true;
    config.fkTimeStart = 0;
    config.fkTimeCount = 2000;
    config.fkDepthStart = 100;
    config.fkDepthCount = 101;
/*#else
    config.mode = ProcessMode::Fbe;
    config.inputFile = "D:/HDF5/1.h5";
    config.outputFile = "D:/HDF5/fbeoutput.bin";
    config.inputDataset = "/default";
    config.rowStep = 2000;
    config.colStep = 1;
    config.fbeLowHz = 10;
    config.fbeHighHz = 100;
    config.fbeWindowSize = 2000;
    config.fbeFftSize = 1024;
    config.medianWindowSize = 5;
    config.lfdasCutoffHz = 0.05;

#else
    config.mode = ProcessMode::LfDas;
    config.inputFile = "D:/HDF5/output.bin";
    config.outputFile = "D:/HDF5/ld_output.bin";
    config.inputDataset = "/default";
    config.rowStep = 2000;
    config.colStep = 1;
    config.fbeLowHz = 10;
    config.fbeHighHz = 100;
    config.fbeWindowSize = 2000;
    config.fbeFftSize = 1024;
    config.medianWindowSize = 5;
    config.lfdasCutoffHz = 0.05;
    config.fkTimePadFactor = 1;
    config.fkDepthPadFactor = 1;
    config.fkSampleRateHz = 2000.0;
    config.fkDepthSpacing = 1.0;
    config.fkVelocityMin = 0.0;
    config.fkVelocityMax = 10000.0;
    config.fkCenterData = true;*/

#else
  /*  config.mode = ProcessMode::Fk;
    config.inputFile = "D:/HDF5/1.h5";
    config.outputFile = "D:/HDF5/fk_output.bin";
    config.inputDataset = "/default";
    config.rowStep = 2000;
    config.colStep = 1;
    config.fbeLowHz = 10;
    config.fbeHighHz = 100;
    config.fbeWindowSize = 2000;
    config.fbeFftSize = 1024;
    config.medianWindowSize = 5;
    config.lfdasCutoffHz = 0.05;
    config.fkTimePadFactor = 1;
    config.fkDepthPadFactor = 1;
    config.fkSampleRateHz = 2000.0;
    config.fkDepthSpacing = 1.0;
    config.fkVelocityMin = 0.0;
    config.fkVelocityMax = 10000.0;
    config.fkCenterData = true;
    config.fkTimeStart = 0;
    config.fkTimeCount = 2000;
    config.fkDepthStart = 100;
    config.fkDepthCount = 101;*/
#endif

   /* try {
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
    }*/

#if 1
    std::vector<std::string> files = {
        "D:/HDF5/1.h5",
        "D:/HDF5/2.h5",
        "D:/HDF5/3.h5"
    };
    BatchFileProcessor batch;
    const std::size_t threadCount = 3;
    bool ok = batch.run(files, threadCount, [&](const std::string& filePath) {

        DownsampleConfig taskConfig = config;
        taskConfig.mode = ProcessMode::Downsample;
        taskConfig.rowStep = 2000;
        taskConfig.colStep = 1;
        taskConfig.inputDataset = "/default";
        taskConfig.inputFile = filePath;
        taskConfig.outputFile = filePath + ".down.bin";

        std::lock_guard<std::mutex> lock(hdf5Mutex);
        DasDownsampleService service;
        std::string error;
        if (!service.execute(taskConfig, error)) {
            qCritical() << "failed:" << filePath.c_str() << error.c_str();
            return false;
        }
        qInfo() << "done:" << filePath.c_str();
        return true;
    });

    qInfo() << "batch done, remaining files:" << static_cast<int>(files.size());
    return ok ? 0 : 1;
#endif

    DasDownsampleService service;
    std::string error;
    if (!service.execute(config, error)) {
        qCritical() << error.c_str();
        return 1;
    }

    qInfo() << "done";
    return 0;
}
