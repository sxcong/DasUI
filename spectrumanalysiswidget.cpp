#include "spectrumanalysiswidget.h"
#include <QFile>
#include <QDataStream>
#include <QHBoxLayout>
#include <QMessageBox>
#include <algorithm>
#include <cstring>
#include <limits>

SpectrumAnalysisWidget::SpectrumAnalysisWidget(const QString& bandEnergyFile,
                                               const QString& spectrumFile,
                                               QWidget* parent)
    : QDialog(parent)
    , m_bandEnergyFile(bandEnergyFile)
    , m_spectrumFile(spectrumFile)
{
    setupUi();
    reload();
}

SpectrumAnalysisWidget::SpectrumAnalysisWidget(const QString& bandEnergyFile,
                                               const QString& spectrumFile,
                                               double sampleRateHz,
                                               QWidget* parent)
    : QDialog(parent)
    , m_bandEnergyFile(bandEnergyFile)
    , m_spectrumFile(spectrumFile)
    , m_sampleRateHz(sampleRateHz > 0.0 ? sampleRateHz : 2000.0)
{
    setupUi();
    reload();
}

void SpectrumAnalysisWidget::setSampleRate(double sampleRateHz)
{
    if (sampleRateHz > 0.0) {
        m_sampleRateHz = sampleRateHz;
    }
}

void SpectrumAnalysisWidget::setupUi()
{
    m_bandPlot = new QCustomPlot(this);
    m_spectrumPlot = new QCustomPlot(this);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    layout->addWidget(m_bandPlot, 1);
    layout->addWidget(m_spectrumPlot, 2);

    setupPlotStyle(m_bandPlot, "Band Energy");
    setupPlotStyle(m_spectrumPlot, "DAS Spectrum");

    // 将 Y 轴 (yAxis) 的方向反转
    m_bandPlot->yAxis->setRangeReversed(true);
    m_spectrumPlot->yAxis->setRangeReversed(true);

    // 如果需要反转 X 轴 (xAxis)
    // ui->customPlot->xAxis->setRangeReversed(true);
    // 将左轴移到右边，右轴移到左边
    // ui->customPlot->axisRect()->setupFullAxesBox();


    /*m_tooltip = new QCPItemText(m_spectrumPlot);
    m_tooltip->setPadding(QMargins(5, 5, 5, 5));
    m_tooltip->setBrush(QBrush(QColor(255, 255, 255, 200)));
    m_tooltip->setPen(QPen(Qt::black));
    m_tooltip->setVisible(false); // 初始隐藏
    m_spectrumPlot->addItem(m_tooltip);*/

    // 连接鼠标移动信号
    connect(m_spectrumPlot, &QCustomPlot::mouseMove, this, &SpectrumAnalysisWidget::showValueAtMouse);

}

void SpectrumAnalysisWidget::setupPlotStyle(QCustomPlot* plot, const QString& title)
{
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->setBackground(QBrush(QColor("#f7f7f7")));
    plot->axisRect()->setBackground(QColor("#ffffff"));
    plot->plotLayout()->insertRow(0);
    auto* titleElement = new QCPTextElement(plot, title, QFont("Microsoft YaHei", 12, QFont::Bold));
    plot->plotLayout()->addElement(0, 0, titleElement);
    plot->xAxis->setBasePen(QPen(Qt::black));
    plot->yAxis->setBasePen(QPen(Qt::black));
    plot->xAxis->setTickLabelColor(Qt::black);
    plot->yAxis->setTickLabelColor(Qt::black);
}

bool SpectrumAnalysisWidget::readBinaryMatrix(const QString& filePath, SpectrumBinaryMatrix& matrix) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    qint32 rows = 0;
    qint32 cols = 0;
    stream >> rows >> cols;
    if (rows <= 0 || cols <= 0) {
        return false;
    }

    const qint64 count = static_cast<qint64>(rows) * cols;
    if (count > std::numeric_limits<int>::max()) {
        return false;
    }

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data.resize(static_cast<int>(count));

    const qint64 byteCount = count * static_cast<qint64>(sizeof(double));
    const QByteArray payload = file.read(byteCount);
    if (payload.size() != byteCount) {
        return false;
    }

    std::memcpy(matrix.data.data(), payload.constData(), static_cast<std::size_t>(byteCount));
    return matrix.isValid();
}

void SpectrumAnalysisWidget::reload()
{
    SpectrumBinaryMatrix bandMatrix;
    SpectrumBinaryMatrix spectrumMatrix;
    if (!readBinaryMatrix(m_bandEnergyFile, bandMatrix)) {
        QMessageBox::warning(this, "Spectrum", "Failed to read band energy file.");
        return;
    }
    if (!readBinaryMatrix(m_spectrumFile, spectrumMatrix)) {
        QMessageBox::warning(this, "Spectrum", "Failed to read spectrum file.");
        return;
    }

    plotBandEnergy(bandMatrix);
    plotSpectrum(spectrumMatrix);
}

void SpectrumAnalysisWidget::plotBandEnergy(const SpectrumBinaryMatrix& matrix)
{
    m_bandPlot->clearPlottables();
    m_bandPlot->xAxis->setLabel("Energy");
    m_bandPlot->yAxis->setLabel("Depth index");

    const QStringList labels = {"0-1Hz", "0-10Hz", "10-100Hz", "100-500Hz", "500-1000Hz"};
    const QVector<QColor> colors = {
        QColor("#1f77b4"), QColor("#ff7f0e"), QColor("#2ca02c"),
        QColor("#d62728"), QColor("#9467bd")
    };

    QVector<double> depth(matrix.rows);
    for (int r = 0; r < matrix.rows; ++r) {
        depth[r] = r;
    }

    double xMax = 0.0;
    for (int c = 0; c < matrix.cols; ++c) {
        QVector<double> energy(matrix.rows);
        for (int r = 0; r < matrix.rows; ++r) {
            energy[r] = matrix.at(r, c);
            xMax = std::max(xMax, energy[r]);
        }

        auto* curve = new QCPCurve(m_bandPlot->xAxis, m_bandPlot->yAxis);
        curve->setData(depth, energy, depth, true);
        curve->setName(c < labels.size() ? labels[c] : QString("Band %1").arg(c + 1));
        curve->setPen(QPen(colors[c % colors.size()], 2));
    }

    m_bandPlot->legend->setVisible(true);
    m_bandPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 220)));
    m_bandPlot->xAxis->setRange(0.0, xMax > 0.0 ? xMax * 1.05 : 1.0);
    //m_bandPlot->yAxis->setRange(matrix.rows - 1, 0);

    m_bandPlot->yAxis->setRange(0, matrix.rows - 1);
    m_bandPlot->yAxis->setRangeReversed(true); // 显式开启轴反转

    m_bandPlot->replot();
}

void SpectrumAnalysisWidget::plotSpectrum(const SpectrumBinaryMatrix& matrix)
{
    m_spectrumPlot->clearPlottables();
    m_spectrumPlot->xAxis->setLabel("Frequency (Hz)");
    m_spectrumPlot->yAxis->setLabel("Depth index");

    m_colorMap = new QCPColorMap(m_spectrumPlot->xAxis, m_spectrumPlot->yAxis);
    m_colorMap->data()->setSize(matrix.cols, matrix.rows);
    m_colorMap->data()->setRange(QCPRange(0, m_sampleRateHz / 2.0), QCPRange(0, matrix.rows - 1));

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::lowest();
    for (int r = 0; r < matrix.rows; ++r) {
        for (int c = 0; c < matrix.cols; ++c) {
            const double value = matrix.at(r, c);
            m_colorMap->data()->setCell(c, r, value);
            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }
    }

    m_colorScale = new QCPColorScale(m_spectrumPlot);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->axis()->setLabel("Power");
    m_colorScale->setDataRange(QCPRange(minValue, maxValue));

    //m_colorScale->setDataRange(QCPRange(0, 10));

    m_spectrumPlot->plotLayout()->addElement(1, 1, m_colorScale);
    m_spectrumPlot->plotLayout()->setColumnStretchFactor(0, 1);
    m_spectrumPlot->plotLayout()->setColumnStretchFactor(1, 0.04);

    m_colorMap->setColorScale(m_colorScale);
    m_colorMap->setGradient(QCPColorGradient::gpJet);
    m_spectrumPlot->xAxis->setRange(0, m_sampleRateHz / 2.0);
    m_spectrumPlot->yAxis->setRange(matrix.rows - 1, 0);
    m_spectrumPlot->replot();
}

void SpectrumAnalysisWidget::showValueAtMouse(QMouseEvent* event)
{

    // 1. 获取鼠标在轴坐标系中的位置 (频率, 深度)
    double x = m_spectrumPlot->xAxis->pixelToCoord(event->pos().x());
    double y = m_spectrumPlot->yAxis->pixelToCoord(event->pos().y());

    // 2. 获取 ColorMap 的范围和尺寸
    double keyMin, keyMax, valueMin, valueMax;
    int keySize, valueSize;
   // m_colorMap->data()->getRange(&keyMin, &keyMax, &valueMin, &valueMax);

    keyMin = m_colorMap->data()->keyRange().lower;
    keyMax = m_colorMap->data()->keyRange().upper;

    valueMin = m_colorMap->data()->valueRange().lower;
    valueMax = m_colorMap->data()->valueRange().upper;

    keySize = m_colorMap->data()->keySize();
    valueSize = m_colorMap->data()->valueSize();

    // 3. 手动计算索引：索引 = (当前值 - 起始值) / 总跨度 * 总尺寸
    int col = static_cast<int>((x - keyMin) / (keyMax - keyMin) * keySize);
    int row = static_cast<int>((y - valueMin) / (valueMax - valueMin) * valueSize);


    // 4. 边界检查
    if (col >= 0 && col < keySize && row >= 0 && row < valueSize)
    {
        double energy = m_colorMap->data()->cell(col, row);

        QString info = QString("Freq: %1 Hz\nDepth: %2\nEnergy: %3")
                           .arg(x, 0, 'f', 1)
                           .arg(y, 0, 'f', 0)
                           .arg(energy, 0, 'f', 3);

       // qDebug()<<"showValueAtMouse"<<x<<y<<keySize<<keyMax<<keyMin<<row;
       // qDebug()<<"showValueAtMouse"<<x<<y<<valueSize<<valueMax<<valueMin<<col;
        m_spectrumPlot->setToolTip(info);
    }
    else
    {
        m_spectrumPlot->setToolTip("");
    }
}
