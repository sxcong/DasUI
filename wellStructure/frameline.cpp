#include "frameline.h"

#include <QPainter>

FrameLine::FrameLine(QWidget *parent) : QWidget(parent)
{
//    this->setStyleSheet("background: #242F3A;\
//                        border-radius: 8px;\
//                        border: 1px solid #6B7380;");

}


void FrameLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(QPen(QBrush(QColor("#6B7380")),2));
    painter.setBrush(QBrush(QColor("#242F3A")));
//    QPainterPath path;
//    path.addRoundedRect(rect(), 8, 8);
//    painter.setClipPath(path);
    painter.drawRoundedRect(rect(),8,8);


    int rect_w = this->rect().width();
    int rect_h = this->rect().height();

    int rect_ceter_w = rect_w/2;
    int rect_ceter_h = rect_h/2;


    QRect pic_equipment(0,0,rect_w,rect_h);
    painter.drawImage(pic_equipment,m_pic,m_pic.rect());


    QRect rect_Oil(rect_ceter_w-60/2,m_height_Up,60,m_height_Down);
    painter.setPen(QPen(QColor(0,0,255),2,Qt::PenStyle(Qt::SolidLine)));//设置画笔样式
    painter.setBrush(QBrush(QColor("#55aaff")));//设置画刷样式
    painter.drawRect(rect_Oil);

}
