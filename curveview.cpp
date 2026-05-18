#include "curveview.h"
#include "ui_curveview.h"
#include "axisrange.h"
CurveView::CurveView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveView)
{
    ui->setupUi(this);
    mPlot = ui->widget;

//    connect(mPlot, &QCustomPlot::mouseMove, this, &CurveView::MyMouseMoveEvent);
    curveInit();
    mPlot->setStyleSheet("QToolTip{border:1px solid rgb(118, 118, 118); color:#000000; font-size:12px;}");
    m_line = new QCPItemStraightLine(mPlot);
    //m_line->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    m_line->setPen(QPen(Qt::white, 1, Qt::SolidLine));

    //垂直参考线，就是两点一线
    //m_pHorReffer_DG->setClipToAxisRect(false);//裁剪，让外部也要看到
    m_line->setClipAxisRect(mPlot->axisRect());//设置裁剪的坐标轴
//    m_line->point1->setAxes(mPlot->yAxis,mPlot->xAxis);//绑定坐标
//    m_line->point2->setAxes(mPlot->yAxis,mPlot->xAxis);
    m_line->point1->setCoords(-10,  10);//设置点1的坐标
    m_line->point2->setCoords(-20,  10);//设置点2的坐标

//    m_axisRange = new AxisRange();
//    m_axisRange->hide();

    if(m_Traser == nullptr)
    {
        m_Traser = new PointTracer(mPlot, mPlot->graph(0), DataTracer);
    }
    QCPItemText * text2 = new QCPItemText(mPlot);
    text2->setText("(m)");
    text2->setLayer("main");
    text2->setClipToAxisRect(false);
    text2->setPadding(QMargins(0, 0, 0,0));
    text2->position->setType(QCPItemPosition::ptViewportRatio);
    text2->position->setCoords(0.05,0.99);
    text2->setFont(QFont("微软雅黑", 8));
    text2->setColor(Qt::white);

    m_line->setVisible(false);
}

CurveView::~CurveView()
{
    delete ui;
}
void CurveView::curveInit()
{
    //设置坐标轴颜色
    mPlot->yAxis->setBasePen(QPen(QColor("#FFFFFF"), 1));
    mPlot->xAxis->setBasePen(QPen(QColor("#FFFFFF"), 1));
    mPlot->yAxis->setTickPen(QPen(QColor("#FFFFFF"), 1));
    mPlot->xAxis->setTickPen(QPen(QColor("#FFFFFF"), 1));
    mPlot->yAxis->setSubTickPen(QPen(QColor("#FFFFFF"), 1));
    mPlot->xAxis->setSubTickPen(QPen(QColor("#FFFFFF"), 1));
    mPlot->yAxis->setTickLabelColor(QColor("#FFFFFF"));
    mPlot->xAxis->setTickLabelColor(QColor("#FFFFFF"));
    mPlot->xAxis->setLabelColor(QColor("#FFFFFF"));
    mPlot->yAxis->setLabelColor(QColor("#FFFFFF"));

    //    设置画布背景色
    //    QLinearGradient plotGradient;

    //    plotGradient.setStart(0, 0);

    //    plotGradient.setFinalStop(0, 350);

    //    plotGradient.setColorAt(0, QColor(80, 80, 80));

    //    plotGradient.setColorAt(1, QColor(50, 50, 50));
    mPlot->setBackground(QBrush(QColor("#323E4C")));



    //设置坐标背景色

    //    QLinearGradient axisRectGradient;

    //    axisRectGradient.setStart(0, 0);

    //    axisRectGradient.setFinalStop(0, 350);

    //    axisRectGradient.setColorAt(0, QColor(80, 80, 80));

    //    axisRectGradient.setColorAt(1, QColor(30, 30, 30));

//    mPlot->axisRect()->setBackground(QColor("#141B29"));
    mPlot->axisRect()->setBackground(QColor("#242F3A"));

    //设置网格线
    mPlot->yAxis->grid()->setPen(QPen(QColor("#314575"), 1, Qt::SolidLine));     // 网格线(对应刻度)画笔
    mPlot->xAxis->grid()->setPen(QPen(QColor("#314575"), 1, Qt::SolidLine));
    //    mPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // 子网格线(对应子刻度)画笔
    //    mPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
//        mPlot->yAxis->grid()->setSubGridVisible(true);     // 显示子网格线
//        mPlot->xAxis->grid()->setSubGridVisible(true);
        mPlot->xAxis->grid()->setVisible(true);
        mPlot->yAxis->grid()->setVisible(true);
    //    mPlot->yAxis->grid()->setZeroLinePen(QPen(Qt::blue));   // 设置刻度为0时的网格线的画笔
    //    mPlot->xAxis->grid()->setZeroLinePen(QPen(Qt::blue));

    //设置图例
    mPlot->legend->setVisible(false);
    mPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QFont legendFont = font();
    legendFont.setPointSize(10);
    mPlot->legend->setFont(legendFont);
    mPlot->legend->setIconSize(0, 0);
    mPlot->legend->setSelectedFont(legendFont);
    mPlot->legend->setSelectableParts(QCPLegend::spLegendBox);
    mPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    //设置标题
    //    mPlot->plotLayout()->insertRow(0);
    //    mPlot->plotLayout()->addElement(0, 0, new QCPTextElement(mPlot, "QCustomPlot Demo", QFont("sans", 12, QFont::Bold)));

    //设置鼠标样式
    mPlot->setCursor(QCursor(Qt::PointingHandCursor));
    //可拖拽+可滚轮缩放
    mPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iMultiSelect);

    // create graphs:
    mGraph1 = mPlot->addGraph(mPlot->yAxis, mPlot->xAxis);
    mGraph1->setPen(QPen(Qt::green));
    mGraph1->setName("温度曲线图");

    //设置曲线样式
    mPlot->graph()->setLineStyle(QCPGraph::lsLine);
    //    mPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));

    //设置坐标轴
    //    mPlot->yAxis->setLabel("光纤长度");
    //    mPlot->xAxis->setLabel("值");

//    mPlot->xAxis2->setVisible(true);
//    mPlot->yAxis2->setVisible(true);

    mPlot->yAxis->setTickLength(5,0);
    mPlot->yAxis->setSubTickLength(0,0);


    mPlot->yAxis->setRange(m_x1,m_x2);
    mPlot->xAxis->setRange(m_y1,m_y2);

    mPlot->xAxis->setSubTicks(false);
    mPlot->xAxis->ticker()->setTickCount(3);

    mPlot->yAxis->ticker()->setTickCount(10);

    mPlot->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));//设置选框的样式：虚线
    mPlot->selectionRect()->setBrush(QBrush(QColor(255,255,255,50)));//设置选框的样式：半透明浅蓝

    mPlot->yAxis->setRangeReversed(true);
    //设置拖拽
    mPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    //设置鼠标滚轮缩放比例
    mPlot->axisRect()->setRangeZoomFactor(1,1.5);//设置缩放比例
    //    mPlot->axisRect(0)->setRangeZoomFactor(0.2,0.2);

    //设置右键菜单
    mPlot->setContextMenuPolicy(Qt::CustomContextMenu);

    //    setupSimpleItemDemo(mPlot);
    //信号连接槽函数
//    connect(mPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(paintbigpoint(QMouseEvent *)));
     connect(mPlot, &QCustomPlot::mouseMove, this, &CurveView::MyMouseMoveEvent);
    connect(mPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    //    connect(mPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
//    connect(mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
//    connect(mPlot, &QCustomPlot::axisDoubleClick, this, &CurveView::slot_axisDoubleClick);

    connect(mPlot->yAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(xAxisChanged(QCPRange,QCPRange)));
    connect(mPlot->xAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(yAxisChanged(QCPRange,QCPRange)));
}

void CurveView::addData1(double keys, double values)
{
    mGraph1->addData(keys,values);
}


void CurveView::updateLine1(const QVector<double> &keys, const QVector<double> &values)
{
    mGraph1->setData(keys,values);
    mPlot->replot();
}

void CurveView::updateLine(const QList<QPointF> &list)
{
    QVector<double> keys;
    QVector<double> values;
    for(int i = 0;i< list.size();i++)
    {
        keys.append(list[i].x());
        values.append(list[i].y());
    }

    if(nullptr != mGraph1)
    {
        mGraph1->setData(keys,values);
    }
    else
    {
        qDebug()<<"dcdc nullptr == mGraph1";
    }

    if(nullptr != mPlot)
    {
        mPlot->replot();
    }
    else
    {
        qDebug()<<"dcdc nullptr == mPlot";
    }
}

void CurveView::updateLine(const QVector<QPointF> &list)
{
    updateLine(list.toList());
}

void CurveView::setRangeX(const int nMin, const int nMax)
{
    m_x1 = nMin;
    m_x2 = nMax;
    mPlot->xAxis->setRange(nMin,nMax);
    mPlot->replot();
}

void CurveView::setRangeY(const double nMin, const double nMax)
{
    m_y1 = nMin;
    m_y2 = nMax;
    mPlot->yAxis->setRange(nMin,nMax);
    mPlot->replot();
}

void CurveView::rescaleAllline()
{
    mPlot->rescaleAxes();
    mPlot->replot();
}

void CurveView::clearAllline()
{
    if(nullptr != mGraph1)
    {
        mGraph1.data()->data().clear();
    }
}

void CurveView::resetAxes()
{
    setRangeX(m_x1,m_x2);
    setRangeY(m_y1,m_y2);
}

void CurveView::MyMouseMoveEvent(QMouseEvent *event)
{

    // 当前鼠标位置（像素坐标）
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    // 像素坐标转成实际的x,y轴的坐标
    double x_val = mPlot->xAxis->pixelToCoord(x_pos);//横坐标
    double y_val = mPlot->yAxis->pixelToCoord(y_pos);//纵坐标

    //通过坐标轴范围判断光标是否在点附近
    double x_begin = mPlot->xAxis->range().lower;
    double x_end = mPlot->xAxis->range().upper;
    double y_begin = mPlot->yAxis->range().lower;
    double y_end = mPlot->yAxis->range().upper;
    double x_tolerate = (x_end - x_begin) / 40;//光标与最近点距离在此范围内，便显示该最近点的值
    double y_tolerate = (y_end - y_begin) / 40;

    //判断有没有超出坐标轴范围
//    qDebug()<<x_val<<y_val;
//    if (x_val < x_begin || x_val > x_end||y_val< y_begin||y_val > y_end)
//    {
//        return;
//    }


    //通过x值查找离曲线最近的一个key值索引
    int index = 0;
    int index_left = mPlot->graph(0)->findBegin(y_val, true);//左边最近的一个key值索引
    int index_right = mPlot->graph(0)->findEnd(y_val, true);//右边
    float dif_left = fabs(mPlot->graph(0)->data()->at(index_left)->key - y_val);
    float dif_right = fabs(mPlot->graph(0)->data()->at(index_right)->key - y_val);
    index = ((dif_left < dif_right) ? index_left : index_right);


    double x_posval = mPlot->graph(0)->data()->at(index)->key;//纵坐标，此时绑定的坐标轴纵坐标为key，横坐标为value
    double y_posval = mPlot->graph(0)->data()->at(index)->value;//横坐标
#if Test
//    qDebug()<<dy << y_tolerate;
    //判断光标点与最近点的距离是否在设定范围内
    if( x_val < x_begin || x_val > x_end||y_val< y_begin||y_val > y_end )
    {
       m_Traser->setVisible(false);
    }
    else
    {
//        qDebug()<<index<<x_posval<<y_posval;
        m_Traser->setVisible(true);
        m_Traser->updatePosition(y_posval,x_posval);

        if(m_type == 0)
        {
            m_Traser->setText2(QString("深度:%1m\n温度:%2℃").
                               arg(QString::number(x_posval, 'f', 0)).
                               arg(QString::number(y_posval, 'f', 2)));
        }
        else {
            m_Traser->setText2(QString("深度:%1\n强度:%2").
                               arg(QString::number(x_posval, 'f', 0)).
                               arg(QString::number(y_posval, 'f', 2)));
        }


    }
    mPlot->replot();
#endif


    float dx = fabs(x_posval - y_val);
//    float dy = fabs(y_posval - y_val);
//    qDebug()<<"cc"<<x_posval<<y_posval<<x_val<< y_val;
    if (dx <= 2)
    {
        m_line->point1->setCoords(y_posval, x_posval);
        m_line->point2->setCoords(y_posval - 1000.0, x_posval);


        m_line->setVisible(true);
        m_Traser->setVisible(true);
        m_Traser->updatePosition(y_posval,x_posval);
        if(m_type == 0)
        {
            m_Traser->setText2(QString("深度:%1m\n温度:%2℃").
                               arg(QString::number(x_posval, 'f', 0)).
                               arg(QString::number(y_posval, 'f', 2)));
        }
        else if(m_type == 1){
            m_Traser->setText2(QString("深度:%1\n强度:%2").
                               arg(QString::number(x_posval, 'f', 0)).
                               arg(QString::number(y_posval, 'f', 2)));
        }
        else if(m_type == 2)
        {
            m_Traser->setText2(QString("深度:%1\n值:%2").
                               arg(QString::number(x_posval, 'f', 0)).
                               arg(QString::number(y_posval, 'f', 2)));
        }
    }
    else
    {
        m_line->setVisible(false);
        m_Traser->setVisible(false);
    }
    mPlot->replot();

}

void CurveView::contextMenuRequest(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;	//创建一个QMenu对象
    //给menu添加三个选项
    QAction * act1 = menu.addAction("自适应范围");
    QAction * act2 =menu.addAction("框选");
    QAction * act3 = menu.addAction("恢复");

    connect(act1, &QAction::triggered,[=]
    {
        mPlot->yAxis->rescale();//设置外部坐标
        mPlot->replot();
    });
    connect(act2, &QAction::triggered,[=]
    {
//        if(mPlot->selectionRectMode() == QCP::SelectionRectMode::srmNone)
//        {
//            mPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
//        }
//        else {
            mPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
//        }

    });
    connect(act3, &QAction::triggered,[=]
    {
        resetAxes();
    });
    //将menu显示在鼠标当前位置
    menu.exec(QCursor::pos());

}
void CurveView::selectionChanged()
{
    /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

    // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (mPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            mPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        mPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        mPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (mPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            mPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || mPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        mPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        mPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i=0; i<mPlot->graphCount(); ++i)
    {
        QCPGraph *graph = mPlot->graph(i);
        QCPPlottableLegendItem *item = mPlot->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}

void CurveView::xAxisChanged(const QCPRange &newRange, const QCPRange &oldRange)
{
    if(newRange.lower < 0 )
    {
        mPlot->yAxis->setRangeLower(0);
    }
    if(newRange.upper > 14336 )
    {
        mPlot->yAxis->setRangeUpper(14336);
    }
    emit signal_CurveXRangeChange(mPlot->yAxis->range().lower,mPlot->yAxis->range().upper);
//    else
//    {
//        if(nullptr != mPlot)
//        {
//            mPlot->yAxis->setRange(oldRange.lower,oldRange.upper);
//            mPlot->replot();
//        }
//    }
}

void CurveView::yAxisChanged(const QCPRange &newRange, const QCPRange &oldRange)
{
    if(newRange.lower < -100000000 || newRange.upper > 100000000)
    {
        if(nullptr != mPlot)
        {
            mPlot->xAxis->setRange(oldRange.lower,oldRange.upper);
            mPlot->replot();
        }
    }
}

void CurveView::paintbigpoint(QMouseEvent *event)
{
    QPointF movepoit = event->pos();
    //排除label区域以外的点
    if(!mPlot->viewport().contains(event->pos()))
    {
        return;
    }
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    // 像素坐标转成实际的x,y轴的坐标
    double x_val = mPlot->xAxis->pixelToCoord(x_pos);
    float y_val = mPlot->yAxis->pixelToCoord(y_pos);
        // 获取鼠标经过的横轴的坐标值并取整数
    double datavalue = 0;/*m_pColorMap->dataMainValue(currentx);*/
    /*画出鼠标移动时对应在线上的点*/
    QString tipstext = "x:"+QDateTime::fromSecsSinceEpoch(x_val).toString("hh:mm:ss")+"y:"+QString::number(y_val);
    QToolTip::showText(mapToGlobal(event->pos()),tipstext/*QString("data：%1").arg(datavalue)*/,this);
}

void CurveView::slot_axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
{
    // 当前鼠标位置（像素坐标）
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    // 像素坐标转成实际的x,y轴的坐标
//    float x_val = mPlot->xAxis->pixelToCoord(x_pos);
//    float y_val = mPlot->yAxis->pixelToCoord(y_pos);
    m_axisRange->setGeometry(x_pos,y_pos,100,50);


    if (m_axisRange->exec()== QDialog::Accepted)
    {
        int min = 0,max = 1000;
        m_axisRange->getValue(min,max);
        if(axis->axisType() == QCPAxis::atLeft)
        {
            mPlot->yAxis->setRange(min,max);
            mPlot->replot();
            return;
        }
        if(axis->axisType() == QCPAxis::atBottom)
        {
            mPlot->xAxis->setRange(min,max);
            mPlot->replot();
            return;
        }

    } else
    {
       // do something else
    }


}
void CurveView::setupSimpleItemDemo(QCustomPlot *customPlot)
{
    // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(customPlot);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabel->setText("10000,10000点在这里");
    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text

    // add the arrow:
    QCPItemLine *arrow = new QCPItemLine(customPlot);
    arrow->start->setParentAnchor(textLabel->bottom);
    arrow->end->setCoords(10000, 10000); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

void CurveView::createNewLine()
{
    QPen pen;
    pen.setWidth(5);  //线宽（可以自己设置喜欢的）
    pen.setColor(Qt::red);  //线颜色（可以自己设置喜欢的）
    pen.setStyle(Qt::PenStyle::SolidLine);   //线的类型或风格（可以自己设置喜欢的）

    QCPGraph * newLine = mPlot->addGraph();   //创建一个新图形
    newLine->setPen(pen);   //把笔给它用于绘画

    QVector<double> x(2), y(2), x1(5), y1(5);

    x[0] = 0;
    y[0] = 1000;
    x[1] = 10000;
    y[1] = 1000;
    newLine->setData(x, y); //为绘图提供数据
}
void CurveView::updateSymbolicLines(QList<QList<QPointF> > &linePoints, int alarmType)
{
    QPen pen;
    pen.setWidth(1);  //线宽（可以自己设置喜欢的）
    pen.setStyle(Qt::PenStyle::SolidLine);   //线的类型或风格（可以自己设置喜欢的）
    QColor myColor = QColor(Qt::yellow);
    if(0 == alarmType)
    {
        myColor = QColor(Qt::yellow);
    }
    else if(1 == alarmType)
    {
        myColor = QColor(250,128,10);
    }
    else
    {
        myColor = QColor(Qt::red);
    }
    pen.setColor(myColor);  //线颜色（可以自己设置喜欢的）
    for(int i = 0;i < linePoints.length();++i)
    {
        QCPGraph * newLine = mPlot->addGraph();   //创建一个新图形
        if(0 == i)
        {
            if(0 == alarmType)
            {
                newLine->setName(QObject::tr("Pre-Alarm"));
            }
            else if(1 == alarmType)
            {
                newLine->setName(QObject::tr("Alarm"));
            }
            else
            {
                newLine->setName(QObject::tr("Serious"));
            }
        }

        newLine->setPen(pen);   //把笔给它用于绘画
        QVector<double> keys;
        QVector<double> values;
        for(int j = 0;j< linePoints[i].size();j++)
        {
            keys.append(linePoints[i][j].x());
            values.append(linePoints[i][j].y());
        }
        newLine->setData(keys,values);
    }

}
void CurveView::updateThreshold(int channel)
{
//    QList<QList<QPointF> > linePoints;
//    AlarmConfig config;
//    AlarmConfigMgr::Instance().getConfig(channel, config);
//    config.getHighThresholdLinePoints(linePoints);

//    qDebug()<<linePoints;
//    clearThreshold();
//    updateSymbolicLines(linePoints,0);

//    int size_partition = config.m_PartitionVec.size();

//    QList<QList<QPointF> > preAlarmLines;
//    QList<QList<QPointF> > alarmLines;
//    QList<QList<QPointF> > shutDownLines;
//    for(int i = 0;i < size_partition;i++)
//    {
//        AlarmPartitionItem tmp = config.m_PartitionVec[i];
//        QList<QPointF> positive1;
//        QPointF fsPoint = QPointF(tmp.m_fiberStart,tmp.m_ThirdLevel);
//        positive1.append(fsPoint);
//        QPointF fePoint = QPointF(tmp.m_fiberEnd,tmp.m_ThirdLevel);
//        positive1.append(fePoint);
//        preAlarmLines.append(positive1);

//        QList<QPointF> positive2;
//        QPointF ssPoint = QPointF(tmp.m_fiberStart,tmp.m_SecondLevel);
//        positive2.append(ssPoint);
//        QPointF sePoint = QPointF(tmp.m_fiberEnd,tmp.m_SecondLevel);
//        positive2.append(sePoint);
//        alarmLines.append(positive2);

//        QList<QPointF> positive3;
//        QPointF tsPoint = QPointF(tmp.m_fiberStart,tmp.m_FirstLevel);
//        positive3.append(tsPoint);
//        QPointF tePoint = QPointF(tmp.m_fiberEnd,tmp.m_FirstLevel);
//        positive3.append(tePoint);
//        shutDownLines.append(positive3);

//    }
//    clearThreshold();
//    qDebug()<<preAlarmLines.size()<<alarmLines.size()<<shutDownLines.size();
//    updateSymbolicLines(preAlarmLines,0);
//    updateSymbolicLines(alarmLines,1);
//    updateSymbolicLines(shutDownLines,2);
}

void CurveView::clearThreshold()
{
    int c = mPlot->graphCount();
    for (int i=c-1; i > 0; --i)
      mPlot->removeGraph(i);
    mPlot->replot();
}
