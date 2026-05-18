#ifndef CURVEVIEW_H
#define CURVEVIEW_H

#include "pointtracer.h"

#include <QWidget>
#include <qcustomplot.h>
#include "axisrange.h"
namespace Ui {
class CurveView;
}

class CurveView : public QWidget
{
    Q_OBJECT

public:
    explicit CurveView(QWidget *parent = nullptr);
    ~CurveView();
public:
    void curveInit();
    //数据处理部分
    void addData1(double keys, double values);
    void updateLine1(const QVector<double> &keys, const QVector<double> &values);
    void updateLine(const QList<QPointF> &list);
    void updateLine(const QVector<QPointF> &list);
    void clearAllline();

    //坐标轴部分
    void resetAxes();
    void setRangeX(const int nMin, const int nMax);// 设置X轴坐标范
    void setRangeY(const double nMin, const double nMax);// 设置Y轴坐标范围
    void rescaleAllline();
    void setupSimpleItemDemo(QCustomPlot *customPlot);

    void updateSymbolicLines(QList<QList<QPointF> > &linePoints, int alarmType);
    void updateThreshold(int channel);
    void clearThreshold();
    void createNewLine();

    void setIsAmplitude(bool IsAmplitude){m_IsAmplitude = IsAmplitude;}


    void setAxisFormat(QString format,bool IsxAxis = true)
    {
        if(IsxAxis)
        {
            mPlot->xAxis->setAxisFormat(format);
        }
        else
        {
            mPlot->yAxis->setAxisFormat(format);
        }

    }


    void setType(int type )
    {
        m_type = type;
    }
signals:
    void signal_CurveXRangeChange(const double min,const double max);

public slots:
    void contextMenuRequest(const QPoint &pos);
    void selectionChanged();
    void xAxisChanged(const QCPRange &newRange,const QCPRange &oldRange);
    void yAxisChanged(const QCPRange &newRange,const QCPRange &oldRange);

    void paintbigpoint(QMouseEvent *event);
    void slot_yAxisChange(int min,int max)
    {
            mPlot->yAxis->setRange(min,max);
            mPlot->replot();
    }

    void slot_axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
private:
    QCustomPlot *mPlot= nullptr;
    QPointer<QCPGraph> mGraph1 =nullptr;
    QCPItemStraightLine *m_line = nullptr;  // tooltip的垂直线
    void MyMouseMoveEvent(QMouseEvent* event);

    int m_x1 = 0;
    int m_x2 = 14336;
    double m_y1 = 0;
    double m_y2 = 50;

    Ui::CurveView *ui;
    PointTracer *m_Traser = nullptr;

    bool m_IsAmplitude = true;

    AxisRange * m_axisRange = nullptr;


    int m_type = 0;//0代表温度，1代表振动,2为FBE
};

#endif // CURVEVIEW_H
