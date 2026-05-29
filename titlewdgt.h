#ifndef TITLEWDGT_H
#define TITLEWDGT_H

//#include "titletopwdgt.h"

#include <QMenu>
#include <QWidget>

namespace Ui {
class TitleWdgt;
}

class TitleWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWdgt(QWidget *parent = nullptr);
    ~TitleWdgt();

    void adjustSize();

    void adjustMaxSize();

signals:
    void signal_DTS_Connect();
    void signal_btn_User_clicked();
    void signal_btn_Setting_clicked();
    void signal_btn_Min_clicked();
    void signal_btn_Middle_clicked();
    void signal_btn_Max_clicked();
    void signal_btn_Close_clicked();


    void signal_tbtn_FFtData_clicked();
    void signal_mousePressEvent(QMouseEvent *event);
    void signal_mouseReleaseEvent(QMouseEvent *event);
    void signal_mouseDoubleClickEvent(QMouseEvent *event);
    void signal_mouseMoveEvent(QMouseEvent *event);
    void signal_btn_Menu_clicked();

    void signal_tBtn_DataImport_clicked();
    void signal_tBtn_Process_clicked();
    void signal_tBtn_Analysis_clicked();
    void signal_tbtn_Report_clicked();
    void signal_btn_OpenLfDas_clicked();
    void signal_btn_OpenFbeEnergy_clicked();
    void signal_btn_OpenDownsample_clicked();
    void signal_btn_OpenSpectrumDb_clicked();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_btn_User_clicked();
    void on_btn_Setting_clicked();
    void on_btn_Min_clicked();
    void on_btn_Max_clicked();
    void on_btn_Close_clicked();
    void on_btn_Middle_clicked();
    void on_btn_Menu_clicked();

private:
    Ui::TitleWdgt *ui;
    int m_WdgtWidth = 1920;
    //TitleTopWdgt *m_TitleTopWdgt = nullptr;

    QMenu *m_pSetMenu = nullptr;
    // QWidget interface
protected:
    void showEvent(QShowEvent *event);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif // TITLEWDGT_H
