
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include "hdfspectrumprocessor.h"
#include <QElapsedTimer>
#include "defines.h"

void testSpectrum(const std::string& inputFile, const std::string& outputPath)
{
    QFileInfo info(inputFile.c_str());
    std::string szFile = outputPath + info.fileName().toStdString() + ".spectrum";
    HDFSpectrumProcessor* pSpe = new HDFSpectrumProcessor();
    //pSpe->process(inputFile, szFile, 2382, 3365);//2382：3890米 ， 3365：4090米
    pSpe->process(inputFile, szFile, 2382, 3365);//2382：3890米 ， 3365：4090米
    //pSpe->process(inputFile, szFile, 0, 4000);
    delete pSpe;
    return;
}


int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    printInfo("main");
    if (argc != 3)
    {
        printInfo("argc = ");
        printInfo(QString::number(argc));
        return 0;
    }

    std::string in = "D:/HDF5/20251023/h5_20251023-135006.042214963_333.h5";
    std::string out = "d:/output/4/";
    QElapsedTimer timer;
    timer.start();
    //for test
    //testSpectrum(in, out);
    testSpectrum(argv[1], argv[2]);
    printInfo("testSpectrum timer.elapsed()");
    printInfo(QString::number(timer.elapsed()));
    return 0;
}
