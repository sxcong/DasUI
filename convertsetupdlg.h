#ifndef CONVERTSETUPDLG_H
#define CONVERTSETUPDLG_H

#include <QDialog>
#include <QElapsedTimer>
#include <QTime>
#include <QDir>
#include <QMap>
#include <QList>

namespace Ui {
class ConvertSetupDlg;
}

class FileCollection {
public:
    // 存储分类后的列表
    QMap<QString, QList<QString>> files;

    // 组织文件：传入目录路径
    void organize(const QString &dirPath) {
        QDir dir(dirPath);
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);

        for (const QFileInfo &fileInfo : fileInfoList) {
            QString ext = fileInfo.suffix().toLower();
            // 只收集我们关心的类型
            if (isSupported(ext)) {
                files[ext].append(fileInfo.absoluteFilePath());
            }
        }
    }

    // 获取特定类型的列表
    const QList<QString>& getFiles(const QString &ext) const {
        return files.value(ext);
    }

private:
    bool isSupported(const QString &ext) {
        static const QList<QString> validExts = {
            "fk2", "fbe", "lfdas", "spectrum_power", "bands", "down"
        };
        return validExts.contains(ext);
    }
};


class ConvertSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConvertSetupDlg(QWidget *parent = nullptr);
    ~ConvertSetupDlg();
    void setInputPath(const QString& path);
private:
    QList<QString> filterFilesByTime(const QString &directoryPath,
                                     const QTime &startTime,
                                     const QTime &endTime);

    void processNextFile();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_brow_clicked();

    void on_pushButton_merge_clicked();

private:
    Ui::ConvertSetupDlg *ui;
    QString m_szInputPath;
    QString m_szOutputPath;
    std::atomic<bool> isStopRequested{false};
    int currentIndex = 0;
    QList<QString> m_fileList;

    QElapsedTimer totalTimer;
    QElapsedTimer fileTimer;
};

#endif // CONVERTSETUPDLG_H
