#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QTimer>

class QColor;
class QProgressBar;

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget();

private slots:
    void updateSystemUsage();

private:
    void initUsageBars();
    void setUsageBarStyle(QProgressBar* bar, const QColor& color) const;
    QColor usageColor(int value, const QColor& normalColor) const;
    int readCpuUsage();
    int readMemoryUsage() const;
    int readDiskUsage() const;
    quint64 fileTimeToUInt64(const void* fileTime) const;

    Ui::StatusWidget *ui;
    QTimer m_usageTimer;
    quint64 m_lastIdleTime = 0;
    quint64 m_lastKernelTime = 0;
    quint64 m_lastUserTime = 0;
    bool m_hasCpuSample = false;
};

#endif // STATUSWIDGET_H
