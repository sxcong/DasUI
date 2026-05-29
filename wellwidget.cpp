#include "wellwidget.h"
#include "ui_wellwidget.h"
#include "chartselectwidget.h"
#include "SpectrumAnalysisWidget.h"

WellWidget::WellWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WellWidget)
{
    ui->setupUi(this);

    ChartSelectWidget *chartSelect = ui->widget_chart_select;
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_IntakeProfileFBE, this, &WellWidget::slot_checkBox_IntakeProfileFBE);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_InjectionProfile, this, &WellWidget::slot_checkBox_InjectionProfile);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_AmplitudeCurveRaw, this, &WellWidget::slot_checkBox_AmplitudeCurveRaw);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_TemperatureCurve, this, &WellWidget::slot_checkBox_TemperatureCurve);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_AmplitudeCurve, this, &WellWidget::slot_checkBox_AmplitudeCurve);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_PhaseEnergy, this, &WellWidget::slot_checkBox_PhaseEnergy);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_struct, this, &WellWidget::slot_checkBox_struct);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_TemperatureWaterfallPlot, this, &WellWidget::slot_checkBox_TemperatureWaterfallPlot);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_AmplitudeWaterfallPlot, this, &WellWidget::slot_checkBox_AmplitudeWaterfallPlot);
    connect(chartSelect, &ChartSelectWidget::signal_checkBox_PhaseWaterfallPlot, this, &WellWidget::slot_checkBox_PhaseWaterfallPlot);

    chartSelect->initEvent();

    QSplitter *vSplitter = new QSplitter(Qt::Vertical);
    QWidget *bottomContainer;
    bottomContainer = new QWidget();
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomContainer);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    auto createStatusBox = [](const QString &name) {
        QFrame *box = new QFrame();
        box->setStyleSheet("background-color: #0a0a0a; border: 1px solid #333;");
        QVBoxLayout *l = new QVBoxLayout(box);
        QLabel *title = new QLabel(name);
        title->setStyleSheet("color: #888; border: none;");
        l->addWidget(title);
        l->addStretch();
        return box;
    };
    bottomLayout->addWidget(createStatusBox("F-K 域速度分析"));
    bottomLayout->addWidget(createStatusBox("实时通道频谱"));
    // 特殊处理最后一个“系统状态”框
    QFrame *sysStatus = createStatusBox("系统状态");
    QLabel *sysLog = new QLabel("> 引擎状态: 运行中\n> GPU 渲染: 已开启", sysStatus);
    sysLog->setStyleSheet("color: #00ff00; font-family: 'Consolas'; border: none;");
    sysStatus->layout()->addWidget(sysLog);
    bottomLayout->addWidget(sysStatus);

    vSplitter->addWidget(ui->widget);
    vSplitter->addWidget(bottomContainer);
    vSplitter->setSizes(QList<int>() << 900 << 300);

    ui->verticalLayout_5->addWidget(vSplitter);
}

WellWidget::~WellWidget()
{
    delete ui;
}

void WellWidget::slot_checkBox_IntakeProfileFBE(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title9->show();
        ui->widget_FBE_curve->show();

    }
    else
    {
        ui->widget_title9->hide();
        ui->widget_FBE_curve->hide();
    }
}

void WellWidget::slot_checkBox_InjectionProfile(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title8->show();
        ui->widget_waterInject->show();
    }
    else
    {
        ui->widget_title8->hide();
        ui->widget_waterInject->hide();
    }
}

void WellWidget::slot_checkBox_AmplitudeCurveRaw(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_rawAmCurve->show();
        //ui->widget_title7->show();
        //ui->widget_DAS_rawAmCurve->show();
    }
    else
    {
        ui->widget_rawAmCurve->hide();
        //ui->widget_title7->hide();
        //ui->widget_DAS_rawAmCurve->hide();
    }
}

void WellWidget::slot_checkBox_TemperatureCurve(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title1->show();
        ui->widget_DTS_curve->show();
    }
    else
    {
        ui->widget_title1->hide();
        ui->widget_DTS_curve->hide();
    }
}

void WellWidget::slot_checkBox_AmplitudeCurve(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title3->show();
        ui->widget_DAS_curve->show();
    }
    else
    {
        ui->widget_title3->hide();
        ui->widget_DAS_curve->hide();
    }
}
//相位能量
void WellWidget::slot_checkBox_PhaseEnergy(bool bCheck)
{
    /*if(bCheck)
    {
        ui->widget_title3->show();
        ui->widget_DAS_curve->show();
    }
    else
    {
        ui->widget_title3->hide();
        ui->widget_DAS_curve->hide();
    }*/
}

void WellWidget::slot_checkBox_struct(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_struct->show();
    }
    else
    {
        ui->widget_struct->hide();
    }
}

void WellWidget::slot_checkBox_TemperatureWaterfallPlot(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title2->show();
        ui->widget_DTS->show();
    }
    else
    {
        ui->widget_title2->hide();
        ui->widget_DTS->hide();
    }
}

void WellWidget::slot_checkBox_AmplitudeWaterfallPlot(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title4->show();
        ui->widget_DAS->show();
    }
    else
    {
        ui->widget_title4->hide();
        ui->widget_DAS->hide();
    }
}

void WellWidget::slot_checkBox_PhaseWaterfallPlot(bool bCheck)
{
    if(bCheck)
    {
        ui->widget_title5->show();
        ui->widget_DAS_Phase->show();
    }
    else
    {
        ui->widget_title5->hide();
        ui->widget_DAS_Phase->hide();
    }
}
