#ifndef WATERFALLDIALOG_H
#define WATERFALLDIALOG_H

#include <QDialog>
#include <qcustomplot.h>
#include <Eigen/Dense>
#include <QLabel>

class WaterfallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WaterfallDialog(const QString& h5Path, QWidget *parent = nullptr);
    ~WaterfallDialog();

private:
    bool loadAndDisplay(const QString& h5Path);
    void downsample(const Eigen::MatrixXf& src, Eigen::MatrixXf& dst, int factor);

    QString m_h5Path;
    QLabel *m_loadingLabel;
    QCustomPlot *m_plot;
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
};

#endif // WATERFALLDIALOG_H
