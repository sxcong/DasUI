#ifndef SPECTRUMDBWIDGET_H
#define SPECTRUMDBWIDGET_H

#include <QLabel>
#include <QVector>
#include <QWidget>
#include <qcustomplot.h>

class QMouseEvent;

class SpectrumDbWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpectrumDbWidget(QWidget* parent = nullptr);

    void setFiles(const QStringList& files);
    void setSampleRate(double sampleRateHz);

private slots:
    void openFiles();
    void showPreviousFile();
    void showNextFile();
    void reloadCurrentFile();
    void MyMouseMoveEvent(QMouseEvent* event);

private:
    struct BinaryMatrix {
        int rows = 0;
        int cols = 0;
        QVector<double> data;

        bool isValid() const { return rows > 0 && cols > 0 && data.size() == rows * cols; }
        double at(int row, int col) const { return data[row * cols + col]; }
    };

    void setupUi();
    void setupPlot();
    bool readBinaryMatrix(const QString& filePath, BinaryMatrix& matrix) const;
    void displayFile(int index);
    void displayMatrix(const QString& filePath, const BinaryMatrix& matrix);
    void updateStatus();

    QStringList m_files;
    int m_currentIndex = -1;
    double m_sampleRateHz = 2000.0;

    QCustomPlot* m_plot = nullptr;
    QCPColorMap* m_colorMap = nullptr;
    QCPColorScale* m_colorScale = nullptr;
    QCPTextElement* m_title = nullptr;
    QLabel* m_statusLabel = nullptr;
};

#endif // SPECTRUMDBWIDGET_H
