#include "waterfalldialog.h"
#include "hd5reader.h"
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <QApplication>
#include <QTimer>

WaterfallDialog::WaterfallDialog(const QString& h5Path, QWidget *parent)
    : QDialog(parent)
    , m_h5Path(h5Path)
    , m_plot(nullptr)
    , m_colorMap(nullptr)
    , m_colorScale(nullptr)
{
    qDebug()<<"WaterfallDialog"<<h5Path;
    setWindowTitle("Waterfall - " + h5Path.section('/', -1));
    resize(1000, 600);

    m_plot = new QCustomPlot(this);
    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->setInteraction(QCP::iRangeZoom, true);
    m_plot->setBackground(QBrush(QColor("#000040"))); // Dark blue for overall plot background
    m_plot->axisRect()->setBackground(QColor("#000060")); // Slightly lighter blue for axis rect background
    m_plot->xAxis->setLabel("Time (s)");
    m_plot->yAxis->setLabel("Depth (channel)");
    m_plot->xAxis->setLabelColor(Qt::white);
    m_plot->yAxis->setLabelColor(Qt::white);
    m_plot->xAxis->setTickLabelColor(Qt::white);
    m_plot->yAxis->setTickLabelColor(Qt::white);
    m_plot->xAxis->setBasePen(QPen(Qt::white));
    m_plot->yAxis->setBasePen(QPen(Qt::white));

    // 创建加载提示动画层
    m_loadingLabel = new QLabel("正在加载大数据，请稍候...", m_plot);
    m_loadingLabel->setStyleSheet("color: #00AAFF; font-size: 20px; font-weight: bold; background: rgba(0,0,0,150); border-radius: 10px; padding: 20px;");
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingLabel->hide(); // 初始隐藏，等窗口显示后再弹出

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_plot);

    // 关键优化：先让窗口弹出，延迟 100ms 再加载数据
    QTimer::singleShot(100, this, [this]() {
        m_loadingLabel->resize(this->size());
        m_loadingLabel->show();
        QApplication::setOverrideCursor(Qt::WaitCursor); // 鼠标变忙碌
        
        QElapsedTimer tt;
        tt.start();
        bool success = loadAndDisplay(m_h5Path);
        qDebug() << "loadAndDisplay 实际耗时:" << tt.elapsed() << "ms";

        m_loadingLabel->hide();
        QApplication::restoreOverrideCursor(); // 恢复光标
    });
}

WaterfallDialog::~WaterfallDialog() {}

void WaterfallDialog::downsample(const Eigen::MatrixXf& src, Eigen::MatrixXf& dst, int factor)
{
    int srcRows = static_cast<int>(src.rows());
    int cols = static_cast<int>(src.cols());
    int dstRows = srcRows / factor;
    dst.resize(dstRows, cols);
    for (int s = 0; s < dstRows; ++s) {
        dst.row(s) = src.block(s * factor, 0, factor, cols).colwise().mean();
    }
}

bool WaterfallDialog::loadAndDisplay(const QString& h5Path)
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

    int sampleRate = 2000;
    Eigen::MatrixXf downMat;
    downsample(mat, downMat, sampleRate);

    int timePts = static_cast<int>(downMat.rows());
    int depthCh = static_cast<int>(downMat.cols());

    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorMap->data()->setSize(timePts, depthCh);
    m_colorMap->data()->setRange(QCPRange(0, timePts), QCPRange(0, depthCh));

    m_colorScale = new QCPColorScale(m_plot);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->setDataRange(QCPRange(downMat.minCoeff(), downMat.maxCoeff()));
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
    for (int t = 0; t < timePts; ++t) {
        for (int d = 0; d < depthCh; ++d) {
            mapData->setCell(t, d, downMat(t, d));
        }
    }

    m_plot->xAxis->setRange(0, timePts);
    m_plot->yAxis->setRange(0, depthCh);
    m_plot->yAxis->setRangeReversed(true);
    m_plot->replot();
    return true;
}
