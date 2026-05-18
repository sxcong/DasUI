#ifndef WELLWIDGET_H
#define WELLWIDGET_H

#include <QWidget>

namespace Ui {
class WellWidget;
}

class WellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WellWidget(QWidget *parent = nullptr);
    ~WellWidget();

private slots:
    void slot_checkBox_IntakeProfileFBE(bool bCheck);
    void slot_checkBox_InjectionProfile(bool bCheck);
    void slot_checkBox_AmplitudeCurveRaw(bool bCheck);
    void slot_checkBox_TemperatureCurve(bool bCheck);
    void slot_checkBox_AmplitudeCurve(bool bCheck);
    void slot_checkBox_PhaseEnergy(bool bCheck);
    void slot_checkBox_struct(bool bCheck);
    void slot_checkBox_TemperatureWaterfallPlot(bool bCheck);
    void slot_checkBox_AmplitudeWaterfallPlot(bool bCheck);
    void slot_checkBox_PhaseWaterfallPlot(bool bCheck);
private:
    Ui::WellWidget *ui;
};

#endif // WELLWIDGET_H
