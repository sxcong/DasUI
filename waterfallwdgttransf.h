#ifndef WATERFALLWDGTTRANSF_H
#define WATERFALLWDGTTRANSF_H

#include <QWidget>
#include <qcustomplot.h>
#include <QVector>

class WaterFallWdgtTransf : public QWidget
{
    Q_OBJECT
public:
    explicit WaterFallWdgtTransf(QWidget *parent = nullptr);
    void waterFallInit();
    void waterFallShow(const QVector<double> &data);
    void setRangeX(int min,int max);
    void setRangeY(int min, int max);
    void setRangeColor(int minBlue,int maxRed);

    void setDataLenth(int length);
    void reset();

    void setType(int type);
    void update(){fp3->replot();}

    void setDataInterval(double interval);
    void setCptext(QString text);
    void setRangeTime(int sec);
    void stop(bool isStop){IsUpdate = !isStop;}
signals:
    void signal_alignRangeY(int min,int max);
public slots:
    void yAxisChanged(const QCPRange &newRange, const QCPRange &oldRange);
    void slot_yAxisChanged(double min,double max)
    {
        fp3->yAxis->setRange(min,max);
        fp3->replot();
    }
    void CPColorRangeChange(const QCPRange &newRange);
    void contextMenuRequest(const QPoint &pos);
    void paintbigpoint(QMouseEvent *event);
private:
    QCustomPlot * fp3 = nullptr;
    QCPColorMap *m_pColorMap= nullptr;
    QCPColorScale *m_pColorScale = nullptr;
    int m_Start = 0;
    int m_End = 14336;
    int m_Length = 14336;
    QVector<QVector<double>>value_lofar;
    double data_interval = 1;
    uint m_TimeInterval = 15;
    uint m_CurrTimet = 0;

    int  m_type = 1;//1为das 2为dts 3为相位
    int m_XCount = 150;//150
    QMutex m_mutex;
    QSharedPointer<QCPAxisTickerDateTime> m_Timer;

    QMenu menu;

    double m_Ymin  = 0;
    double m_Ymax = 1000;

    bool IsUpdate = true;


    int m_ActualLength = 14336;

    qint64 m_Xmin = 0;
    qint64 m_Xmax = 0;


    bool m_istopColorscale = false;
    // QObject interface
};

#endif // WATERFALLWDGTTRANSF_H
