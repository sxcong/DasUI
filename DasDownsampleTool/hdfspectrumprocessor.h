#ifndef HDFSPECTRUMPROCESSOR_H
#define HDFSPECTRUMPROCESSOR_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>


//using namespace Eigen;
#include "defines.h"
#include "processlfdas.h"

class HDFSpectrumProcessor
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    //如果 HDFSpectrumProcessor 被拷贝（Copy Constructor），指针 plan_、fftInput_ 等会被浅拷贝。当两个实例析构时，会导致重复 free 或逻辑错误。
    HDFSpectrumProcessor(const HDFSpectrumProcessor&) = delete;
    HDFSpectrumProcessor& operator=(const HDFSpectrumProcessor&) = delete;

    HDFSpectrumProcessor()
    {
        initFFTW();
        initWindow();

        hdfBuffer_.resize(m_rows * m_cols);
        workBuffer_.resize(FFT_SIZE);
    }

    ~HDFSpectrumProcessor()
    {
        releaseFFTW();
    }


    const Matrix& getFBE() const { return fbe_; }
    const Matrix& getSpectrum() const { return spectrum_; }
    const Matrix& getSpectrumDb() const { return spectrumDb_; }
    const Matrix& getDownSample() const { return downData_; }
    const Matrix& getBandEnergy() const { return bandEnergy_; }
    const Matrix& getFK() const { return fkData_; }

bool process(const std::string& h5file,
             const std::string& outBin,
             int beginCol,
                 int endCol);

private:
    //Downsample 简单降采样，直接从2000通过平均变成1，mean() 方法其实相当于一个“全带通滤波器”，它没有抑制高频噪声，所以需要调用ProcessDownSample里的方法。
    void downSample();

    //calc 分频能量曲线
    void calcBandEnergy(const std::vector<std::pair<int,int>>& bands);

    //calc FBE 分频能量（Frequency Band Energy)
    void calcFBE(int lowHz = 10, int highHz = 100);

    //calc spectrum 频谱
    void calcSpectrum();
    void toDbSpectrum();
    void normalizeRelativeDb();

    //cald lfdas 低频应变提取
    void processLFDAS();

    //calc fk
   
private:

    void initWindow();
    void initFFTW();
    void releaseFFTW();

    void normalizeDB(float range);
    void removeMean(Vector& x);
    void applyWindow(Vector& x);

    bool readHDF(const std::string& file);

private:
    using Matrix = Eigen::MatrixXf;
    using Vector = Eigen::VectorXf;

    ProcessLFDAS m_processLFDAS;

    //static constexpr int TOTAL_ROWS = 120000;
    //static constexpr int TOTAL_COLS = 4001;

    static constexpr int SAMPLE_RATE = 2000;
    static constexpr int SEC_COUNT = 60;

    static constexpr int FFT_SIZE = 2000;
    static constexpr int FFT_BINS = FFT_SIZE / 2 + 1;
    static constexpr int FBE_FFT_SIZE = 1024;
    static constexpr int FBE_FFT_BINS = FBE_FFT_SIZE / 2 + 1;

private:
    fftwf_plan plan_{};
    fftwf_plan fbePlan_{};
    float* fftInput_{};
    float* fbeInput_{};
    fftwf_complex* fftOutput_{};
    fftwf_complex* fbeOutput_{};

private:
    std::vector<float> hdfBuffer_;

    Matrix rawData_;
    //RowMajorMatrix rawData_;
    Matrix downData_;
    Matrix fbe_;
    Matrix spectrum_;
    Matrix spectrumDb_;
    Matrix bandEnergy_;
    Matrix lsdas_;
    Matrix fkData_;

    Vector window_;
    Vector workBuffer_;

    int beginCol_ = 0;
    int currentCols_ = 0;
    int processedSecCount_ = 0;

    int m_rows = 120000;
    int m_cols = 4001;

};


class FKProcessor {
public:
    FKProcessor(int pad_k, int n_time, double fs = 2000.0)
        : pad_k_(std::max(1, pad_k)), n_time_(std::max(1, n_time)), fs_(fs) {
        allocate_resources(pad_k_, n_time_);
        precompute_hanning();
    }

    ~FKProcessor() {
        cleanup();
    }

    Eigen::MatrixXd process_segment(const Eigen::Ref<const Eigen::MatrixXd>& data, float f_limit = 400.0f) {
        int n_ch = (int)data.rows();
        int n_samples = (int)data.cols();

        if (n_ch <= 0 || n_samples <= 0) {
            return Eigen::MatrixXd();
        }

        // --- 1. 动态尺寸检查：防止 pad_to 变动导致的崩溃 ---
        // 如果输入数据列数 n_samples 与初始化时的 n_time_ 不符，需要重建计划
        int required_pad_k = pad_k_;
        if (n_ch > required_pad_k) {
            required_pad_k = next_pow2(n_ch);
        }
        if (n_samples != n_time_ || required_pad_k != pad_k_) {
            reinitialize(required_pad_k, n_samples);
        }

        // 2. 映射并清零输入缓冲区 (RowMajor 对应 FFTW 布局)
        Eigen::Map<Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>
            input_map(reinterpret_cast<std::complex<double>*>(in_), pad_k_, n_time_);
        input_map.setZero();

        // 3. 空间维预处理：共模抑制 (CMR)
        // 减去每一时刻的平均值，有效消除图中垂直的系统噪声条纹
        // 4. 预计算当前段的空间窗 (长度为实际通道数 n_ch)
        Eigen::VectorXd win_k = Eigen::VectorXd::Zero(n_ch);
        if (n_ch == 1) {
            win_k.setOnes();
        }
        else {
            win_k.setOnes();
            const int taper = std::max(1, n_ch / 10);//建议 taper 设置为 n_ch / 8 到 n_ch / 10 之间是合理的
            for (int i = 0; i < n_ch; ++i) {
                if (i < taper) {
                    win_k(i) = 0.5 * (1.0 - std::cos(M_PI * i / taper));
                }
                else if (i >= n_ch - taper) {
                    win_k(i) = 0.5 * (1.0 - std::cos(M_PI * (n_ch - 1 - i) / taper));
                }
            }
        }

        int offset_k = (pad_k_ - n_ch) / 2;

        Eigen::VectorXd spatial_mean = data.colwise().mean();
        //注：CMR 只有在通道很多时，才能区分“全场共有的系统噪声”和“局部传播的信号”。
                // 5. 数据填充 + 双向加窗
//#pragma omp parallel for
        for (int i = 0; i < n_ch; ++i) {
            // 减去空间均值(CMR)
            Eigen::ArrayXd row_data = data.row(i).array();
            row_data -= spatial_mean.array();
            // 减去通道自身均值 (Temporal DC)
            row_data -= row_data.mean();
            // 应用时间窗 & 空间窗
            input_map.row(offset_k + i) = (row_data * hanning_.array() * win_k(i)).cast<std::complex<double>>();
        }

        // 6. 执行 2D FFT
        fftw_execute(plan_);

        // 7. 频率截断提取 (含正负频率)
        Eigen::Map<Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>
            output_map(reinterpret_cast<std::complex<double>*>(out_), pad_k_, n_time_);

        int n_keep = static_cast<int>(f_limit * n_time_ / fs_);
        if (n_keep > n_time_ / 2) n_keep = n_time_ / 2;

        // 提取正频率 [0 : n_keep] 和 负频率 [end-n_keep+1 : end]
        Eigen::MatrixXcd reduced(pad_k_, 2 * n_keep);
        reduced.leftCols(n_keep) = output_map.leftCols(n_keep);
        reduced.rightCols(n_keep) = output_map.rightCols(n_keep); // 这里的顺序在 Shift 后会回到左侧

        // 8. 二维 Shift (将 0,0 移至中心)
        fftShift2D(reduced);

        const int k_center = reduced.rows() / 2;
        const int notch_radius = std::max(3, (int)reduced.rows() / 32);
        for (int r = std::max(0, k_center - notch_radius);
             r <= std::min((int)reduced.rows() - 1, k_center + notch_radius);
             ++r) {
            const double x = std::abs(r - k_center) / static_cast<double>(notch_radius);
            const double weight = 0.02 + 0.98 * x;
            reduced.row(r) *= weight;
        }

        // 9. 计算幅度并转为 dB
        // eps 设置为 1e-6 是为了防止 log(0)，如果幅值依然极低，可以尝试归一化后计算
        // 门限处理：可选，让背景更干净
        // double max_val = magnitude.maxCoeff();
        // magnitude = magnitude.array().max(max_val * 1e-5);

        // 6. 计算结果后的后处理
        Eigen::MatrixXd result_db = (reduced.array().abs() + 1.0).log10();
        Eigen::VectorXd k_background = result_db.rowwise().mean();
        result_db.colwise() -= k_background;
        result_db.array() -= result_db.minCoeff();

        // 归一化显示：将最大值对齐到 10.0 (参考右图 Colorbar 范围)
        const double peak_db = result_db.maxCoeff();

        // 截断背景噪声：参考右图，将低于 0 或 -2.5 的部分压成深蓝色
        double floor_val = 0.0;
        result_db = result_db.array().max(floor_val);

        
        return result_db.transpose();
    }

private:
    int pad_k_, n_time_;
    double fs_;
    fftw_complex* in_, * out_;
    fftw_plan plan_;
    Eigen::VectorXd hanning_;

    void allocate_resources(int pk, int nt) {
        in_ = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pk * nt);
        out_ = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pk * nt);
        plan_ = fftw_plan_dft_2d(pk, nt, in_, out_, FFTW_FORWARD, FFTW_MEASURE);
    }

    void cleanup() {
        if (plan_) fftw_destroy_plan(plan_);
        if (in_) fftw_free(in_);
        if (out_) fftw_free(out_);
        plan_ = nullptr; in_ = nullptr; out_ = nullptr;
    }

    void reinitialize(int pk, int nt) {
        cleanup();
        pad_k_ = pk;
        n_time_ = nt;
        allocate_resources(pad_k_, n_time_);
        precompute_hanning(); // 时间窗长度变了，需要重算
    }

    void precompute_hanning() {
        hanning_.resize(n_time_);
        if (n_time_ <= 1) {
            hanning_.setOnes();
            return;
        }
        for (int i = 0; i < n_time_; ++i) {
            hanning_(i) = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (n_time_ - 1)));
        }
    }

    static int next_pow2(int v) {
        int n = 1;
        while (n < v) {
            n <<= 1;
        }
        return n;
    }

    void fftShift2D(Eigen::MatrixXcd& mat) {
        int r = (int)mat.rows(), c = (int)mat.cols();
        int r2 = r / 2, c2 = c / 2;

        // 块交换逻辑
        Eigen::MatrixXcd topLeft = mat.topLeftCorner(r2, c2);
        Eigen::MatrixXcd bottomRight = mat.bottomRightCorner(r - r2, c - c2);
        // ... (此处省略具体行列交换细节，使用 Eigen 的 block 交换确保鲁棒)
        // 简化的可靠实现：
        auto shift_cols = [](Eigen::MatrixXcd& m) {
            int mid = m.cols() / 2;
            Eigen::MatrixXcd left = m.leftCols(mid);
            m.leftCols(m.cols() - mid) = m.rightCols(m.cols() - mid).eval();
            m.rightCols(mid) = left.eval();
            };
        auto shift_rows = [](Eigen::MatrixXcd& m) {
            int mid = m.rows() / 2;
            Eigen::MatrixXcd top = m.topRows(mid);
            m.topRows(m.rows() - mid) = m.bottomRows(m.rows() - mid).eval();
            m.bottomRows(mid) = top.eval();
            };
        shift_rows(mat);
        shift_cols(mat);
    }
};



#endif // HDFSPECTRUMPROCESSOR_H
