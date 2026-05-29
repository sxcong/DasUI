#include "lfdaswidget.h"

#include <QApplication>
#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QRegularExpression>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <utility>


LfDasWidget::LfDasWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    setupPlot();

    connect(m_plot, &QCustomPlot::mousePress, this, &LfDasWidget::MyMouseMoveEvent);

    m_rangeLoadTimer.setSingleShot(true);
    m_rangeLoadTimer.setInterval(120);
    connect(&m_rangeLoadTimer, &QTimer::timeout, this, [this]() {
        if (m_segments.isEmpty()) {
            return;
        }

        const QCPRange range = paddedRange(m_plot->xAxis->range());
        const bool changed = ensureDataForRange(range.lower, range.upper);
        refreshMapForRange(range.lower, range.upper);
        if (changed) {
            m_plot->replot(QCustomPlot::rpQueuedReplot);
        }
    });
}

void LfDasWidget::setupUi()
{
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* toolbar = new QWidget(this);
    toolbar->setObjectName("lfDasToolbar");
    toolbar->setFixedHeight(38);
    toolbar->setStyleSheet(R"(
        QWidget#lfDasToolbar {
            background: #f6f7f9;
            border-bottom: 1px solid #d7dce2;
        }
        QToolButton {
            color: #263238;
            background: #ffffff;
            border: 1px solid #c9d0d8;
            border-radius: 4px;
            padding: 4px 10px;
            min-width: 56px;
        }
        QToolButton:hover {
            border-color: #4385c5;
            background: #eef6ff;
        }
        QLabel {
            color: #53606b;
        }
    )");

    auto* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(8, 4, 8, 4);
    toolbarLayout->setSpacing(8);

    auto* openButton = new QToolButton(toolbar);
    openButton->setText("打开");
    openButton->setToolTip("打开一个或多个 LF-DAS 二进制文件");
    connect(openButton, &QToolButton::clicked, this, &LfDasWidget::openFiles);

    auto* reloadButton = new QToolButton(toolbar);
    reloadButton->setText("刷新");
    reloadButton->setToolTip("重新加载当前显示范围内的数据");
    connect(reloadButton, &QToolButton::clicked, this, &LfDasWidget::reloadLoadedFiles);

    auto* fitButton = new QToolButton(toolbar);
    fitButton->setText("全览");
    fitButton->setToolTip("显示全部文件时间范围");
    connect(fitButton, &QToolButton::clicked, this, &LfDasWidget::fitToAll);

    m_statusLabel = new QLabel("未加载文件", toolbar);
    m_statusLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    toolbarLayout->addWidget(openButton);
    toolbarLayout->addWidget(reloadButton);
    toolbarLayout->addWidget(fitButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_statusLabel);

    m_plot = new QCustomPlot(this);

    m_loadingLabel = new QLabel("正在加载 LF-DAS 数据...", m_plot);
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingLabel->setStyleSheet(
        "color: #ffffff; font-size: 18px; font-weight: bold;"
        "background: rgba(20, 24, 28, 170); border-radius: 6px; padding: 16px;");
    m_loadingLabel->hide();

    rootLayout->addWidget(toolbar);
    rootLayout->addWidget(m_plot, 1);
}

void LfDasWidget::setupPlot()
{
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    m_plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    m_plot->setBackground(QBrush(Qt::white));
    m_plot->axisRect()->setBackground(QBrush(Qt::white));

    m_plot->plotLayout()->insertRow(0);
    m_title = new QCPTextElement(m_plot, "DAS 瀑布图", QFont("Microsoft YaHei", 13, QFont::Bold));
    m_plot->plotLayout()->addElement(0, 0, m_title);

    m_plot->xAxis->setLabel("时间");
    m_plot->yAxis->setLabel("深度(m)");
    m_plot->xAxis->setTickLabelRotation(-35);
    m_plot->xAxis->setSubTicks(false);
    m_plot->yAxis->setRangeReversed(true);

    QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
    ticker->setDateTimeFormat("HH:mm:ss");
    ticker->setTickCount(6);
    m_plot->xAxis->setTicker(ticker);
    m_plot->yAxis->ticker()->setTickCount(12);

    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorMap->setGradient(QCPColorGradient::gpJet);
    m_colorMap->setInterpolate(true);

    m_colorScale = new QCPColorScale(m_plot);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorScale->axis()->setLabel("能量");
    m_colorScale->setBarWidth(22);
    m_plot->plotLayout()->addElement(1, 1, m_colorScale);
    m_plot->plotLayout()->setColumnStretchFactor(0, 1);
    m_plot->plotLayout()->setColumnStretchFactor(1, 0.05);
    m_colorMap->setColorScale(m_colorScale);

    connect(m_plot->xAxis, QOverload<const QCPRange&>::of(&QCPAxis::rangeChanged),
            this, [this](const QCPRange&) {
                if (!m_internalRangeChange) {
                    scheduleRangeLoad();
                }
            });
}

void LfDasWidget::MyMouseMoveEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_colorMap || !m_colorMap->data()) {
        return;
    }

    const double x = m_plot->xAxis->pixelToCoord(event->pos().x());
    const double depth = m_plot->yAxis->pixelToCoord(event->pos().y());
    const QCPRange xRange = m_colorMap->data()->keyRange();
    const QCPRange yRange = m_colorMap->data()->valueRange();
    if (!xRange.contains(x) || !yRange.contains(depth)) {
        QToolTip::hideText();
        return;
    }

    const double value = m_colorMap->data()->data(x, depth);
    const QString timeText = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(x * 1000.0))
                                 .toString("yyyy-MM-dd HH:mm:ss.zzz");
    const QString info = QString("时间: %1\n深度: %2 m\n能量: %3")
                             .arg(timeText)
                             .arg(depth, 0, 'f', 2)
                             .arg(value, 0, 'g', 6);
    QToolTip::showText(m_plot->mapToGlobal(event->pos()), info, m_plot);
}

void LfDasWidget::setFiles(const QStringList& files)
{
    m_files = files;
    m_files.removeDuplicates();
    rebuildSegments();
}

void LfDasWidget::addFiles(const QStringList& files)
{
    m_files.append(files);
    m_files.removeDuplicates();
    rebuildSegments();
}

void LfDasWidget::setSampleIntervalSeconds(double seconds)
{
    if (seconds > 0.0) {
        m_sampleIntervalSeconds = seconds;
        rebuildSegments();
    }
}

void LfDasWidget::setDepthIntervalMeters(double meters)
{
    if (meters > 0.0) {
        m_depthIntervalMeters = meters;
        refreshMapForRange(m_plot->xAxis->range().lower, m_plot->xAxis->range().upper);
        m_plot->replot(QCustomPlot::rpQueuedReplot);
    }
}

void LfDasWidget::setVisibleSpanSeconds(double seconds)
{
    if (seconds > 0.0) {
        m_visibleSpanSeconds = seconds;
    }
}

void LfDasWidget::openFiles()
{
    const QString startDir = m_files.isEmpty() ? QDir::homePath() : QFileInfo(m_files.first()).absolutePath();
    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择 LF-DAS 二进制文件",
        startDir,
        "LF-DAS Binary (*.lfdas *.bin);;All Files (*)");
    if (!files.isEmpty()) {
        setFiles(files);
    }
}

void LfDasWidget::reloadLoadedFiles()
{
    for (Segment& segment : m_segments) {
        if (segment.loaded) {
            segment.loaded = false;
            segment.failed = false;
            segment.data.clear();
        }
    }
    scheduleRangeLoad();
}

void LfDasWidget::fitToAll()
{
    if (m_segments.isEmpty()) {
        return;
    }

    m_internalRangeChange = true;
    m_plot->xAxis->setRange(m_segments.first().startSecs(), m_segments.last().endSecs());
    m_plot->yAxis->setRange(0, std::max(1, maxDepthCols()) * m_depthIntervalMeters);
    m_internalRangeChange = false;
    scheduleRangeLoad();
}

void LfDasWidget::scheduleRangeLoad()
{
    m_rangeLoadTimer.start();
}

void LfDasWidget::rebuildSegments()
{
    m_rangeLoadTimer.stop();
    m_segments.clear();

    for (const QString& file : std::as_const(m_files)) {
        int rows = 0;
        int cols = 0;
        if (!readBinaryHeader(file, rows, cols)) {
            continue;
        }

        Segment segment;
        segment.filePath = file;
        segment.rows = rows;
        segment.cols = cols;
        segment.startTime = parseStartTime(file);
        if (!segment.startTime.isValid()) {
            segment.startTime = QFileInfo(file).lastModified();
        }
        segment.endTime = segment.startTime.addMSecs(
            static_cast<qint64>(std::ceil(fileDurationSeconds(rows) * 1000.0)));
        m_segments.push_back(segment);
    }

    std::sort(m_segments.begin(), m_segments.end(), [](const Segment& lhs, const Segment& rhs) {
        if (lhs.startTime == rhs.startTime) {
            return lhs.filePath < rhs.filePath;
        }
        return lhs.startTime < rhs.startTime;
    });

    for (int i = 1; i < m_segments.size(); ++i) {
        if (m_segments[i].startTime <= m_segments[i - 1].startTime
            && m_segments[i].filePath > m_segments[i - 1].filePath) {
            m_segments[i].startTime = m_segments[i - 1].endTime;
            m_segments[i].endTime = m_segments[i].startTime.addMSecs(
                static_cast<qint64>(std::ceil(fileDurationSeconds(m_segments[i].rows) * 1000.0)));
        }
    }

    updateStatus();

    if (m_segments.isEmpty()) {
        m_colorMap->data()->setSize(1, 1);
        m_colorMap->data()->setRange(QCPRange(0, 1), QCPRange(0, 1));
        m_colorMap->data()->setCell(0, 0, 0.0);
        m_title->setText("DAS 瀑布图");
        m_plot->replot(QCustomPlot::rpQueuedReplot);
        return;
    }

    const double start = m_segments.first().startSecs();
    const double end = std::min(start + m_visibleSpanSeconds, m_segments.last().endSecs());

    m_internalRangeChange = true;
    m_plot->xAxis->setRange(start, end);
    m_plot->yAxis->setRange(0, std::max(1, maxDepthCols()) * m_depthIntervalMeters);
    m_internalRangeChange = false;

    const QString titleTime = m_segments.first().startTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
    m_title->setText(QString("DAS 瀑布图 | %1").arg(titleTime));
    scheduleRangeLoad();
}

bool LfDasWidget::readBinaryHeader(const QString& filePath, int& rows, int& cols) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    qint32 binaryRows = 0;
    qint32 binaryCols = 0;
    stream >> binaryRows >> binaryCols;

    if (binaryRows <= 0 || binaryCols <= 0) {
        return false;
    }

    rows = binaryRows;
    cols = binaryCols;
    return rows > 0 && cols > 0;
}

bool LfDasWidget::readBinaryData(const QString& filePath, Segment& segment) const
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

    QVector<double> data(static_cast<int>(count));
    const qint64 byteCount = count * static_cast<qint64>(sizeof(double));
    const QByteArray payload = file.read(byteCount);
    if (payload.size() != byteCount) {
        return false;
    }

    std::memcpy(data.data(), payload.constData(), static_cast<std::size_t>(byteCount));

    segment.rows = rows;
    segment.cols = cols;
    segment.data = std::move(data);
    return segment.data.size() == segment.rows * segment.cols;
}

QDateTime LfDasWidget::parseStartTime(const QString& filePath) const
{
    const QString fileName = QFileInfo(filePath).fileName();
    const QRegularExpression re("(\\d{8})[-_](\\d{6})(?:\\.(\\d{1,9}))?");
    const QRegularExpressionMatch match = re.match(fileName);
    if (!match.hasMatch()) {
        return QDateTime();
    }

    const QString dateText = match.captured(1);
    const QString timeText = match.captured(2);
    const QDate date(dateText.mid(0, 4).toInt(), dateText.mid(4, 2).toInt(), dateText.mid(6, 2).toInt());
    QTime time(timeText.mid(0, 2).toInt(), timeText.mid(2, 2).toInt(), timeText.mid(4, 2).toInt());
    if (!date.isValid() || !time.isValid()) {
        return QDateTime();
    }

    const QString fraction = match.captured(3);
    if (!fraction.isEmpty()) {
        const int ms = fraction.left(3).leftJustified(3, QLatin1Char('0')).toInt();
        time = time.addMSecs(ms);
    }

    return QDateTime(date, time, Qt::LocalTime);
}

bool LfDasWidget::ensureDataForRange(double lower, double upper)
{
    bool changed = false;
    showLoading(true);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    for (Segment& segment : m_segments) {
        if (segment.failed || segment.loaded) {
            continue;
        }
        if (segment.endSecs() < lower || segment.startSecs() > upper) {
            continue;
        }

        if (!readBinaryData(segment.filePath, segment)) {
            segment.failed = true;
            continue;
        }

        segment.endTime = segment.startTime.addMSecs(
            static_cast<qint64>(std::ceil(fileDurationSeconds(segment.rows) * 1000.0)));
        segment.loaded = true;
        changed = true;
    }

    QApplication::restoreOverrideCursor();
    showLoading(false);
    updateStatus();
    return changed;
}

void LfDasWidget::refreshMapForRange(double lower, double upper)
{
    if (m_segments.isEmpty()) {
        return;
    }

    double dataStart = std::numeric_limits<double>::max();
    double dataEnd = std::numeric_limits<double>::lowest();
    int depthCols = 0;
    bool hasData = false;

    for (const Segment& segment : m_segments) {
        if (!segment.loaded || segment.data.isEmpty()) {
            continue;
        }
        if (segment.endSecs() < lower || segment.startSecs() > upper) {
            continue;
        }

        dataStart = std::min(dataStart, std::max(segment.startSecs(), lower));
        dataEnd = std::max(dataEnd, std::min(segment.endSecs(), upper));
        depthCols = std::max(depthCols, segment.cols);
        hasData = true;
    }

    if (!hasData || dataEnd <= dataStart || depthCols <= 0) {
        return;
    }

    const int timeCells = std::max(1, static_cast<int>(std::ceil((dataEnd - dataStart) / m_sampleIntervalSeconds)));
    m_colorMap->data()->setSize(timeCells, depthCols);
    m_colorMap->data()->setRange(QCPRange(dataStart, dataStart + timeCells * m_sampleIntervalSeconds),
                                 QCPRange(0, depthCols * m_depthIntervalMeters));

    auto* mapData = m_colorMap->data();
    for (int t = 0; t < timeCells; ++t) {
        for (int d = 0; d < depthCols; ++d) {
            mapData->setCell(t, d, 0.0);
        }
    }

    for (const Segment& segment : m_segments) {
        if (!segment.loaded || segment.data.isEmpty()) {
            continue;
        }
        if (segment.endSecs() < dataStart || segment.startSecs() > dataEnd) {
            continue;
        }

        const int srcStart = std::max(0, static_cast<int>(std::floor((dataStart - segment.startSecs()) / m_sampleIntervalSeconds)));
        const int srcEnd = std::min(segment.rows, static_cast<int>(std::ceil((dataEnd - segment.startSecs()) / m_sampleIntervalSeconds)));

        for (int srcRow = srcStart; srcRow < srcEnd; ++srcRow) {
            const double x = segment.startSecs() + srcRow * m_sampleIntervalSeconds;
            const int dstCol = static_cast<int>(std::floor((x - dataStart) / m_sampleIntervalSeconds));
            if (dstCol < 0 || dstCol >= timeCells) {
                continue;
            }

            for (int depth = 0; depth < segment.cols; ++depth) {
                const double value = segment.data[srcRow * segment.cols + depth];
                if (!std::isfinite(value)) {
                    continue;
                }
                mapData->setCell(dstCol, depth, value);
            }
        }
    }

    smoothSegmentBoundaries(mapData, timeCells, depthCols, dataStart, dataEnd);

    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::lowest();
    for (int t = 0; t < timeCells; ++t) {
        for (int d = 0; d < depthCols; ++d) {
            const double value = mapData->cell(t, d);
            if (!std::isfinite(value)) {
                continue;
            }
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
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}

void LfDasWidget::smoothSegmentBoundaries(QCPColorMapData* mapData,
                                          int timeCells,
                                          int depthCols,
                                          double dataStart,
                                          double dataEnd)
{
    if (!mapData || timeCells <= 2 || depthCols <= 0) {
        return;
    }

    const int blendCells = 3;
    for (int i = 1; i < m_segments.size(); ++i) {
        const Segment& previous = m_segments[i - 1];
        const Segment& current = m_segments[i];
        if (!previous.loaded || !current.loaded || previous.data.isEmpty() || current.data.isEmpty()) {
            continue;
        }

        const double boundary = current.startSecs();
        if (boundary <= dataStart || boundary >= dataEnd) {
            continue;
        }

        const int boundaryCol = static_cast<int>(std::round((boundary - dataStart) / m_sampleIntervalSeconds));
        const int leftAnchor = std::max(0, boundaryCol - blendCells - 1);
        const int rightAnchor = std::min(timeCells - 1, boundaryCol + blendCells);
        if (rightAnchor - leftAnchor < 2) {
            continue;
        }

        for (int depth = 0; depth < depthCols; ++depth) {
            const double leftValue = mapData->cell(leftAnchor, depth);
            const double rightValue = mapData->cell(rightAnchor, depth);
            if (!std::isfinite(leftValue) || !std::isfinite(rightValue)) {
                continue;
            }

            for (int col = leftAnchor + 1; col < rightAnchor; ++col) {
                const double ratio = static_cast<double>(col - leftAnchor) / (rightAnchor - leftAnchor);
                mapData->setCell(col, depth, leftValue * (1.0 - ratio) + rightValue * ratio);
            }
        }
    }
}

void LfDasWidget::updateStatus()
{
    int loadedCount = 0;
    int failedCount = 0;
    for (const Segment& segment : std::as_const(m_segments)) {
        loadedCount += segment.loaded ? 1 : 0;
        failedCount += segment.failed ? 1 : 0;
    }

    if (m_segments.isEmpty()) {
        m_statusLabel->setText("未加载文件");
        return;
    }

    const QString timeRange = QString("%1 - %2")
        .arg(m_segments.first().startTime.toString("yyyy-MM-dd HH:mm:ss"))
        .arg(m_segments.last().endTime.toString("HH:mm:ss"));
    QString status = QString("%1 个文件，已缓存 %2 个，%3")
        .arg(m_segments.size())
        .arg(loadedCount)
        .arg(timeRange);
    if (failedCount > 0) {
        status += QString("，失败 %1 个").arg(failedCount);
    }
    m_statusLabel->setText(status);
}

void LfDasWidget::showLoading(bool visible)
{
    if (!m_loadingLabel) {
        return;
    }
    m_loadingLabel->resize(m_plot->viewport().size());
    m_loadingLabel->move(m_plot->viewport().topLeft());
    m_loadingLabel->setVisible(visible);
    if (visible) {
        m_loadingLabel->raise();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

QCPRange LfDasWidget::paddedRange(const QCPRange& range) const
{
    const double width = std::max(range.size(), m_sampleIntervalSeconds);
    return QCPRange(range.lower - width * 0.7, range.upper + width * 0.7);
}

int LfDasWidget::maxDepthCols() const
{
    int cols = 0;
    for (const Segment& segment : m_segments) {
        cols = std::max(cols, segment.cols);
    }
    return cols;
}

double LfDasWidget::fileDurationSeconds(int rows) const
{
    return std::max(1, rows) * m_sampleIntervalSeconds;
}
