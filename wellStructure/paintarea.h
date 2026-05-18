#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QPen>
#include <QWidget>
#include <QPainter>
class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);

    enum Shape {cannula,oil,perforate,cement};

    void setShape(Shape shape,QPen pen,QBrush brush,int width,int height);
    void setPen(QPen pen);
    void setBrush(QBrush brush);
    void setFillRule(Qt::FillRule rule);
    void paintEvent(QPaintEvent *event);

    void setCannulaH(int h1,int h2,int h3);
    void setScale(double scale){m_scale=scale;}
private:
    Shape m_shape;
    QPen m_pen;
    QBrush m_brush;

    QPen m_pen_oil;
    QBrush m_brush_oil;

    QPen m_pen_perforate;
    QBrush m_brush_perforate;

    QPen m_pen_cement;
    QBrush m_brush_cement;

    Qt::FillRule m_fillrule;

    int m_cannula_w = 120;//套管
    int m_cannula_h = 200;

    int m_cannula_w_2 = 140;//套管
    int m_cannula_h_2 = 200;

    int m_cannula_w_3 = 160;//套管
    int m_cannula_h_3 = 200;



    int m_Oil_w = 70;//油管
    int m_Oil_h = 490;


    int m_perforate_w = 50;//穿孔
    int m_perforate_h = 450;

    int m_cement_w = 90;//水泥
    int m_cement_h = 500;


    QImage  m_Img_perforate;
    QImage m_equipment;
    QImage m_ground;


    double m_scale = 1;

};

#endif // PAINTAREA_H
