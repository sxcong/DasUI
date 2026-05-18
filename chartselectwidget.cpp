#include "chartselectwidget.h"
#include "ui_chartselectwidget.h"
#include <QStyleOption>
#include <QDebug>

ChartSelectWidget::ChartSelectWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartSelectWidget)
{
    ui->setupUi(this);
   // initEvent();
    ui->checkBox_PhaseEnergy->hide();
}

ChartSelectWidget::~ChartSelectWidget()
{
    delete ui;
}

void ChartSelectWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 2. 创建画家对象，指定绘图设备为当前 widget
    QPainter painter(this);

    // 3. 开启反锯齿（可选，如果画圆角边框建议开启）
    painter.setRenderHint(QPainter::Antialiasing);

    // 4. 设置画笔（颜色、宽度、线条样式）
    QPen pen;
    pen.setColor(QColor(68,68,68));
    pen.setWidth(2);            // 边框宽度
    pen.setStyle(Qt::SolidLine); // 实线
    painter.setPen(pen);

    // 5. 绘制矩形边框
    // 注意：绘制时为了防止线条粗细导致边缘被切掉，通常将矩形收缩 1 像素
    QRect rect = this->rect();
    painter.drawRect(rect.adjusted(0, 0, -1, -1)); // 矩形边框

    // 如果想要圆角边框，用这个：
    //painter.drawRoundedRect(rect.adjusted(1, 1, -1, -1), 5, 5);
}

void ChartSelectWidget::initEvent()
{
    connect(ui->checkBox_IntakeProfileFBE, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_IntakeProfileFBE(checked);
    });

    connect(ui->checkBox_InjectionProfile, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_InjectionProfile(checked);
    });

    connect(ui->checkBox_AmplitudeCurveRaw, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_AmplitudeCurveRaw(checked);
    });

    connect(ui->checkBox_TemperatureCurve, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_TemperatureCurve(checked);
    });

    connect(ui->checkBox_AmplitudeCurve, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_AmplitudeCurve(checked);
    });

    connect(ui->checkBox_PhaseEnergy, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_PhaseEnergy(checked);
    });

    connect(ui->checkBox_struct, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_struct(checked);
    });

    connect(ui->checkBox_TemperatureWaterfallPlot, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_TemperatureWaterfallPlot(checked);
    });

    connect(ui->checkBox_AmplitudeWaterfallPlot, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_AmplitudeWaterfallPlot(checked);
    });

    connect(ui->checkBox_PhaseWaterfallPlot, &QCheckBox::toggled, this, [=](bool checked) {
        emit signal_checkBox_PhaseWaterfallPlot(checked);
    });

    ui->checkBox_struct->setChecked(true);
    ui->checkBox_TemperatureCurve->setChecked(true);
    ui->checkBox_TemperatureWaterfallPlot->setChecked(true);
    ui->checkBox_AmplitudeCurveRaw->setChecked(true);
    ui->checkBox_AmplitudeCurve->setChecked(true);
    ui->checkBox_AmplitudeWaterfallPlot->setChecked(true);
    ui->checkBox_PhaseWaterfallPlot->setChecked(true);
    ui->checkBox_IntakeProfileFBE->setChecked(true);
    ui->checkBox_InjectionProfile->setChecked(true);

    //触发 toggled 事件
    ui->checkBox_TemperatureCurve->setChecked(false);
    ui->checkBox_TemperatureWaterfallPlot->setChecked(false);
    ui->checkBox_IntakeProfileFBE->setChecked(false);
    ui->checkBox_InjectionProfile->setChecked(false);


   // ui->checkBox_PhaseEnergy->setChecked(true);
}

