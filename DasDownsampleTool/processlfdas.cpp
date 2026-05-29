#include "processlfdas.h"
#include "iir/Butterworth.h"
#include <algorithm> // 用于 std::nth_element (实现中值滤波)
#include <vector>
#include "processdownsample.h"

ProcessLFDAS::ProcessLFDAS() {}
void ProcessLFDAS::processLF_DAS_Final(Eigen::MatrixXf& rawData, Eigen::MatrixXf& result, const LFDASConfig& cfg)
{
    m_cfg = cfg;
    //> 3382 col is invalid data

    if (rawData.cols() > 3382) {
        //rawData.rightCols(rawData.cols() - 3382).setZero();
        rawData.rightCols(rawData.cols() - 3382).setConstant(std::numeric_limits<float>::quiet_NaN());
    }

    //
    int newRows = (rawData.rows() / (cfg.raw_sr / cfg.target_sr));
    result.resize(newRows, rawData.cols());
    ProcessDownSample downSample;
    int sr = 2000;
    downSample.downSampleMultistage(rawData, result, 10, 20, sr);


    // 2. downsample (先滤波后抽取)
   // performAntiAliasingAndDecimation(rawData, result);

    // 3. 中值滤波 (无需改动参数，但窗口大小可选)
    for (int j = 0; j < result.cols(); ++j) {
        Eigen::VectorXf col = result.col(j);
        applyMedianFilter(col, 3);
        result.col(j) = col;
    }

    // 4. 频率特征提取 (调用上面改好的带通滤波)
    applyBandpassFilter(result);

    // 5. 去直流
    result = removeDCDrift(result);
}


//1 2 低通并降采样
void ProcessLFDAS::performAntiAliasingAndDecimation(const Eigen::MatrixXf& rawData_, Eigen::MatrixXf& result) {
    int factor = m_cfg.raw_sr / m_cfg.target_sr;
    // 动态滤波器设置：截止频率设为目标上限的 1.2 倍以防混叠
    Iir::Butterworth::LowPass<4> lp;
    lp.setup(m_cfg.raw_sr, m_cfg.high_cutoff * 1.2);

    for (int j = 0; j < rawData_.cols(); ++j) {
        lp.reset();
        int output_idx = 0;
        for (int i = 0; i < rawData_.rows(); ++i) {
            float filtered_val = lp.filter(rawData_(i, j));
            if ((i + 1) % factor == 0 && output_idx < result.rows()) {
                result(output_idx++, j) = filtered_val;
            }
        }
    }
}

void ProcessLFDAS::applyBandpassFilter(Eigen::MatrixXf& data)
{
    // 动态选择滤波器类型
    Iir::Butterworth::BandPass<4> bp;
    bp.setup(m_cfg.target_sr, m_cfg.low_cutoff, m_cfg.high_cutoff);

    for (int j = 0; j < data.cols(); ++j) {
        Eigen::VectorXf col = data.col(j);

        // 正向滤波
        bp.reset();
        for (int i = 0; i < col.size(); ++i) col(i) = bp.filter(col(i));

        // 反向滤波 (零相位)
        std::reverse(col.data(), col.data() + col.size());
        bp.reset();
        for (int i = 0; i < col.size(); ++i) col(i) = bp.filter(col(i));
        std::reverse(col.data(), col.data() + col.size());

        data.col(j) = col;
    }
}

//3 中值滤波
void ProcessLFDAS::applyMedianFilter(Eigen::VectorXf& col, int windowSize /*= 3*/) {
    int n = col.size();
    Eigen::VectorXf filtered = col;
    int offset = windowSize / 2;
    for (int i = offset; i < n - offset; ++i) {
        std::vector<float> window;
        for (int k = -offset; k <= offset; ++k) window.push_back(col(i + k));
        std::nth_element(window.begin(), window.begin() + offset, window.end());
        filtered(i) = window[offset];
    }
    col = filtered;
}


/**
 * @param data 已经降采样后的矩阵 (60行, 4001列)
 */
//4 第二次低通
void ProcessLFDAS::applySecondLowPass(Eigen::MatrixXf& data) {
    const double fs = 1.0;             // 当前采样率 1Hz
    const double cutoff = 0.05;        // 目标截止频率 0.05Hz

    // 使用 2 阶巴特沃斯滤波器 (0.05Hz 足够平缓，2阶通常足够，且数值最稳定)
    Iir::Butterworth::LowPass<2> lp;
    lp.setup(fs, cutoff);

    // 对每一列（每个深度点）进行滤波
    for (int j = 0; j < data.cols(); ++j) {
        lp.reset(); // 必须重置状态

        // 遍历时间轴（60个点）
        for (int i = 0; i < data.rows(); ++i) {
            data(i, j) = lp.filter(data(i, j));
        }
    }
}
//零相位滤波
void ProcessLFDAS::applyZeroPhaseLowPass(Eigen::MatrixXf& data)
{
    Iir::Butterworth::LowPass<2> lp;
    lp.setup(1.0, 0.05);

    for (int j = 0; j < data.cols(); ++j) {
        Eigen::VectorXf col = data.col(j);

        // 1. 正向滤波
        lp.reset();
        for (int i = 0; i < col.size(); ++i) col(i) = lp.filter(col(i));

        // 2. 反向滤波 (实现零相位偏移)
        std::reverse(col.data(), col.data() + col.size());
        lp.reset();
        for (int i = 0; i < col.size(); ++i) col(i) = lp.filter(col(i));
        std::reverse(col.data(), col.data() + col.size());

        data.col(j) = col;
    }
}

Eigen::MatrixXf ProcessLFDAS::removeDCDrift(const Eigen::MatrixXf& data)
{
    Eigen::MatrixXf out = data;
    float alpha = 0.001f;

    for (int ch = 0; ch < data.cols(); ++ch)
    {
        // 建议初始 dc 使用第一个点的值，减少启动时的阶跃响应
        float dc = data(0, ch);

        for (int i = 0; i < data.rows(); ++i)
        {
            // 更新直流偏移估计
            dc = (1.0f - alpha) * dc + alpha * data(i, ch);
            // 减去漂移
            out(i, ch) = data(i, ch) - dc;
        }
    }
    return out;
}
