#include "convertsetupdlg.h"
#include "ui_convertsetupdlg.h"
#include "global.h"
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include "binary_file_merger.h"

ConvertSetupDlg::ConvertSetupDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConvertSetupDlg)
{
    ui->setupUi(this);
    QTime initialTime(9, 30, 0);
    ui->timeEdit->setTime(initialTime);

    QTime initialTime2(10, 30, 0);
    ui->timeEdit_2->setTime(initialTime2);

    m_szOutputPath = "d:/output/4/";
    ui->lineEdit->setText(m_szOutputPath);

}

ConvertSetupDlg::~ConvertSetupDlg()
{
    delete ui;
}

void ConvertSetupDlg::setInputPath(const QString& path)
{
    m_szInputPath = path;
    qDebug()<<"m_szInputPath"<<m_szInputPath;
}

void ConvertSetupDlg::on_pushButton_close_clicked()
{
    close();
}

void ConvertSetupDlg::on_pushButton_start_clicked()
{
    QString path = ui->lineEdit->text();
    QDir dir(path);

    if (!dir.exists())
    {
        // mkpath 会尝试创建路径中缺失的所有父目录
        if (dir.mkpath(".")) {
            qDebug() << "文件夹创建成功:" << path;
        } else {
            qDebug() << "文件夹创建失败，请检查权限";
            return;
        }
    }
    m_szOutputPath = path;
    m_szOutputPath = QDir::cleanPath(m_szOutputPath);
    // 确保结尾有斜杠
    if (!m_szOutputPath.endsWith('/')) {
        m_szOutputPath += '/';
    }

    isStopRequested = false;
    totalTimer.start();


    QTime startTime = ui->timeEdit->time();
    QTime endTime = ui->timeEdit_2->time();

    currentIndex = 0;
    m_fileList = filterFilesByTime(m_szInputPath, startTime, endTime);
    qDebug()<<"m_fileList size"<<m_fileList.size();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    processNextFile(); // 启动处理
}

void ConvertSetupDlg::on_pushButton_stop_clicked()
{
    isStopRequested = true;
}

void ConvertSetupDlg::processNextFile() {
    // 检查是否被用户要求停止
    if (isStopRequested || currentIndex >= m_fileList.size()) {
        qDebug() << "所有任务完成，总耗时:" << totalTimer.elapsed() << "ms";
        QApplication::restoreOverrideCursor();
        qDebug() << "任务结束或已中止。";


        return;
    }

    ui->label_10->setText(QString::number(totalTimer.elapsed()) + " ms");

    fileTimer.start();
    // 调用 QProcess
    QProcess *process = new QProcess(this);
    QString inputFile = m_fileList[currentIndex];
    QString appPath = QApplication::applicationDirPath();
    appPath += "/DasDownsampleTool.exe";

    QFileInfo appInfo(appPath);
    process->setWorkingDirectory(appInfo.absolutePath());
    process->setProcessChannelMode(QProcess::ForwardedChannels);


    ui->label_5->setText(inputFile);

    /*process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput();
        // 将 byte array 转为字符串并打印或处理
        QString text = QString::fromLocal8Bit(output);
        qDebug() << "Worker 输出:" << text;
    });*/


    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
        process->deleteLater();
        currentIndex++;
        QString msg = QString("%1 / %2").arg(currentIndex).arg(m_fileList.size());
        ui->label_6->setText(msg);

        qint64 fileTime = fileTimer.elapsed();
        qDebug() << "文件" << inputFile << "处理完成，用时:" << fileTime << "ms";

        ui->label_9->setText(QString::number(fileTimer.elapsed()) + " ms");

        // 递归处理下一个文件，而不是用 for 循环
        processNextFile();
    });


    process->start(appPath, {inputFile, m_szOutputPath});
}

void ConvertSetupDlg::on_pushButton_brow_clicked()
{
    QString path = selectFolder(this);
    if (path.isEmpty())
    {
        return;
    }
    ui->lineEdit->setText(path);

    m_szOutputPath = path;
}

QList<QString> ConvertSetupDlg::filterFilesByTime(const QString &directoryPath,
                                 const QTime &startTime,
                                 const QTime &endTime)
{
    QList<QString> resultFiles;
    QDir dir(directoryPath);

    // 只过滤出 .h5 文件
    QStringList nameFilters;
    nameFilters << "*.h5";
    dir.setNameFilters(nameFilters);

    // 获取所有文件列表
    QFileInfoList fileList = dir.entryInfoList();
    qDebug()<<"ConvertSetupDlg fileList.size"<<fileList.size();

    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.fileName();//h5_20251023-100405.624751733_333.h5

        // 1. 提取时间部分: "h5_20251023-100305"
        // 假设文件名固定格式，可以使用 split 或 mid
        // 格式: h5_YYYYMMDD-HHMMSS
        QStringList parts = fileName.split('_');
        if (parts.size() < 3) continue; // 简单校验

        QString timeStr = parts[1]; // 20251023-100305.624751733
        // 只取前 15 个字符: 20251023-100405
        QString refinedTimeStr = timeStr.left(15);
        QString timeOnlyStr = refinedTimeStr.right(6);//100405
        //qDebug()<<"timeOnlyStr"<<timeOnlyStr;

        QTime fileTime = QTime::fromString(timeOnlyStr, "hhmmss");

       // qDebug()<<"filterFilesByTime"<<fileTime.toString() << startTime.toString()<<endTime.toString();
        // 3. 时间范围比对
        if (fileTime.isValid() && fileTime >= startTime && fileTime <= endTime) {
            resultFiles.append(fileInfo.absoluteFilePath());
        }
    }

    return resultFiles;
}

void mergeFileByType( FileCollection& manager, const QString& type, const QString& outputName)
{
    const auto& fkFiles = manager.getFiles(type);
    std::vector<std::string> vec;
    vec.reserve(fkFiles.size()); // 预分配内存，避免多次重新分配

    std::transform(fkFiles.begin(), fkFiles.end(), std::back_inserter(vec),
                   [](const QString& qs) {
                       return qs.toStdString(); // 将 QString 转为 std::string
                   });

    std::string err;
    bool ok = false;
    if (type == "spectrum_power")
    {
        ok = mergeSpectrumPowerToDb(vec, outputName.toStdString());
    }
    else
    {
        ok = mergeBinaryFilesByMedian(vec, outputName.toStdString(), &err);
    }
    qDebug()<<"mergeBinaryFiles "<<type<<ok;
}

void ConvertSetupDlg::on_pushButton_merge_clicked()
{
    FileCollection manager;
    manager.organize(m_szOutputPath);
    qDebug()<<m_szOutputPath;

    mergeFileByType(manager, "fk2", "d:/output/5/fk.merge.bin");
    mergeFileByType(manager, "fbe", "d:/output/5/fbe.merge.bin");
    mergeFileByType(manager, "lfdas", "d:/output/5/lfdas.merge.bin");
    mergeFileByType(manager, "spectrum_power", "d:/output/5/spectrum.merge.bin");
    mergeFileByType(manager, "bands", "d:/output/5/band_energy.merge.bin");
    mergeFileByType(manager, "down", "d:/output/5/downsample.merge.bin");
    return;

    // 现在你可以很方便地调用任意一类文件
    {
        const auto& fkFiles = manager.getFiles("fk");
        std::vector<std::string> vec;
        vec.reserve(fkFiles.size()); // 预分配内存，避免多次重新分配

        std::transform(fkFiles.begin(), fkFiles.end(), std::back_inserter(vec),
                       [](const QString& qs) {
                           return qs.toStdString(); // 将 QString 转为 std::string
                       });

        std::string err;
        bool ok = mergeBinaryFiles(vec, "fk.merge.bin", &err);
        qDebug()<<"mergeBinaryFiles fk"<<ok;
    }


    //分别合并
    std::vector<std::string> files = {
        "20260525_0000.lfdas.bin",
        "20260525_0001.lfdas.bin",
        "20260525_0002.lfdas.bin"
    };

    std::string err;
    bool ok = mergeBinaryFiles(files, "20260525_0000_0002.lfdas.merge.bin", &err);
    qDebug()<<ok;
}

