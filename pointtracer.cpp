#include "pointtracer.h"

PointTracer::PointTracer(QCustomPlot *_plot, QCPGraph *_graph, TracerType _type) : plot(_plot),
    graph(_graph),
    type(_type),
    visible(false)

{
    if (plot)
    {
        tracer = new QCPItemTracer(plot);
        tracer->setStyle(QCPItemTracer::tsPlus);//可以选择设置追踪光标的样式，这个是小十字，还有大十字，圆点等样式
        tracer->setPen(QPen(Qt::green));//设置tracer的颜色绿色
        //tracer->setPen(graph->pen());//设置tracer的颜色跟曲线
        tracer->setBrush(QPen(Qt::green).color());
        tracer->setSize(6);

        label = new QCPItemText(plot);
        label->setLayer("overlay");
        label->setClipToAxisRect(false);
        label->setPadding(QMargins(15, 15, 15 ,15));
        label->position->setParentAnchor(tracer->position);
        label->setFont(QFont("宋体", 12));

        arrow = new QCPItemLine(plot);
        arrow->setLayer("overlay");
//        arrow->setPen(graph->pen());//设置箭头的颜色跟随曲线
        arrow->setPen(QPen(QColor(21,246,255,255)));//设置箭头的颜色红色
        arrow->setClipToAxisRect(false);
        arrow->setHead(QCPLineEnding::esSpikeArrow);

        switch (type) {
        case XAxisTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
            label->setBrush(QBrush(QColor(244, 244, 244, 100)));
            label->setPen(QPen(Qt::black));

            label->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(20, 0);//偏移量
            break;
        }
        case YAxisTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

            label->setBrush(QBrush(QColor(244, 244, 244, 100)));
            label->setPen(QPen(Qt::black));
            label->setPositionAlignment(Qt::AlignRight|Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(label->position);
            arrow->start->setCoords(-20, 0);//偏移量
            break;
        }
        case DataTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);


            label->setBrush(QBrush(QColor(244, 244, 244, 150)));
//            label->setPen(graph->pen());//边框跟随曲线颜色
//            QPen rect_pen;
//            rect_pen.setColor(QColor(21,246,238,150));
//            rect_pen.setWidth(50);
            label->setPen(QPen(QBrush(QColor(21,246,255,255)),1));//边框红色
            label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            label->setTextAlignment(Qt::AlignLeft);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(50, -50);
            break;
        }
        case CustomerTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

            label->setBrush(QBrush(QColor(244, 244, 244, 150)));
            //label->setPen(graph->pen());//边框跟随曲线颜色
            label->setPen(QPen(QBrush(QColor(21,246,255,255)),1));//边框红色
            label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            label->setTextAlignment(Qt::AlignLeft);
            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(50, -50);

            break;
        }
        default:
            break;
        }

        setVisible(false);
    }
}

PointTracer::~PointTracer()
{
    if (tracer)
        plot->removeItem(tracer);
    if (label)
        plot->removeItem(label);
    if (arrow)
        plot->removeItem(arrow);
}

void PointTracer::setPen(const QPen &pen)
{
    tracer->setPen(pen);
    arrow->setPen(pen);
}

void PointTracer::setBrush(const QBrush &brush)
{
    tracer->setBrush(brush);
}

void PointTracer::setLabelPen(const QPen &pen)
{
    label->setPen(pen);
}

void PointTracer::setText(const QString &text,const QString &text1)
{
    label->setText(tr("深度:%1\n温度:%2").arg(text).arg(text1));
}


void PointTracer::setText2(const QString &text)
{
    label->setText(text);
}
void PointTracer::setVisible(bool visible)
{
    tracer->setVisible(visible);
    label->setVisible(visible);
    arrow->setVisible(visible);
}

void PointTracer::updatePosition(double xValue, double yValue)
{
    if (!visible)
    {
        setVisible(true);
        visible = true;
    }
//    if (yValue > plot->yAxis->range().upper)
//        yValue = plot->yAxis->range().upper;
    switch (type) {
    case XAxisTracer:
    {
        tracer->position->setCoords(xValue, 1);
        label->position->setCoords(0, 15);
        arrow->start->setCoords(0, 15);
        arrow->end->setCoords(0, 0);

        break;
    }
    case YAxisTracer:
    {
        tracer->position->setCoords(1, yValue);
        label->position->setCoords(-20, 0);
        break;
    }
    case DataTracer:
    {
//        qDebug()<<"cc"<<__LINE__<< xValue<< yValue;
        tracer->position->setCoords( xValue, yValue );
//        qDebug()<<"cc"<<__LINE__<< xValue << yValue;
        double x_pos = tracer->position->keyAxis()->coordToPixel(xValue);
        double y_pos = tracer->position->valueAxis()->coordToPixel(yValue);
        int x_value = 50;
        int yValue = -50;
        if(x_pos +180 > plot->viewport().topRight().rx())
        {
            x_value = plot->viewport().topRight().rx()- x_pos - 130;
//            qDebug()<<"cc  hahha"<<x_pos<<plot->viewport().topRight().rx();
        }
        if(y_pos - 80 < 0)
        {
            yValue = 30 - y_pos;
        }

        arrow->start->setCoords(x_value, yValue);
        label->position->setCoords(x_value, yValue);


        break;
    }
    case CustomerTracer:
    {
        tracer->position->setCoords( xValue, yValue );

        double x_pos = tracer->position->keyAxis()->coordToPixel(xValue);
        double y_pos = tracer->position->valueAxis()->coordToPixel(yValue);
        int x_offset_value = 0;
        int y_offset_value = 50;

        if(x_pos +180 > plot->viewport().topRight().rx())
        {
            x_offset_value = plot->viewport().topRight().rx()- x_pos - 130;
        }
        if(y_pos - 80 < 0)
        {
            y_offset_value = 50 - y_pos;
        }

        arrow->start->setCoords(x_offset_value, y_offset_value);
        label->position->setCoords(x_offset_value, y_offset_value);
        arrow->setVisible(false);
        tracer->setVisible(false);
        break;
    }
    default:
        break;
    }
}
