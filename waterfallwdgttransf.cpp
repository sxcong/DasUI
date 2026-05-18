#include "waterfallwdgttransf.h"
//#include "globalmodule.h"

WaterFallWdgtTransf::WaterFallWdgtTransf(QWidget *parent) : QWidget(parent)
{
    fp3 = new QCustomPlot(this);

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    fp3->setBackground(QBrush(QColor("#323E4C")));
    verticalLayout->addWidget(fp3);
    verticalLayout->setContentsMargins(0,0,0,0);
}

void WaterFallWdgtTransf::waterFallInit()
{
    fp3->yAxis->setBasePen(QPen(Qt::white, 1));
    fp3->xAxis->setBasePen(QPen(Qt::white, 1));
    fp3->yAxis->setTickPen(QPen(Qt::white, 1));
    fp3->xAxis->setTickPen(QPen(Qt::white, 1));
    fp3->yAxis->setSubTickPen(QPen(Qt::white, 1));
    fp3->xAxis->setSubTickPen(QPen(Qt::white, 1));
    fp3->yAxis->setTickLabelColor(Qt::white);
    fp3->xAxis->setTickLabelColor(Qt::white);
    fp3->yAxis->setLabelColor(Qt::white);
    fp3->xAxis->setLabelColor(Qt::white);

//    QLinearGradient plotGradient;
//    plotGradient.setColorAt(0,QColor(255,255,255,0));

//    fp3->setBackground(QBrush(QColor(34, 41, 62)));

//    fp3->plotLayout()->insertRow(0);
//    QCPTextElement * text = new QCPTextElement(fp3, "DAS", QFont("sans", 12, QFont::Bold));
//    text->setTextColor(Qt::white);
//    fp3->plotLayout()->addElement(0, 0, text);

//    QCPTextElement * text2 = new QCPTextElement(fp3, "幅值", QFont("sans", 12, QFont::Bold));
//    text2->setTextColor(Qt::white);
//    fp3->plotLayout()->addElement(0, 1, text2);

//    fp3->plotLayout()->insertColumn(0);
//    QCPTextElement * text = new QCPTextElement(fp3, "DAS", QFont("sans", 12, QFont::Bold));
//    text->setTextColor(Qt::white);
//    fp3->plotLayout()->addElement(0, 0, text);

//    QCPTextElement * text2 = new QCPTextElement(fp3, "幅值    ", QFont("微软雅黑", 12, QFont::Normal));
//    text2->setTextColor(Qt::white);
//    text2->setMargins(QMargins(0,0,15,0));
//    fp3->plotLayout()->addElement(0, 1, text2);

//    if(m_type == 1)
//    {
//        fp3->yAxis->setLabel("DAS Plot\n\n测量深度(m)");
//    }
//    else if(m_type == 2)
//    {
//        fp3->yAxis->setLabel("DTS Plot\n\n测量深度(m)");
//        QCPTextElement * widget2 = dynamic_cast<QCPTextElement*>(fp3->plotLayout()->element(0, 1));
//        widget2->setText("温度  ");
//    }
//    else if(m_type == 3)
//    {
//        fp3->yAxis->setLabel("DAS Plot\n\n测量深度(m)");
//        QCPTextElement * widget2 = dynamic_cast<QCPTextElement*>(fp3->plotLayout()->element(0, 1));
//        widget2->setText("相位  ");
//    }


    QFont font;
//    font.setPixelSize(21);
    font.setFamily(QStringLiteral("微软雅黑"));
//    font.setBold(true);
    fp3->xAxis->setTickLabelFont(font);
    //设置坐标轴
    fp3->yAxis->setLabelFont(font);
//    fp3->yAxis->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×
//    fp3->yAxis->setNumberPrecision(2);//精度1

    fp3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);//可拖拽+可滚轮缩放
    fp3->axisRect()->setRangeZoomFactor(1,1.5);//设置缩放比例
    fp3->axisRect()->setRangeDrag(nullptr);//禁用拖拽

    fp3->yAxis->setRange(m_Start,m_End);//设置外部坐标
    fp3->yAxis->setVisible(true);

    uint currTimet = QDateTime::currentDateTime().toTime_t();
    fp3->xAxis->setRange(QCPRange(currTimet,currTimet + m_TimeInterval));

    fp3->yAxis->setTickLength(0,0);
    fp3->yAxis->setSubTickLength(0,0);


//    fp3->legend->setVisible(true);
//    fp3->legend->setBrush(QColor(255, 255, 255, 150));
//    QFont legendFont = this->font();
//    legendFont.setPointSize(10);
//    fp3->legend->setFont(legendFont);
//    fp3->legend->setIconSize(0, 0);
//    fp3->legend->setSelectedFont(legendFont);
//    fp3->legend->setSelectableParts(QCPLegend::spLegendBox);
//    fp3->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);


    m_pColorMap = new QCPColorMap(fp3->xAxis,fp3->yAxis);
    m_pColorMap->data()->setSize(m_XCount,m_Length);//设置整个图（x,y）点数
    m_pColorMap->data()->setRange(QCPRange(currTimet,currTimet + m_TimeInterval),QCPRange(m_Start,m_End));//设置画布，设置X轴以及Y轴的范围

    m_pColorMap->setName(QString("温度瀑布图"));
    if(nullptr == m_pColorScale)
    {
        m_pColorScale = new QCPColorScale(fp3);
        m_pColorScale->setDataRange(QCPRange(0,50));//百分比
        m_pColorScale->setType(QCPAxis::atBottom);
//        m_pColorScale->axis()->setOffset(-18);


        m_pColorScale->axis()->axisRect()->axis(QCPAxis::atRight)->setBasePen(QPen(Qt::white, 2));
        m_pColorScale->axis()->axisRect()->axis(QCPAxis::atLeft)->setBasePen(QPen(Qt::white, 2));
        m_pColorScale->axis()->axisRect()->axis(QCPAxis::atTop)->setBasePen(QPen(Qt::white, 2));

        m_pColorScale->axis()->setVisible(true);
        m_pColorScale->axis()->setTickLabelSide(QCPAxis::lsInside);
        m_pColorScale->axis()->setTickLabelPadding(-1);
//        m_pColorScale->axis()->setTickLength(0,5);
//        m_pColorScale->axis()->setSubTickLength(0,2);
        m_pColorScale->axis()->setTickLength(5,0);
        m_pColorScale->axis()->setSubTickLength(0,0);
        m_pColorScale->axis()->setBasePen(QPen(Qt::white, 2));
        m_pColorScale->axis()->setTickPen(QPen(Qt::transparent, 1));
        m_pColorScale->axis()->setSubTickPen(QPen(Qt::transparent, 1));
        m_pColorScale->axis()->setTickLabelColor(QColor("#777777"));
        m_pColorScale->axis()->setLabelColor(Qt::transparent);

        QFont font;
    //    font.setPixelSize(21);
        font.setFamily(QStringLiteral("黑体"));
        font.setBold(true);
    //    font.setBold(true);
//        font.setStyle(Qt::SolidLine);//轮廓线为实线
        m_pColorScale->axis()->setTickLabelFont(font);

        m_pColorScale->axis()->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×
        m_pColorScale->axis()->setNumberPrecision(2);//精度1
        m_pColorScale->axis()->ticker()->setTickCount(4);
        m_pColorScale->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
//        m_pColorScale->setMargins(QMargins(0,15,0,55));

//        QCPMarginGroup *marginGroup = new QCPMarginGroup(fp3);
//        fp3->axisRect()->setMarginGroup(QCP::msLeft|QCP::msRight, marginGroup);
//        m_pColorScale->setMarginGroup(QCP::msLeft|QCP::msRight, marginGroup);


//        fp3->plotLayout()->insertRow(0);
        m_pColorScale->setRangeDrag(true);
        m_pColorScale->setRangeZoom(true);

        fp3->axisRect()->insetLayout()->addElement(m_pColorScale,QRectF(0,0.95,1,1));

//        fp3->plotLayout()->addElement(1,0,m_pColorScale);
        m_pColorMap->setColorScale(m_pColorScale);
        connect(m_pColorScale,&QCPColorScale::dataRangeChanged,this,&WaterFallWdgtTransf::CPColorRangeChange);

    }

//    QCPItemPixmap df(fp3);
//    fp3->plotLayout()->addElement(df);
    m_pColorMap->setGradient(QCPColorGradient::gpJet);//设置默认渐进色变化（可在QCPColorGradient中查看）

    //设置现在最新时间
//    QSharedPointer<QCPAxisTickerDateTime> timer(new QCPAxisTickerDateTime());
    m_Timer = QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime());
    m_Timer->setTickCount(3);//设置间隔
    m_Timer->setDateTimeFormat("hh:mm:ss");
    m_Timer->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);

    fp3->xAxis->setTicker(m_Timer);//绑定时间轴
    fp3->xAxis->setTickLabelRotation(0);

    fp3->yAxis->ticker()->setTickCount(10);
    fp3->yAxis->setSubTicks(true);//次刻度
    fp3->yAxis->setRangeReversed(true);
//    fp3->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);

    fp3->xAxis->setSubTicks(false);
//    fp3->xAxis->setTickLengthOut(5);//设置坐标轴外侧长度
//    fp3->yAxis->setTickLengthOut(5);
    fp3->xAxis->setRangeReversed(true);
//    fp3->xAxis->setNumberPrecision(1);
    //设置右键菜单
    fp3->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fp3->yAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(yAxisChanged(QCPRange,QCPRange)));
//    setupSimpleItemDemo(mPlot);
    //信号连接槽函数
    //不显示点值
//    connect(fp3,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(paintbigpoint(QMouseEvent *)));
    connect(fp3, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
//    fp3->xAxis2->setVisible(true);
//    fp3->yAxis2->setVisible(true);
    connect(fp3, &QCustomPlot::axisDoubleClick,[=](QCPAxis *axis)
    {
        if(axis == m_pColorScale->axis()||axis ==  m_pColorScale->axis()->axisRect()->axis(QCPAxis::atTop))
        {
            if(!m_istopColorscale)
            {
                fp3->axisRect()->insetLayout()->setInsetRect(1,QRectF(0,0,1,1));
                m_istopColorscale = true;
            }
            else {
                fp3->axisRect()->insetLayout()->setInsetRect(1,QRectF(0,0.95,1,1));
                m_istopColorscale = false;
            }
//            qDebug()<<"changed";
            fp3->replot();
        }
    });
//    fp3->axisRect(0)->setVisible(true);
    //创建一个QMenu对象
    //给menu添加三个选项
    QAction * act1 = menu.addAction("自适应范围");
    QAction * act2 = menu.addAction("框选");
    QAction * act3 = menu.addAction("恢复");

    connect(act1, &QAction::triggered,[=]
    {
        IsUpdate = true;
        fp3->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
        fp3->yAxis->setRange(0,m_ActualLength);//设置外部坐标
        this->setRangeX(m_Xmin,m_Xmax);
        fp3->replot();
    });
    connect(act2, &QAction::triggered,[=]
    {
        fp3->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);   
    });
    connect(act3, &QAction::triggered,[=]
    {
       fp3->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
       this->setRangeY(m_Ymin,m_Ymax);
       this->setRangeX(m_Xmin,m_Xmax);
       IsUpdate = true;
       fp3->replot();
    });

    connect(fp3->selectionRect(),&QCPSelectionRect::accepted,[=](){
        IsUpdate = false;
    } );

    reset();
}

void WaterFallWdgtTransf::waterFallShow(const QVector<double> &data)
{
    m_ActualLength = data_interval * data.size();
    if(m_ActualLength > 14000)
    { 
        static bool IsQuit = true;
        if(IsQuit)
        {
            IsQuit = false;
            QMessageBox::warning(nullptr, "警告", QString("<font color='red'>测量长度超过14km，不支持,程序退出"), QString("确定"));
            qApp->exit();
            return ;
        }
        else {
            return;
        }

    }
    QMutexLocker locker(&m_mutex);
//    qDebug()<<QDateTime::currentDateTime();

    if(value_lofar.size() >= m_XCount)
    {
        value_lofar.removeFirst();
        value_lofar.push_back(data);
    }
    else
    {
        value_lofar.push_back(data);
    }
    if (IsUpdate)
    {
        uint currTimet = QDateTime::currentDateTime().toTime_t();
        if (value_lofar.size() == 1)
        {
            fp3->xAxis->setRange(currTimet - m_TimeInterval, currTimet);
            m_pColorMap->data()->setRange(QCPRange(currTimet - m_TimeInterval, currTimet), QCPRange(m_Start, m_End));
        }
        else if (value_lofar.size() == m_XCount)
        {
            m_Xmin = currTimet - m_TimeInterval;
            m_Xmax = currTimet;
            m_Timer->setTickOrigin(currTimet);
            fp3->xAxis->setRange(currTimet - m_TimeInterval, currTimet);
            m_pColorMap->data()->setRange(QCPRange(currTimet - m_TimeInterval, currTimet), QCPRange(m_Start, m_End));
        }

        QCPColorMapData *mapData = m_pColorMap->data();
        int n = value_lofar.size();
        int di = data_interval;
        for (int i = 0; i < n; i++)
        {
            const QVector<double> &src = value_lofar[i];
            int currLen = src.size();
            if (currLen * di > m_Length)
                continue;
            int j = 0;
            for (int s = 0; s < currLen; s++)
            {
                double val = src[s];
                int end = j + di;
                for (; j < end; j++)
                    mapData->setCell(i, j, val);
            }
        }
        fp3->replot(QCustomPlot::rpQueuedReplot);
    }

}

void WaterFallWdgtTransf::reset()
{
    QMutexLocker locker(&m_mutex);
    value_lofar.clear();
    QVector<double> data(m_Length,0);
    value_lofar.fill(data,m_XCount);
    for (int i=0;i< m_XCount ;i++)
    {
        int currLength = m_Length;
        for(int j=0;j<currLength;j++)
        {
            m_pColorMap->data()->setCell(i,j,0);
        }
    }
    fp3->replot();
}

void WaterFallWdgtTransf::setRangeColor(int minBlue, int maxRed)
{
    if(nullptr != m_pColorScale)
    {
        m_pColorScale->setDataRange(QCPRange(minBlue,maxRed));//百分比
    }
}

void WaterFallWdgtTransf::setType(int type)
{
    m_type = type;

}

void WaterFallWdgtTransf::setCptext(QString text)
{
    QCPTextElement * widget2 = dynamic_cast<QCPTextElement*>(fp3->plotLayout()->element(0, 1));
    widget2->setText(text);
}

void WaterFallWdgtTransf::setRangeTime(int sec)
{
    QMutexLocker locker(&m_mutex);
    m_TimeInterval = sec;
    if(m_type == 1)
    {
        if(sec >3600)
        {
            m_XCount = sec /20;
        }
        else if(sec > 60 && sec <= 3600)
        {
            m_XCount = sec /3;
        }
        else if(sec >0&& sec <= 60)
        {
            m_XCount = sec * 3;
        }
//        else if(sec >0&& sec <= 30)
//        {
//            m_XCount = sec *20;
//        }

    }
    else if(m_type == 2)
    {
        if(sec == 43200)
        {
            m_XCount = 720;
//            m_DataTimeInterval = 60;
        }
        else if(sec == 21600)
        {
            m_XCount = 720;
//            m_DataTimeInterval = 30;
        }
        else if(sec== 3600)
        {
            m_XCount = 720;
//            m_DataTimeInterval = 5;
        }
    }
    else if(m_type == 3)
    {
        if(sec > 3600)
        {
            m_XCount = sec / 20;
        }
        else if(sec > 60 && sec <= 3600)
        {
            m_XCount = sec / 3;
        }
        else if(sec >0&& sec <=60)
        {
            m_XCount = sec * 3;
        }
    }

    uint currTimet = QDateTime::currentDateTime().toTime_t();
    m_Timer->setTickOrigin(currTimet);
    fp3->xAxis->setRange(currTimet,currTimet - m_TimeInterval);//设置外部坐标
    m_pColorMap->data()->setRange(QCPRange(currTimet,currTimet - m_TimeInterval),QCPRange(m_Start,m_End));
    m_pColorMap->data()->setSize(m_XCount,m_Length);
}

void WaterFallWdgtTransf::yAxisChanged(const QCPRange &newRange,const QCPRange &oldRange)
{
    if(newRange.lower < 0 )
    {
        fp3->yAxis->setRangeLower(0);
    }
    if(newRange.upper > 14336 )
    {
        fp3->yAxis->setRangeUpper(14336);
    }

    if(newRange.upper-newRange.lower <= 1)
    {
        fp3->yAxis->ticker()->setTickCount(2);
    }
    else if(newRange.upper-newRange.lower <10)
    {
        fp3->yAxis->ticker()->setTickCount(5);
    }
    else {
        fp3->yAxis->ticker()->setTickCount(10);
    }
    if(newRange.upper < 100)
    {
        fp3->yAxis->setPadding(20);
    }
    else if(newRange.upper < 1000&&newRange.upper >= 100)
    {
        fp3->yAxis->setPadding(15);
    }
    else if(newRange.upper >= 1000&&newRange.upper< 10000)
    {
        fp3->yAxis->setPadding(8);
    }
    else if(newRange.upper >= 10000&&newRange.upper< 20000)
    {
        fp3->yAxis->setPadding(1);
    }
    fp3->replot();
    emit signal_alignRangeY(newRange.lower,newRange.upper);
}

void WaterFallWdgtTransf::CPColorRangeChange(const QCPRange &newRange)
{
//    if(newRange.upper < 5)
//    {
//       m_pColorScale->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
//       m_pColorScale->axis()->ticker()->setTickCount(2);
//       m_pColorScale->setMargins(QMargins(0,15,41,55));
//       return;
//    }
//    else if(newRange.upper < 10)
//    {
//       m_pColorScale->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
//       m_pColorScale->setMargins(QMargins(0,15,41,55));
//    }
//    else if(newRange.upper < 100&&newRange.upper >=10 )
//    {
//        m_pColorScale->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
//       m_pColorScale->setMargins(QMargins(0,15,36,55));
//    }
//    else if(newRange.upper >= 100)
//    {
//       m_pColorScale->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
//       m_pColorScale->setMargins(QMargins(0,15,0,55));
//    }
//    m_pColorScale->axis()->ticker()->setTickCount(6);
//    if(newRange.lower < 0)
//    {
//        m_pColorScale->axis()->setRangeLower(0);
//    }
//    if(newRange.upper < 1)
//    {
//        m_pColorScale->axis()->setRangeUpper(1);
//    }
}
void WaterFallWdgtTransf::setRangeX(int min, int max)
{
    if(nullptr != fp3 && nullptr != fp3->xAxis)
    {
        fp3->xAxis->setRange(min ,max);//设置外部坐标
    }
}
void WaterFallWdgtTransf::setRangeY(int min, int max)
{
    if(nullptr != fp3 && nullptr != fp3->yAxis)
    {
        m_Ymin = min;
        m_Ymax = max;
        fp3->yAxis->setRange(min,max);//设置外部坐标
    }
}
void WaterFallWdgtTransf::contextMenuRequest(const QPoint &pos)
{
    Q_UNUSED(pos);

    //将menu显示在鼠标当前位置
    menu.exec(QCursor::pos());

}
void WaterFallWdgtTransf::setDataInterval(double interval)
{
    data_interval = interval;
    qDebug()<<"setDataInterval"<<data_interval;
}
void WaterFallWdgtTransf::paintbigpoint(QMouseEvent *event)
{
    QPointF movepoit = event->pos();
    //排除label区域以外的点
    if(!fp3->viewport().contains(event->pos()))
    {
        return;
    }
    int x_pos = event->pos().x();
    int y_pos = event->pos().y();

    // 像素坐标转成实际的x,y轴的坐标
    double x_val = fp3->xAxis->pixelToCoord(x_pos);
    float y_val = fp3->yAxis->pixelToCoord(y_pos);
        // 获取鼠标经过的横轴的坐标值并取整数
    double datavalue = m_pColorMap->data()->data(x_val,y_val);/*m_pColorMap->dataMainValue(currentx);*/
    /*画出鼠标移动时对应在线上的点*/
    QString tipstext = "x:"+QDateTime::fromSecsSinceEpoch(x_val).toString("hh:mm:ss")+" y:"+QString::number(y_val)+" z:"+QString::number(datavalue);
    QToolTip::showText(mapToGlobal(event->pos()),tipstext/*QString("data：%1").arg(datavalue)*/,this);
//    ui->label->graph(2)->data().data()->clear();
//    ui->label->graph(2)->addData(currentx,datavalue);
//    ui->label->replot();
////    qDebug()<<"center:->>>"<<ui->label->xAxis->range().center();
//    /*右边数值小箭头*/
//    mTag1->setPen(ui->label->graph(0)->pen());  //设置画笔
//    mTag1->updatePosition(datavalue);
//    mTag1->setText(QString::number(datavalue,'f',2));
}

