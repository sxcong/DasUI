#ifndef SPECTRUMANALYSISWIDGET_H
#define SPECTRUMANALYSISWIDGET_H

#include <QWidget>
#include <QString>
#include <QVector>
#include "qcustomplot.h"

struct SpectrumBinaryMatrix {
    int rows = 0;
    int cols = 0;
    QVector<double> data;

    bool isValid() const { return rows > 0 && cols > 0 && data.size() == rows * cols; }
    double at(int r, int c) const { return data[r * cols + c]; }
};

class SpectrumAnalysisWidget : public QDialog //QWidget
{
    Q_OBJECT
public:
    explicit SpectrumAnalysisWidget(const QString& bandEnergyFile,
                                    const QString& spectrumFile,
                                    QWidget* parent = nullptr);
    SpectrumAnalysisWidget(const QString& bandEnergyFile,
                           const QString& spectrumFile,
                           double sampleRateHz,
                           QWidget* parent = nullptr);

    void setSampleRate(double sampleRateHz);
    void reload();
private slots:
    void showValueAtMouse(QMouseEvent* event);
private:
    bool readBinaryMatrix(const QString& filePath, SpectrumBinaryMatrix& matrix) const;
    void setupUi();
    void setupPlotStyle(QCustomPlot* plot, const QString& title);
    void plotBandEnergy(const SpectrumBinaryMatrix& matrix);
    void plotSpectrum(const SpectrumBinaryMatrix& matrix);

    QString m_bandEnergyFile;
    QString m_spectrumFile;
    double m_sampleRateHz = 2000.0;

    QCustomPlot* m_bandPlot = nullptr;
    QCustomPlot* m_spectrumPlot = nullptr;
    QCPColorMap* m_colorMap = nullptr;
    QCPColorScale* m_colorScale = nullptr;

    QCPItemText* m_tooltip = nullptr; // 用于显示信息的文字项
};

#endif
