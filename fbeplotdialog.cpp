#include "fbeplotdialog.h"
#include "hd5reader.h"
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <QApplication>
#include <QTimer>
#include <fftw3.h>
#include <cmath>
#include <QDebug>

FbePlotDialog::FbePlotDialog(const QString& h5Path, QWidget *parent)
    : QDialog(parent)
    , m_h5Path(h5Path)
    , m_plot(nullptr)
    , m_colorMap(nullptr)
    , m_colorScale(nullptr)
{
    setWindowTitle("FBE - " + h5Path.section('/', -1));
    resize(1000, 600);

    m_plot = new QCustomPlot(this);
    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->setInteraction(QCP::iRangeZoom, true);
    m_plot->setBackground(QBrush(QColor("#000040")));
    m_plot->axisRect()->setBackground(QColor("#000060"));
    m_plot->xAxis->setLabel("Frequency Band");
    m_plot->yAxis->setLabel("Depth (channel)");
    m_plot->xAxis->setLabelColor(Qt::white);
    m_plot->yAxis->setLabelColor(Qt::white);
    m_plot->xAxis->setTickLabelColor(Qt::white);
    m_plot->yAxis->setTickLabelColor(Qt::white);
    m_plot->xAxis->setBasePen(QPen(Qt::white));
    m_plot->yAxis->setBasePen(QPen(Qt::white));

    m_loadingLabel = new QLabel("Calculating FBE, please wait...", m_plot);
    m_loadingLabel->setStyleSheet("color: #00AAFF; font-size: 20px; font-weight: bold; background: rgba(0,0,0,150); border-radius: 10px; padding: 20px;");
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingLabel->hide();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_plot);

    QTimer::singleShot(100, this, [this]() {
        m_loadingLabel->resize(this->size());
        m_loadingLabel->show();
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QElapsedTimer tt;
        tt.start();
        bool success = loadAndCompute(m_h5Path);
        qDebug() << "FBE loadAndCompute:" << tt.elapsed() << "ms";

        m_loadingLabel->hide();
        QApplication::restoreOverrideCursor();
    });
}

FbePlotDialog::~FbePlotDialog() {}

double FbePlotDialog::readSampleRate(const QString& h5Path)
{
    Hd5Reader reader;
    if (!reader.open(h5Path.toStdString()))
        return 2000.0;

    std::string datasetName = "/default";
    if (!reader.hasDataset(datasetName)) {
        auto datasets = reader.listDatasets();
        if (datasets.empty())
            return 2000.0;

        datasetName = datasets[0];
    }

   /* auto attrs = reader.readAttributes(datasetName);
    for (const auto& key : {"sample_rate", "SampleRate", "fs", "Fs", "sampling_rate", "SamplingRate"}) {
        auto it = attrs.find(key);
        if (it != attrs.end()) {
            double val = std::stod(it->second);
            if (val > 0) return val;
        }
    }*/
    return 2000.0;
}

void FbePlotDialog::computeFBE(const Eigen::MatrixXf& data, double sampleRate,
                                Eigen::MatrixXf& fbeMatrix,
                                QVector<FrequencyBand>& bands)
{
    int numTime = static_cast<int>(data.rows());   // time samples per channel
    int numDepth = static_cast<int>(data.cols());    // depth channels

    // Define frequency bands
    double nyquist = sampleRate / 2.0;
    bands.clear();
    bands.append({"0-5Hz", 0, 5});
    bands.append({"5-10Hz", 5, 10});
    bands.append({"10-20Hz", 10, 20});
    bands.append({"20-50Hz", 20, 50});
    bands.append({"50-100Hz", 50, 100});
    bands.append({"100-200Hz", 100, 200});
    bands.append({"200-500Hz", 200, 500});
    bands.append({"500-1000Hz", 500, qMin(1000.0, nyquist)});

    // Remove bands beyond Nyquist
    while (!bands.isEmpty() && bands.last().lowHz >= nyquist)
        bands.removeLast();
    if (bands.isEmpty()) {
        bands.append({"0-Nyquist", 0, nyquist});
    }

    int numBands = bands.size();
    fbeMatrix.resize(numDepth, numBands);
    fbeMatrix.setZero();

    int nFFT = numTime;
    int nOut = nFFT / 2 + 1;   // r2c output size

    // Precompute bin-to-band mapping
    QVector<int> binToBand(nOut, -1);
    for (int k = 0; k < nOut; ++k) {
        double freq = static_cast<double>(k) * sampleRate / nFFT;
        for (int b = 0; b < numBands; ++b) {
            if (freq >= bands[b].lowHz && freq < bands[b].highHz) {
                binToBand[k] = b;
                break;
            }
        }
    }

    // Allocate FFTW buffers
    float *in = static_cast<float*>(fftwf_malloc(sizeof(float) * nFFT));
    fftwf_complex *out = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * nOut));
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(nFFT, in, out, FFTW_ESTIMATE);

    // Process each depth channel
    for (int d = 0; d < numDepth; ++d) {
        // Copy channel data (column-major: each column is contiguous)
        const float *src = data.col(d).data();
        memcpy(in, src, sizeof(float) * nFFT);

        fftwf_execute(plan);

        // Accumulate energy per band
        for (int k = 0; k < nOut; ++k) {
            int b = binToBand[k];
            if (b < 0) continue;
            double energy = out[k][0] * out[k][0] + out[k][1] * out[k][1];
            fbeMatrix(d, b) += static_cast<float>(energy);
        }

        // Normalise each band by number of bins
        for (int b = 0; b < numBands; ++b) {
            int count = 0;
            for (int k = 0; k < nOut; ++k) {
                if (binToBand[k] == b) ++count;
            }
            if (count > 0)
                fbeMatrix(d, b) /= count;
        }
    }

    fftwf_destroy_plan(plan);
    fftwf_free(in);
    fftwf_free(out);
}

bool FbePlotDialog::loadAndCompute(const QString& h5Path)
{
    Hd5Reader reader;
    if (!reader.open(h5Path.toStdString()))
        return false;

    std::string datasetName = "/default";
    if (!reader.hasDataset(datasetName)) {
        auto datasets = reader.listDatasets();
        if (datasets.empty()) return false;
        datasetName = datasets[0];
    }

    hsize_t dims[2] = {0, 0};
    if (!reader.getDims(datasetName, dims))
        return false;

    Eigen::MatrixXf mat;
    if (!reader.readDataset(datasetName, mat))
        return false;

    double sampleRate = readSampleRate(h5Path);

    Eigen::MatrixXf fbeMatrix;
    QVector<FrequencyBand> bands;
    computeFBE(mat, sampleRate, fbeMatrix, bands);

    int numDepth = static_cast<int>(fbeMatrix.rows());
    int numBands = static_cast<int>(fbeMatrix.cols());

    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorMap->data()->setSize(numBands, numDepth);
    m_colorMap->data()->setRange(QCPRange(0, numBands), QCPRange(0, numDepth));

    m_colorScale = new QCPColorScale(m_plot);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->setDataRange(QCPRange(fbeMatrix.minCoeff(), fbeMatrix.maxCoeff()));
    m_colorScale->axis()->setLabelColor(Qt::white);
    m_colorScale->axis()->setTickLabelColor(Qt::white);
    m_colorScale->axis()->setBasePen(QPen(Qt::white));
    m_colorScale->setBarWidth(12);
    m_plot->plotLayout()->addElement(0, 1, m_colorScale);
    m_plot->plotLayout()->setColumnStretchFactor(0, 1);
    m_plot->plotLayout()->setColumnStretchFactor(1, 0.03);

    m_colorMap->setColorScale(m_colorScale);
    m_colorMap->setGradient(QCPColorGradient::gpJet);

    auto *mapData = m_colorMap->data();
    for (int d = 0; d < numDepth; ++d) {
        for (int b = 0; b < numBands; ++b) {
            mapData->setCell(b, d, fbeMatrix(d, b));
        }
    }

    // Custom x-axis ticker for frequency band labels
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (int b = 0; b < numBands; ++b) {
        textTicker->addTick(b + 0.5, bands[b].label);
    }
    m_plot->xAxis->setTicker(textTicker);
    m_plot->xAxis->setRange(0, numBands);
    m_plot->yAxis->setRange(0, numDepth);
    m_plot->yAxis->setRangeReversed(true);
    m_plot->replot();
    return true;
}
