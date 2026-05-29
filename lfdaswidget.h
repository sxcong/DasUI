#ifndef LFDASWIDGET_H
#define LFDASWIDGET_H

#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <qcustomplot.h>
class QMouseEvent;
class PointTracer;
class LfDasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LfDasWidget(QWidget* parent = nullptr);

    void setFiles(const QStringList& files);
    void addFiles(const QStringList& files);
    void setSampleIntervalSeconds(double seconds);
    void setDepthIntervalMeters(double meters);
    void setVisibleSpanSeconds(double seconds);

private slots:
    void openFiles();
    void reloadLoadedFiles();
    void fitToAll();
    void scheduleRangeLoad();

    void MyMouseMoveEvent(QMouseEvent* event);

private:
    struct Segment {
        QString filePath;
        QDateTime startTime;
        QDateTime endTime;
        int rows = 0;
        int cols = 0;
        bool loaded = false;
        bool failed = false;
        QVector<double> data;

        double startSecs() const { return startTime.toMSecsSinceEpoch() / 1000.0; }
        double endSecs() const { return endTime.toMSecsSinceEpoch() / 1000.0; }
    };

    void setupUi();
    void setupPlot();
    void rebuildSegments();
    bool readBinaryHeader(const QString& filePath, int& rows, int& cols) const;
    bool readBinaryData(const QString& filePath, Segment& segment) const;
    QDateTime parseStartTime(const QString& filePath) const;
    bool ensureDataForRange(double lower, double upper);
    void refreshMapForRange(double lower, double upper);
    void smoothSegmentBoundaries(QCPColorMapData* mapData,
                                 int timeCells,
                                 int depthCols,
                                 double dataStart,
                                 double dataEnd);
    void updateStatus();
    void showLoading(bool visible);
    QCPRange paddedRange(const QCPRange& range) const;
    int maxDepthCols() const;
    double fileDurationSeconds(int rows) const;

    QStringList m_files;
    QVector<Segment> m_segments;
    double m_sampleIntervalSeconds = 1.0;
    double m_depthIntervalMeters = 1.0;
    double m_visibleSpanSeconds = 10.0 * 60.0;
    bool m_internalRangeChange = false;

    QCustomPlot* m_plot = nullptr;
    QCPColorMap* m_colorMap = nullptr;
    QCPColorScale* m_colorScale = nullptr;
    QCPTextElement* m_title = nullptr;
    QLabel* m_statusLabel = nullptr;
    QLabel* m_loadingLabel = nullptr;
    QTimer m_rangeLoadTimer;
};

#endif // LFDASWIDGET_H
