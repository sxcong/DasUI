#include "hdfspectrumprocessor.h"
#include <algorithm> // Required for std::nth_element
#include <vector>    // Assuming 'window' is a std::vector
#include <QDebug>
#include <QElapsedTimer>
#include <vector>
#include "iir/Butterworth.h"
#include "processdownsample.h"


bool HDFSpectrumProcessor::process(const std::string& h5file,
                                   const std::string& outBin,
                                   int beginCol,
                                   int endCol)
{
    if(beginCol < 0)
        beginCol = 0;
    if(beginCol > endCol)
        return false;


    QElapsedTimer timer;
    timer.start();
    if(!readHDF(h5file))
        return false;
    qint64 t1 = timer.restart();
    qDebug()<<"readHDF timer.elapsed()"<<t1;

    if (endCol >= m_cols)
        endCol = m_cols - 1;
    if (beginCol >= m_cols)
        return false;

    beginCol_ = beginCol;
    currentCols_ = endCol - beginCol + 1;


    rawData_ = Eigen::Map<RowMajorMatrix>(
        hdfBuffer_.data(),
        m_rows,
        m_cols);

    //前3382列数据有效
    if (rawData_.cols() > 3382) {
        //rawData.rightCols(rawData.cols() - 3382).setZero();
        rawData_.rightCols(rawData_.cols() - 3382).setConstant(std::numeric_limits<float>::quiet_NaN());
    }

    // 1 down sample
    downSample();//ok

    std::string fileName = outBin + ".down";
    qDebug() << downData_.rows() << downData_.cols();
    saveBinary(fileName, downData_);//

    //2 lfdas, 需要使用降采样后的数据
    processLFDAS();
    std::string fileName4 = outBin + ".lfdas";
    saveBinary(fileName4, lsdas_);


  
    //3 Spectrum
    calcSpectrum();//ok
    saveBinary(outBin + ".spectrum_power", spectrum_);

    std::vector<std::string> fileList;
    fileList.push_back(outBin + ".spectrum_power");
    mergeSpectrumPowerToDb(fileList, outBin + ".spectrum_db");


    toDbSpectrum();
    normalizeRelativeDb();
    qint64 t2 = timer.restart();
    qDebug()<<"calcSpectrum timer.elapsed()"<<t2;
    std::string fileName2 = outBin + ".spectrum";
    saveBinary(fileName2, spectrumDb_);//
    qint64 t3 = timer.restart();
    qDebug()<<"saveBinary timer.elapsed()"<<t3;


    //4 bands
    std::vector<std::pair<int,int>> bands = {
        {0.0, 1.0},
        {0.0, 10.0},
        {10.0, 100.0},
        {100.0, 500.0},
        {500.0, 1000.0}
    };
    calcBandEnergy(bands);
    std::string fileName3 = outBin + ".bands";
    saveBinary(fileName3, bandEnergy_);//

    //5 fk
  
   
  /*  int centerDepth = 300;
    int windowSize = 256; // 空间窗口太小会导致波数分辨率不足，X型特征不稳定
    computeFkStacked(0, centerDepth - windowSize / 2, windowSize);

    std::string fileName5 = outBin + ".fk";
    saveBinary(fileName5, fkData_);//
    //saveFkViewBinary(outBin + ".fkview", fkData_, SAMPLE_RATE, 1.0f, 90.0f, 0.05f);
    */

    int start_ch = beginCol_;
    int end_ch = beginCol_ + currentCols_ - 1;
    int n_ch = end_ch - start_ch + 1;
    int pad_to = n_ch;

    FKProcessor fk_engine(pad_to, 2000, 2000);

    Eigen::MatrixXd fk_result;
    int fk_count = 0;
    for (int sec = 0; sec < SEC_COUNT; ++sec) {
        const int row_start = sec * SAMPLE_RATE;
        if (row_start + SAMPLE_RATE > rawData_.rows()) {
            break;
        }

        auto segment = rawData_.block(row_start, start_ch, SAMPLE_RATE, n_ch).transpose();
        Eigen::MatrixXd segment_double = segment.cast<double>();
        Eigen::MatrixXd one_fk = fk_engine.process_segment(segment_double, 400.0f);
        if (one_fk.size() == 0) {
            continue;
        }

        if (fk_count == 0) {
            fk_result = one_fk;
        }
        else {
            fk_result = fk_result.cwiseMax(one_fk);
        }
        ++fk_count;
       // break;
    }

    if (fk_count > 0 && fk_result.size() > 0) {
        Eigen::RowVectorXd k_background = fk_result.colwise().mean();
        fk_result.rowwise() -= 0.8 * k_background;

        const int k_center = fk_result.cols() / 2;
        const int notch_radius = std::max(3, (int)(fk_result.cols() / 24));
        for (int c = std::max(0, k_center - notch_radius);
             c <= std::min((int)fk_result.cols() - 1, k_center + notch_radius);
             ++c) {
            const double x = std::abs(c - k_center) / static_cast<double>(notch_radius);
            const double weight = 0.05 + 0.95 * x;
            fk_result.col(c) *= weight;
        }

        fk_result.array() -= fk_result.minCoeff();
    }

    if (fk_count > 0 && fk_result.size() > 0) {
        qDebug() << "fk stacked" << fk_count << fk_result.rows() << fk_result.cols()
                 << "min" << fk_result.minCoeff()
                 << "max" << fk_result.maxCoeff();

        Eigen::MatrixXf fk_result_f = fk_result.cast<float>();
        saveBinary(outBin + ".fk2", fk_result_f);//
    } else {
        qDebug() << "fk stacked" << fk_count << 0 << 0;
    }



    //6 fbe
    calcFBE();
    std::string fileName6 = outBin + ".fbe";
    saveBinary(fileName6, fbe_);//

    return true;
}

void HDFSpectrumProcessor::initWindow()
{
    window_.resize(FFT_SIZE);
    for(int i = 0; i < FFT_SIZE; ++i)
    {
        window_(i) = 0.5f - 0.5f * cos(2.0 * M_PI * i / (FFT_SIZE - 1));
    }
}

void HDFSpectrumProcessor::initFFTW()
{
    fftInput_ = static_cast<float*>(fftwf_malloc(sizeof(float) * FFT_SIZE));
    fftOutput_ = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * FFT_BINS));
    fbeInput_ = static_cast<float*>(fftwf_malloc(sizeof(float) * FBE_FFT_SIZE));
    fbeOutput_ = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * FBE_FFT_BINS));

    plan_ = fftwf_plan_dft_r2c_1d(
        FFT_SIZE, fftInput_, fftOutput_, FFTW_MEASURE);
    fbePlan_ = fftwf_plan_dft_r2c_1d(
        FBE_FFT_SIZE, fbeInput_, fbeOutput_, FFTW_MEASURE);
}

void HDFSpectrumProcessor::releaseFFTW()
{
    if(plan_) fftwf_destroy_plan(plan_);
    if(fbePlan_) fftwf_destroy_plan(fbePlan_);
    if(fftInput_) fftwf_free(fftInput_);
    if(fbeInput_) fftwf_free(fbeInput_);
    if(fftOutput_) fftwf_free(fftOutput_);
    if(fbeOutput_) fftwf_free(fbeOutput_);
    fftwf_cleanup();
}

void HDFSpectrumProcessor::removeMean(Vector& x)
{
    x.array() -= x.mean();
}

void HDFSpectrumProcessor::applyWindow(Vector& x)
{
    x.array() *= window_.array();
}

void HDFSpectrumProcessor::calcSpectrum()
{
    spectrum_.resize(currentCols_, FFT_BINS);
    spectrum_.setZero();
    processedSecCount_ = std::min(SEC_COUNT, static_cast<int>(rawData_.rows() / FFT_SIZE));
    if (processedSecCount_ <= 0 || currentCols_ <= 0) {
        return;
    }

    const float windowEnergy = std::max(window_.squaredNorm(), 1e-12f);

    for(int s = 0; s < processedSecCount_; ++s)
    {
        const int r0 = s * FFT_SIZE;

        for(int ch = 0; ch < currentCols_; ++ch)
        {
            const int rawCol = beginCol_ + ch;
            for(int i = 0; i < FFT_SIZE; ++i)
            {
                if (rawCol >= 3382) {
                    workBuffer_(i) = 0.0f;
                } else {
                    const float sample = rawData_(r0 + i, rawCol);
                    workBuffer_(i) = std::isfinite(sample) ? sample : 0.0f;
                }
            }

            if (rawCol < 3382) {
                removeMean(workBuffer_);
                applyWindow(workBuffer_);
            }

            for(int i = 0; i < FFT_SIZE; ++i)
            {
                fftInput_[i] = workBuffer_(i);
            }

            fftwf_execute(plan_);

            for(int k = 0; k < FFT_BINS; ++k)
            {
                const float re = fftOutput_[k][0];
                const float im = fftOutput_[k][1];
                //spectrum_(ch, k) += re * re + im * im;
                //幅值谱
#if 0
                const float amp = std::sqrt(re * re + im * im) * scale;
                spectrum_(ch, k) += amp;
#else
                //功率谱
                //const float scale = 1.0f / static_cast<float>(FFT_SIZE * FFT_SIZE);
                //const float power = (re * re + im * im) * scale;
                //spectrum_(ch, k) += power;
                const float current_power = (re * re + im * im) / windowEnergy;
                //spectrum_(ch, k) = std::max(spectrum_(ch, k), current_power);
                spectrum_(ch, k) += current_power;
#endif
            }
        }
    }

    // Average the accumulated linear power spectrum. Keep this output in
    // linear scale so multiple files can be merged before converting to dB.
    float inv_count = 1.0f / static_cast<float>(processedSecCount_);
    spectrum_.array() *= inv_count;
}

void HDFSpectrumProcessor::toDbSpectrum()
{
    if (spectrum_.size() <= 0) {
        spectrumDb_.resize(0, 0);
        return;
    }

    spectrumDb_ = (spectrum_.array() + 1e-9f).log10().matrix() * 10.0f;
}

void HDFSpectrumProcessor::normalizeRelativeDb()
{
    if (spectrumDb_.size() <= 0) {
        return;
    }

    const float maxDb = spectrumDb_.maxCoeff();
    spectrumDb_.array() -= maxDb;
}


void HDFSpectrumProcessor::normalizeDB(float range)
{
    //spectrum_ = 10.0f * (spectrum_.array() + 1e-12f).log10();
    //float mx = spectrum_.maxCoeff();
    //spectrum_.array() -= mx;
    //spectrum_ = spectrum_.cwiseMax(-range);
}

void HDFSpectrumProcessor::downSample()
{
    ProcessDownSample downSample;
    int sr = 2000;
    downSample.downSampleMultistage(rawData_, downData_, 10, 20, sr);

    return;
 /*   downData_.resize(SEC_COUNT, rawData_.cols());//60 4001
    //SEC_COUNT = 60
    //SAMPLE_RATE = 2000
    //rawData_ (120000 x 4001 )

#pragma omp parallel for
    for (int i = 0; i < SEC_COUNT; i++) {
        int r0 = i * SAMPLE_RATE;
        downData_.row(i) = rawData_.block(r0, 0, SAMPLE_RATE, rawData_.cols())
                               .colwise()
                               .mean()
                               .cast<float>();
    }*/
}


void HDFSpectrumProcessor::calcBandEnergy(const std::vector<std::pair<int,int>>& bands)
{
    bandEnergy_.resize(currentCols_, static_cast<int>(bands.size()));
    bandEnergy_.setZero();

    for(size_t b = 0; b < bands.size(); ++b)
    {
        int f1 = std::clamp(bands[b].first, 0, FFT_BINS - 1);
        int f2 = std::clamp(bands[b].second, 0, FFT_BINS - 1);

        if(f1 > f2)
            continue;

        int n = f2 - f1 + 1;
        auto blk = spectrum_.block(0, f1, currentCols_, n);
        //bandEnergy_.col(b) = blk.rowwise().sum(); 
        bandEnergy_.col(b) = blk.rowwise().mean();//如果用sum()，数值非常高，左右图会不匹配
    }
}

void HDFSpectrumProcessor::calcFBE(int lowHz, int highHz)
{
    lowHz = std::clamp(lowHz, 0, FFT_BINS - 1);
    highHz = std::clamp(highHz, 0, FFT_BINS - 1);

    if (lowHz > highHz)
        std::swap(lowHz, highHz);

    const int n = highHz - lowHz + 1;

    if (processedSecCount_ <= 0) {
        processedSecCount_ = std::min(SEC_COUNT, static_cast<int>(rawData_.rows() / FFT_SIZE));
    }

    fbe_.resize(processedSecCount_, currentCols_);
    fbe_.setZero();
    const float windowEnergy = std::max(window_.squaredNorm(), 1e-12f);

    for (int s = 0; s < processedSecCount_; ++s)
    {
        const int r0 = s * FFT_SIZE;

        for (int ch = 0; ch < currentCols_; ++ch)
        {
            const int rawCol = beginCol_ + ch;
            for (int i = 0; i < FFT_SIZE; ++i)
            {
                if (rawCol >= 3382) {
                    workBuffer_(i) = 0.0f;
                } else {
                    const float sample = rawData_(r0 + i, rawCol);
                    workBuffer_(i) = std::isfinite(sample) ? sample : 0.0f;
                }
            }

            if (rawCol < 3382) {
                removeMean(workBuffer_);
                applyWindow(workBuffer_);
            }

            for (int i = 0; i < FFT_SIZE; ++i)
            {
                fftInput_[i] = workBuffer_(i);
            }

            fftwf_execute(plan_);

            float energy = 0.0f;
            for (int k = lowHz; k <= highHz; ++k)
            {
                const float re = fftOutput_[k][0];
                const float im = fftOutput_[k][1];
                energy += (re * re + im * im) / windowEnergy;
            }
            fbe_(s, ch) = energy / static_cast<float>(n);
        }
    }
}

void HDFSpectrumProcessor::processLFDAS()
{
    LFDASConfig lfdas_cfg;
    lfdas_cfg.raw_sr = 2000;
    lfdas_cfg.target_sr = 1;      // 强制 1s 采样
    lfdas_cfg.low_cutoff = 0.0;   // 0-0.1Hz 仅需低通
    lfdas_cfg.high_cutoff = 0.05; // 目标拐角频率

    Matrix selectedRaw = rawData_.middleCols(beginCol_, currentCols_);
    m_processLFDAS.processLF_DAS_Final(selectedRaw, lsdas_, lfdas_cfg);

    //如果多个文件，只能在按时间维度进行拼接 (Concatenation)，不能数据计算合并，比如平均值。
}


bool HDFSpectrumProcessor::readHDF(const std::string& file)
{
    hid_t f = H5Fopen(file.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if(f < 0) return false;

    hid_t d = H5Dopen2(f, "/default", H5P_DEFAULT);
    if(d < 0)
    {
        H5Fclose(f);
        return false;
    }

    hid_t space = H5Dget_space(d);
    hsize_t dims[2];
    H5Sget_simple_extent_dims(space, dims, nullptr);
    m_rows = static_cast<int>(dims[0]);    // 120000
    m_cols = static_cast<int>(dims[1]);    // 4001
    H5Sclose(space);


    hdfBuffer_.resize(m_rows * m_cols);

    herr_t r = H5Dread(d, H5T_NATIVE_FLOAT,
                       H5S_ALL, H5S_ALL,
                       H5P_DEFAULT,
                       hdfBuffer_.data());


    H5Dclose(d);
    H5Fclose(f);

    return r >= 0;
}
