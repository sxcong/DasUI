#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QStatusBar>
#include <QMenuBar>
#include <QSplitter>
#include <QPushButton>
#include <QStackedWidget>
#include "wellwidget.h"

class TitleWdgt;
class StatusWidget;
class FileTreeListWidget;
class MainWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFrame* createCard(const QString &title, const QString &content, const QString &color) ;
private slots:
    void slot_btn_Min_clicked();
    void slot_btn_Middle_clicked();
    void slot_btn_Max_clicked();
    void slot_btn_Close_clicked();
    void slot_mousePressEvent(QMouseEvent *event);
    void slot_mouseReleaseEvent(QMouseEvent *event);
    void slot_mouseDoubleClickEvent(QMouseEvent *event);
    void slot_mouseMoveEvent(QMouseEvent *event);

    void slot_tBtn_DataImport_clicked();
    void slot_tBtn_Process_clicked();
    void slot_tBtn_Analysis_clicked();
    void slot_tbtn_Report_clicked();

protected:
    virtual void showEvent(QShowEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;
    virtual bool eventFilter(QObject *target, QEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
private:
    void loadStyle(const QString &qssFile);
private:
    Ui::MainWindow *ui;

    bool m_bDragFlag = false;

    bool m_bLeftButtonPressed = false;
    QPoint m_currentPos;//屏幕坐标
    QPoint m_currentWidgetPos;//窗体坐标
    bool m_IsClickMax = false;//用于记录主窗体是否是在最大化状态下点击的左键
    bool m_IsMax = false;//用于记录当前是否是最大化状态


    //子窗口管理
    //右侧窗口
    TitleWdgt *m_TitleWdgt = nullptr;      //最上
    MainWidget* m_pMainWidget = nullptr;   //中间
    StatusWidget* m_StatusWidget = nullptr;//下边状态

    //左侧导航条
    QToolBar* activityBar = nullptr;
    bool isExpanded = false;
    QAction *actToggle;
    QAction *actWorkspace;
    QAction *actMonitor;
    QAction *actReport;


};

#endif // MAINWINDOW_H
