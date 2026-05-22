#ifndef FBEPLOTDIALOG_H
#define FBEPLOTDIALOG_H

#include <QDialog>
#include <qcustomplot.h>
#include <Eigen/Dense>
#include <QLabel>

struct FrequencyBand {
    QString label;
    double lowHz;
    double highHz;
};

class FbePlotDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FbePlotDialog(const QString& h5Path, QWidget *parent = nullptr);
    ~FbePlotDialog();

private:
    bool loadAndCompute(const QString& h5Path);
    void computeFBE(const Eigen::MatrixXf& data, double sampleRate,
                    Eigen::MatrixXf& fbeMatrix,
                    QVector<FrequencyBand>& bands);

    double readSampleRate(const QString& h5Path);

    QString m_h5Path;
    QLabel *m_loadingLabel;
    QCustomPlot *m_plot;
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
};

#endif // FBEPLOTDIALOG_H
