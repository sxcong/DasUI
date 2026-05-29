#include "spectrumdbwidget.h"

#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

SpectrumDbWidget::SpectrumDbWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    setupPlot();
    connect(m_plot, &QCustomPlot::mousePress, this, &SpectrumDbWidget::MyMouseMoveEvent);
}

void SpectrumDbWidget::setupUi()
{
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* toolbar = new QWidget(this);
    toolbar->setObjectName("spectrumDbToolbar");
    toolbar->setFixedHeight(38);
    toolbar->setStyleSheet(R"(
        QWidget#spectrumDbToolbar { background: #f6f7f9; border-bottom: 1px solid #d7dce2; }
        QToolButton { color: #263238; background: #ffffff; border: 1px solid #c9d0d8; border-radius: 4px; padding: 4px 10px; min-width: 56px; }
        QToolButton:hover { border-color: #4385c5; background: #eef6ff; }
        QLabel { color: #53606b; }
    )");

    auto* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(8, 4, 8, 4);
    toolbarLayout->setSpacing(8);

    auto* openButton = new QToolButton(toolbar);
    openButton->setText("打开");
    openButton->setToolTip("打开一个或多个 spectrum_db 二进制文件");
    connect(openButton, &QToolButton::clicked, this, &SpectrumDbWidget::openFiles);

    auto* previousButton = new QToolButton(toolbar);
    previousButton->setText("上一个");
    previousButton->setToolTip("显示上一个频谱文件");
    connect(previousButton, &QToolButton::clicked, this, &SpectrumDbWidget::showPreviousFile);

    auto* nextButton = new QToolButton(toolbar);
    nextButton->setText("下一个");
    nextButton->setToolTip("显示下一个频谱文件");
    connect(nextButton, &QToolButton::clicked, this, &SpectrumDbWidget::showNextFile);

    auto* reloadButton = new QToolButton(toolbar);
    reloadButton->setText("刷新");
    reloadButton->setToolTip("重新加载当前频谱文件");
    connect(reloadButton, &QToolButton::clicked, this, &SpectrumDbWidget::reloadCurrentFile);

    m_statusLabel = new QLabel("未加载文件", toolbar);
    m_statusLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    toolbarLayout->addWidget(openButton);
    toolbarLayout->addWidget(previousButton);
    toolbarLayout->addWidget(nextButton);
    toolbarLayout->addWidget(reloadButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_statusLabel);

    m_plot = new QCustomPlot(this);
    rootLayout->addWidget(toolbar);
    rootLayout->addWidget(m_plot, 1);
}

void SpectrumDbWidget::setupPlot()
{
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    m_plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    m_plot->setBackground(QBrush(Qt::white));
    m_plot->axisRect()->setBackground(QBrush(Qt::white));

    m_plot->plotLayout()->insertRow(0);
    m_title = new QCPTextElement(m_plot, "频谱图", QFont("Microsoft YaHei", 13, QFont::Bold));
    m_plot->plotLayout()->addElement(0, 0, m_title);

    m_plot->xAxis->setLabel("频率(Hz)");
    m_plot->yAxis->setLabel("深度(m)");
    m_plot->yAxis->setRangeReversed(true);
    m_plot->xAxis->ticker()->setTickCount(8);
    m_plot->yAxis->ticker()->setTickCount(12);

    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorMap->setGradient(QCPColorGradient::gpJet);
    m_colorMap->setInterpolate(true);

    m_colorScale = new QCPColorScale(m_plot);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->axis()->setLabel("功率(dB)");
    m_colorScale->setBarWidth(22);
    m_plot->plotLayout()->addElement(1, 1, m_colorScale);
    m_plot->plotLayout()->setColumnStretchFactor(0, 1);
    m_plot->plotLayout()->setColumnStretchFactor(1, 0.05);
    m_colorMap->setColorScale(m_colorScale);
}

void SpectrumDbWidget::MyMouseMoveEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_colorMap || !m_colorMap->data()) {
        return;
    }

    const double frequency = m_plot->xAxis->pixelToCoord(event->pos().x());
    const double depth = m_plot->yAxis->pixelToCoord(event->pos().y());
    const QCPRange xRange = m_colorMap->data()->keyRange();
    const QCPRange yRange = m_colorMap->data()->valueRange();
    if (!xRange.contains(frequency) || !yRange.contains(depth)) {
        QToolTip::hideText();
        return;
    }

    const double value = m_colorMap->data()->data(frequency, depth);
    const QString info = QString("频率: %1 Hz\n深度: %2 m\n功率: %3 dB")
                             .arg(frequency, 0, 'f', 2)
                             .arg(depth, 0, 'f', 2)
                             .arg(value, 0, 'g', 6);
    QToolTip::showText(m_plot->mapToGlobal(event->pos()), info, m_plot);
}

void SpectrumDbWidget::setFiles(const QStringList& files)
{
    m_files = files;
    m_files.removeDuplicates();
    m_currentIndex = m_files.isEmpty() ? -1 : 0;
    displayFile(m_currentIndex);
}

void SpectrumDbWidget::setSampleRate(double sampleRateHz)
{
    if (sampleRateHz > 0.0) {
        m_sampleRateHz = sampleRateHz;
        displayFile(m_currentIndex);
    }
}

void SpectrumDbWidget::openFiles()
{
    const QString startDir = m_files.isEmpty() ? QDir::homePath() : QFileInfo(m_files.first()).absolutePath();
    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择 spectrum_db 二进制文件",
        startDir,
        "Spectrum DB (*.spectrum_db);;Binary Files (*.bin);;All Files (*)");
    if (!files.isEmpty()) {
        setFiles(files);
    }
}

void SpectrumDbWidget::showPreviousFile()
{
    if (m_files.isEmpty()) {
        return;
    }
    m_currentIndex = (m_currentIndex <= 0) ? m_files.size() - 1 : m_currentIndex - 1;
    displayFile(m_currentIndex);
}

void SpectrumDbWidget::showNextFile()
{
    if (m_files.isEmpty()) {
        return;
    }
    m_currentIndex = (m_currentIndex + 1) % m_files.size();
    displayFile(m_currentIndex);
}

void SpectrumDbWidget::reloadCurrentFile()
{
    displayFile(m_currentIndex);
}

bool SpectrumDbWidget::readBinaryMatrix(const QString& filePath, BinaryMatrix& matrix) const
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

void SpectrumDbWidget::displayFile(int index)
{
    if (index < 0 || index >= m_files.size()) {
        m_statusLabel->setText("未加载文件");
        return;
    }

    BinaryMatrix matrix;
    if (!readBinaryMatrix(m_files[index], matrix)) {
        m_statusLabel->setText(QString("读取失败: %1").arg(QFileInfo(m_files[index]).fileName()));
        return;
    }

    displayMatrix(m_files[index], matrix);
}

void SpectrumDbWidget::displayMatrix(const QString& filePath, const BinaryMatrix& matrix)
{
    m_colorMap->data()->setSize(matrix.cols, matrix.rows);
    m_colorMap->data()->setRange(QCPRange(0.0, m_sampleRateHz / 2.0), QCPRange(0.0, matrix.rows - 1.0));

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::lowest();
    for (int r = 0; r < matrix.rows; ++r) {
        for (int c = 0; c < matrix.cols; ++c) {
            const double value = matrix.at(r, c);
            if (!std::isfinite(value)) {
                continue;
            }
            m_colorMap->data()->setCell(c, r, value);
            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }
    }

    if (minValue > maxValue) {
        minValue = 0.0;
        maxValue = 1.0;
    } else if (qFuzzyCompare(minValue, maxValue)) {
        maxValue = minValue + 1.0;
    }

    m_colorScale->setDataRange(QCPRange(minValue, maxValue));
    m_plot->xAxis->setRange(0.0, m_sampleRateHz / 2.0);
    m_plot->yAxis->setRange(0.0, matrix.rows - 1.0);
    m_title->setText(QString("频谱图 | %1").arg(QFileInfo(filePath).fileName()));
    updateStatus();
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}

void SpectrumDbWidget::updateStatus()
{
    if (m_files.isEmpty() || m_currentIndex < 0) {
        m_statusLabel->setText("未加载文件");
        return;
    }

    m_statusLabel->setText(QString("%1 / %2  %3")
                               .arg(m_currentIndex + 1)
                               .arg(m_files.size())
                               .arg(QFileInfo(m_files[m_currentIndex]).fileName()));
}
