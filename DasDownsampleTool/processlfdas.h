#ifndef PROCESSLFDAS_H
#define PROCESSLFDAS_H
#include "defines.h"

struct LFDASConfig {
    int raw_sr = 2000;           // 原始采样率
    int target_sr = 200;         // 目标降采样率 (必须 > 2 * 最高频率)
    float low_cutoff = 10.0f;    // 带通下限
    float high_cutoff = 50.0f;   // 带通上限
};

class ProcessLFDAS
{
public:
    ProcessLFDAS();
    void processLF_DAS_Final(Eigen::MatrixXf& rawData, Eigen::MatrixXf& result, const LFDASConfig& cfg);

private:
    //1 2 低通并降采样
    void performAntiAliasingAndDecimation(const Eigen::MatrixXf& rawData_, Eigen::MatrixXf& result);
    //3 中值滤布
    void applyMedianFilter(Eigen::VectorXf& col, int windowSize = 3);
    //4 再次低通滤波
    void applySecondLowPass(Eigen::MatrixXf& data);
    Eigen::MatrixXf removeDCDrift(const Eigen::MatrixXf& data);

    //5 去直流
    void applyZeroPhaseLowPass(Eigen::MatrixXf& data);

    //带通滤波,用于计算10-50Hz这样的需求
    void applyBandpassFilter(Eigen::MatrixXf& data);

private:
    LFDASConfig m_cfg;
};



#endif // PROCESSLFDAS_H
