#ifndef CHARTSELECTWIDGET_H
#define CHARTSELECTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

namespace Ui {
class ChartSelectWidget;
}

class ChartSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartSelectWidget(QWidget *parent = nullptr);
    ~ChartSelectWidget();
    void initEvent();
signals:
    void signal_checkBox_IntakeProfileFBE(bool bCheck);
    void signal_checkBox_InjectionProfile(bool bCheck);
    void signal_checkBox_AmplitudeCurveRaw(bool bCheck);
    void signal_checkBox_TemperatureCurve(bool bCheck);
    void signal_checkBox_AmplitudeCurve(bool bCheck);
    void signal_checkBox_PhaseEnergy(bool bCheck);
    void signal_checkBox_struct(bool bCheck);
    void signal_checkBox_TemperatureWaterfallPlot(bool bCheck);
    void signal_checkBox_AmplitudeWaterfallPlot(bool bCheck);
    void signal_checkBox_PhaseWaterfallPlot(bool bCheck);

//Intake Profile     吸水剖面
//Injection Profile  注水剖面
//Flow Profile：产液剖面
//注水井叫 Injection Profile，生产井叫 Production/Flow Profile
protected:
    void ChartSelectWidget::paintEvent(QPaintEvent *event) override;

private:


private:
    Ui::ChartSelectWidget *ui;
};

#endif // CHARTSELECTWIDGET_H
