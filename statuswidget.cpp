#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <QColor>
#include <QLabel>
#include <QProgressBar>
#include <QStorageInfo>
#include <QtGlobal>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    ui->label_RunStatus->setStyleSheet("color:#12C17E;border-color:#36EB79;");
    ui->label_DAS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_DTS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_fileSave->setStyleSheet("color:#FFFF00;border-color:#FFFF00;");


    QString qss = R"(StatusWidget QWidget {
            background-color: #202228;
            border-top: 1px solid #333333;
            color: #999999;
            font-size: 12px;
            padding: 4px;
        }

        /* --- 进度条样式 (CPU/内存占用) --- */
        QProgressBar {
            background-color: #333333;
            border: 1px solid #444444;
            border-radius: 3px;
            text-align: center;
            height: 12px; /* 细进度条 */
            width: 100px;
            margin: 2px;
        }

        /* 进度条滑块 - 统一使用蓝色系，通过代码动态改变颜色更佳，这里给个默认色 */
        QProgressBar::chunk {
            background-color: #4A90E2; /* 统一品牌蓝 */
            border-radius: 2px;
        }

        /* 如果想保留多色（CPU绿、内存紫），可以定义不同的 class 或 objectName */
        QProgressBar#cpuBar::chunk { background-color: #2ECC71; }
        QProgressBar#memBar::chunk { background-color: #9B59B6; }
        QProgressBar#diskBar::chunk { background-color: #E67E22; })";

  //  this->setStyleSheet(qss);


    QString qssMemery = R"(QProgressBar {
        border: 2px solid rgba(255, 255, 255, 0.2);
        border-radius: 8px;
        background-color:rgba(255,255,255,0.4);
        text-align: center;
        color: #000000;
        }

        QProgressBar::chunk {
        background-color:rgba(139,18,174,0.8);
        background-color: #4A90E2; /* 统一品牌蓝 */
        border-bottom-right-radius:8px;
        border-bottom-left-radius:8px;
        border-top-right-radius:8px;
        border-top-left-radius:8px;
        }

        )";
    ui->progressBar_mem->setStyleSheet(qssMemery);


    QString qssDisk = R"(
                QProgressBar {
                border: 2px solid rgba(255, 255, 255, 0.2);
                    border-radius: 8px;
                    background-color: rgba(255,255,255,0.4);
                    text-align: center;
                color: #000000;
                }

                QProgressBar::chunk {
                    /*background-color: rgba(77,166,12, 0.4);*/
                    background-color: #4A90E2; /* 统一品牌蓝 */
                    border-bottom-right-radius:8px;
                    border-bottom-left-radius:8px;
                    border-top-right-radius:8px;
                    border-top-left-radius:8px;
                }

        )";
    ui->progressBar_disk->setStyleSheet(qssDisk);

    QString qssCpu = R"(
            QProgressBar {
            border: 2px solid rgba(255, 255, 255, 0.2);
            border-radius: 8px;
            background-color:rgba(255,255,255,0.4);
            text-align: center;
            color: #000000;
            }

            QProgressBar::chunk {
            /*background-color:rgba(31,133,191,0.8);*/
            background-color: #4A90E2; /* 统一品牌蓝 */
            border-bottom-right-radius:8px;
            border-bottom-left-radius:8px;
            border-top-right-radius:8px;
            border-top-left-radius:8px;
            }
        )";
    ui->progressBar_cpu->setStyleSheet(qssCpu);

    const QString usageLabelStyle = QStringLiteral("color:#D8DEE9;font-size:12px;");
    ui->label->setStyleSheet(usageLabelStyle);
    ui->label_11->setStyleSheet(usageLabelStyle);
    ui->label_12->setStyleSheet(usageLabelStyle);

    initUsageBars();
    updateSystemUsage();
    connect(&m_usageTimer, &QTimer::timeout, this, &StatusWidget::updateSystemUsage);
    m_usageTimer.start(1000);
}




StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::initUsageBars()
{
    const QList<QProgressBar*> bars = {
        ui->progressBar_cpu,
        ui->progressBar_disk,
        ui->progressBar_mem
    };

    for (QProgressBar* bar : bars) {
        bar->setRange(0, 100);
        bar->setFormat("%p%");
        bar->setTextVisible(true);
        bar->setValue(0);
        bar->setFixedHeight(16);
        bar->setMinimumWidth(112);
        bar->setMaximumWidth(112);
    }

    setUsageBarStyle(ui->progressBar_cpu, QColor("#35C987"));
    setUsageBarStyle(ui->progressBar_mem, QColor("#4DA3FF"));
    setUsageBarStyle(ui->progressBar_disk, QColor("#D7A343"));
}

void StatusWidget::setUsageBarStyle(QProgressBar* bar, const QColor& color) const
{
    if (!bar) {
        return;
    }

    bar->setStyleSheet(QStringLiteral(R"(
        QProgressBar {
            border: 1px solid #3A404A;
            border-radius: 5px;
            background-color: #252A31;
            color: #E7ECF2;
            font-size: 11px;
            font-weight: 500;
            text-align: center;
            padding: 1px;
        }

        QProgressBar::chunk {
            background-color: %1;
            border-radius: 4px;
            margin: 1px;
        }
    )").arg(color.name()));
}

QColor StatusWidget::usageColor(int value, const QColor& normalColor) const
{
    if (value >= 90) {
        return QColor("#E85D5D");
    }

    if (value >= 75) {
        return QColor("#E2A044");
    }

    return normalColor;
}

void StatusWidget::updateSystemUsage()
{
    const int cpuUsage = readCpuUsage();
    const int memUsage = readMemoryUsage();
    const int diskUsage = readDiskUsage();

    if (cpuUsage >= 0) {
        setUsageBarStyle(ui->progressBar_cpu, usageColor(cpuUsage, QColor("#35C987")));
        ui->progressBar_cpu->setValue(cpuUsage);
        ui->progressBar_cpu->setToolTip(QString("CPU占用: %1%").arg(cpuUsage));
    }

    if (memUsage >= 0) {
        setUsageBarStyle(ui->progressBar_mem, usageColor(memUsage, QColor("#4DA3FF")));
        ui->progressBar_mem->setValue(memUsage);
        ui->progressBar_mem->setToolTip(QString("内存占用: %1%").arg(memUsage));
    }

    if (diskUsage >= 0) {
        setUsageBarStyle(ui->progressBar_disk, usageColor(diskUsage, QColor("#D7A343")));
        ui->progressBar_disk->setValue(diskUsage);
        ui->progressBar_disk->setToolTip(QString("系统盘占用: %1%").arg(diskUsage));
    }
}

int StatusWidget::readCpuUsage()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return -1;
    }

    const quint64 idle = fileTimeToUInt64(&idleTime);
    const quint64 kernel = fileTimeToUInt64(&kernelTime);
    const quint64 user = fileTimeToUInt64(&userTime);

    if (!m_hasCpuSample) {
        m_lastIdleTime = idle;
        m_lastKernelTime = kernel;
        m_lastUserTime = user;
        m_hasCpuSample = true;
        return ui->progressBar_cpu->value();
    }

    const quint64 idleDelta = idle - m_lastIdleTime;
    const quint64 kernelDelta = kernel - m_lastKernelTime;
    const quint64 userDelta = user - m_lastUserTime;
    const quint64 totalDelta = kernelDelta + userDelta;

    m_lastIdleTime = idle;
    m_lastKernelTime = kernel;
    m_lastUserTime = user;

    if (totalDelta == 0) {
        return 0;
    }

    const double usage = (1.0 - static_cast<double>(idleDelta) / static_cast<double>(totalDelta)) * 100.0;
    return qBound(0, static_cast<int>(usage + 0.5), 100);
}

int StatusWidget::readMemoryUsage() const
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (!GlobalMemoryStatusEx(&memStatus)) {
        return -1;
    }

    return qBound(0, static_cast<int>(memStatus.dwMemoryLoad), 100);
}

int StatusWidget::readDiskUsage() const
{
    const QStorageInfo storage = QStorageInfo::root();
    if (!storage.isValid() || storage.bytesTotal() <= 0) {
        return -1;
    }

    const qint64 used = storage.bytesTotal() - storage.bytesAvailable();
    const double usage = static_cast<double>(used) * 100.0 / static_cast<double>(storage.bytesTotal());
    return qBound(0, static_cast<int>(usage + 0.5), 100);
}

quint64 StatusWidget::fileTimeToUInt64(const void* fileTime) const
{
    const FILETIME* ft = static_cast<const FILETIME*>(fileTime);
    ULARGE_INTEGER value;
    value.LowPart = ft->dwLowDateTime;
    value.HighPart = ft->dwHighDateTime;
    return value.QuadPart;
}
