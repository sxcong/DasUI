#include "processdownsample.h"
#include "iir/Butterworth.h"

ProcessDownSample::ProcessDownSample() {}

// 封装的四层级联计算
void ProcessDownSample::downSampleMultistage(Eigen::MatrixXf& rawData, Eigen::MatrixXf& result, int factor, int order, int sr)
{
    m_downsamplingFactor = factor;
    m_filterOrder = order;
    // 例如：总倍数 2000 = 10 * 10 * 10 * 2
    std::vector<int> factors = {10, 10, 10, 2};
    Eigen::MatrixXf workingData = rawData;
    current_sr = 2000.0;

    for (int f : factors) {
        Eigen::MatrixXf nextLayer;
        applyLayer(workingData, nextLayer, f, m_filterOrder);
        workingData = nextLayer;
        current_sr /= f;
    }
    result = workingData;
}


// 核心函数：单层滤波+降采样
void ProcessDownSample::applyLayer(const Eigen::MatrixXf& input, Eigen::MatrixXf& output, int factor, int order)
{
    int newRows = input.rows() / factor;
    int cols = input.cols();
    output.resize(newRows, cols);

    // 使用 Butterworth 滤波器 (每列独立处理，需注意重置滤波器状态)
    // 假设你已有一个 IIR::Butterworth::LowPass<order> 包装类
    // 注意：filterOrder 需要在编译期确定，如果需动态，可用动态滤波器库

#pragma omp parallel for
    for (int j = 0; j < cols; ++j) {
        Iir::Butterworth::LowPass<4> lp; // 以 4 阶为例，或使用动态阶数滤波器
        lp.setup(current_sr, (current_sr / factor) / 2.0 * 0.9); // 截止频率设为新奈奎斯特频率的 90%

        int out_idx = 0;
        for (int i = 0; i < input.rows(); ++i) {
            float val = lp.filter(input(i, j));
            if ((i + 1) % factor == 0 && out_idx < newRows) {
                output(out_idx++, j) = val;
            }
        }
    }
}
