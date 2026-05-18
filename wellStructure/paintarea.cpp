#include "paintarea.h"
#include "QCoreApplication"
#include "QDebug"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setPalette(QPalette(Qt::transparent));
    setAutoFillBackground(true);
//    setFixedSize(300,600);
    {

        QString filename = ":/res/struct/perforate.png";
        if(! ( m_Img_perforate.load(filename) ) ) //加载图像
        {
            return;
        }

    }
    {
        QString filename = ":/res/struct/equipment.png";
        if(! ( m_equipment.load(filename) ) ) //加载图像
        {
            return;
        }

    }
    {
        QString filename = ":/res/struct/ground.png";
        if(! ( m_ground.load(filename) ) ) //加载图像
        {
            return;
        }

    }


}

void PaintArea::setShape(PaintArea::Shape shape,QPen pen,QBrush brush,int width,int height) {

    m_shape=shape;
    if(shape == cannula)
    {
        m_cannula_w = width ;
//        m_cannula_h = height*m_scale;
        m_pen = pen;
        m_brush = brush;

    }else if(shape == oil)
    {
        m_Oil_w = width;
        m_Oil_h= height*m_scale;
        m_pen_oil = pen;
        m_brush_oil = brush;
    }
    else if(shape == perforate)
    {
        m_perforate_w = width;
        m_perforate_h= height*m_scale;
        m_pen_perforate = pen;
        m_brush_perforate = brush;
    }
    else if(shape == cement)
    {
        m_cement_w = width;
        m_cement_h= height*m_scale;
        m_pen_cement = pen;
        m_brush_cement = brush;
        this->setFixedHeight(m_cement_h+90);
    }

    update();

}

void PaintArea::setPen(QPen pen) {
    m_pen=pen;
    update();
}

void PaintArea::setBrush(QBrush brush) {
    m_brush=brush;
    update();
}

void PaintArea::setFillRule(Qt::FillRule rule) {
    m_fillrule=rule;
    update();
}
void PaintArea::paintEvent(QPaintEvent *event) {


    QPainter painter(this);

    int rect_w = this->rect().width();
    int rect_h = this->rect().height();

    int rect_ceter_w = rect_w/2;
    int rect_ceter_h = rect_h/2;


    QRect rect_equipment(0,0,400,80);
    painter.drawImage(rect_equipment,m_equipment,m_equipment.rect());

    QRect rect_ground(0,80,400,10);//左上宽高
    painter.drawImage(rect_ground,m_ground,m_ground.rect());

    QRect rect_cannula_3((rect_w-m_cannula_w_3)/2,90,m_cannula_w_3,m_cannula_h_3);
    painter.setPen(m_pen);//设置画笔样式
    painter.setBrush(m_brush);//设置画刷样式
    painter.drawRect(rect_cannula_3);



    QRect rect_cannula_2((rect_w-m_cannula_w_2)/2,90,m_cannula_w_2,m_cannula_h_2);
    painter.setPen(m_pen);//设置画笔样式
    painter.setBrush(m_brush);//设置画刷样式
    painter.drawRect(rect_cannula_2);

    QRect rect_cannula((rect_w-m_cannula_w)/2,90,m_cannula_w,m_cannula_h);
    painter.setPen(m_pen);//设置画笔样式
    painter.setBrush(m_brush);//设置画刷样式
    painter.drawRect(rect_cannula);


    QRect rect_perforate((rect_w-m_cement_w)/2-50,m_perforate_h+rect_cannula.y()-30,50,30);
    painter.drawImage(rect_perforate,m_Img_perforate,m_Img_perforate.rect());


    QImage m_Img_perforate_2= m_Img_perforate.mirrored(true,false);
    QRect rect_perforate_2((rect_w+m_cement_w)/2,m_perforate_h+rect_cannula.y()-30,50,30);
    painter.drawImage(rect_perforate_2,m_Img_perforate_2,m_Img_perforate_2.rect());

//    QRect rect_perforate((rect_w-m_perforate_w)/2,rect_cannula.y()+m_Oil_h,m_perforate_w,-m_perforate_h/2);
//    painter.setPen(m_pen_perforate);//设置画笔样式
//    painter.setBrush(m_brush_perforate);//设置画刷样式
//    painter.drawRect(rect_perforate);

//    QRect rect_perforate_2((rect_w-m_perforate_w)/2,rect_cannula.y()+m_Oil_h-m_perforate_h/2-10,m_perforate_w,-m_perforate_h/2);
//    painter.setPen(m_pen_perforate);//设置画笔样式
//    painter.setBrush(m_brush_perforate);//设置画刷样式
//    painter.drawRect(rect_perforate_2);

    QRect rect_cement((rect_w-m_cement_w)/2,rect_cannula.y(),m_cement_w,m_cement_h);
    painter.setPen(m_pen_cement);//设置画笔样式
    painter.setBrush(m_brush_cement);//设置画刷样式
    painter.drawRect(rect_cement);


    QRect rect_Oil((rect_w-m_Oil_w)/2,rect_cannula.y(),m_Oil_w,m_Oil_h);
    painter.setPen(m_pen_oil);//设置画笔样式
    painter.setBrush(m_brush_oil);//设置画刷样式
    painter.drawRect(rect_Oil);

}

void PaintArea::setCannulaH(int h1, int h2, int h3)
{
    m_cannula_h  = h1*m_scale;
    m_cannula_h_2 = h2*m_scale;
    m_cannula_h_3 = h3*m_scale;
}
