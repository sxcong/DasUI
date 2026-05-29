#ifndef PROCESSDOWNSAMPLE_H
#define PROCESSDOWNSAMPLE_H

#include "defines.h"
class ProcessDownSample
{
public:
    ProcessDownSample();
    //通过降采样因子和滤波阶数降采样
    void downSampleMultistage(Eigen::MatrixXf& rawData, Eigen::MatrixXf& result, int factor, int order, int sr);
public:
     void applyLayer(const Eigen::MatrixXf& input, Eigen::MatrixXf& output, int factor, int order);
private:
    int m_downsamplingFactor = 10; // 降采样因子
    int m_filterOrder = 20;        // 滤波阶数
    int current_sr = 2000;//当前采样率，初始为2000
};

#endif // PROCESSDOWNSAMPLE_H
