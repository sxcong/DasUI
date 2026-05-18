#ifndef POINTTRACER_H
#define POINTTRACER_H

#include <QObject>
#include "qcustomplot.h"

enum TracerType
{
    XAxisTracer,
    YAxisTracer,
    DataTracer,
    CustomerTracer
};

class PointTracer : public QObject
{
    Q_OBJECT
public:
    explicit PointTracer(QCustomPlot *_plot,QCPGraph *_graph, TracerType _type);//这里与原贴不同，按照原贴构造总是过不去
    ~PointTracer();

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setText(const QString &text,const QString &text1);//固定格式
    //void setText(const QString &text);
    void setText2(const QString &text);
    void setLabelPen(const QPen &pen);
    void updatePosition(double xValue, double yValue);

    void setVisible(bool visible);
protected:
    QCustomPlot *plot ;	     //传入实例化的QcustomPlot
    QCPGraph *graph;	   	 //这里是存传入的绘图图层
    QCPItemTracer *tracer;   // 跟踪的点
    QCPItemText *label;   	 // 显示的数值框
    QCPItemLine *arrow;  	 // 箭头引线

    TracerType type;
    bool visible;
public slots:
};

#endif // POINTTRACER_H
