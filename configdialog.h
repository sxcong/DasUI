#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QButtonGroup>
#include <QDialog>
#include <QTableWidget>
//#include "globalmodule.h"
#include "QHeaderView"
#include "defines.h"
namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    static ConfigDialog& instance();

    ~ConfigDialog();
    void loadConfig();
    void getAxisY(int &min, int &max);
    void getColor(int &minBlue, int &maxRed);
    void getAxisYDTS(int &min, int &max);
    void getColorDTS(int &minBlue, int &maxRed);
    void getXtime(int &time);
    void getXtimeDTS(int &time);
    void getColorPhase(int &minBlue, int &maxRed);
    void getTempYDTS(int &min, int &max);

    void setColor(int minBlue,int maxRed,int p_minBlue,int p_maxRed);
    int  getDasDeductLength();
    void setDasDeductLength(int length);

    void saveToFile(int type,const WaterFallConfig &config);


    void getAxisXDas(int &min, int &max);
    void getAxisXPhase(int &min, int &max);

    void initTableWidget(QTableWidget* pTable);
signals:
    void signal_ParamsChanged();
    void signal_ParamsChangedDTS();
    void signal_TimeChanged(bool IsDas);

    void signal_FFTTimeChanged(int length);
    void signal_paramsFFTChanged(int type,const WaterFallConfig &config);
    void signal_ImagChanged();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool m_bLeftButtonPressed = false;
    QPoint m_currentPos;//屏幕坐标
    QPoint m_currentWidgetPos;//窗体坐标
private slots:
    void on_tBtn_Close_clicked();
    void on_btn_Save_2_clicked();
    void on_btn_Save_clicked();
    void on_btn_Save_3_clicked();
    void on_btn_Save_4_clicked();
    void slot_buttonToggled(int id, bool checked);

    void on_btn_Insert_clicked();

    void on_btn_Delete_clicked();
    void on_btn_save_clicked();

private:
    Ui::ConfigDialog *ui;
    QButtonGroup * buttonGroup = nullptr;
    explicit ConfigDialog(QWidget *parent = nullptr);
};

#endif // CONFIGDIALOG_H
