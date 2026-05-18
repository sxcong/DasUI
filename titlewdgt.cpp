#include "titlewdgt.h"
#include "ui_titlewdgt.h"
#include <QButtonGroup>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include "aboutdialog.h"
#include <QScreen>
//#include "globalmodule.h"
#include "informationwgt.h"

TitleWdgt::TitleWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWdgt)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_pSetMenu = new QMenu(this);
    InformationWgt *newinfo = new InformationWgt();

    QAction *pAct1 = new QAction("开始采集", this);
    QAction *pAct2 = new QAction("停止采集", this);
    QAction *pAct3 = new QAction("展示面板", this);
    QAction *pAct4 = new QAction("配置DTS设备IP", this);
    QAction *pAct5 = new QAction("关于", this);
    connect(pAct1, &QAction::triggered, [=]{});
    connect(pAct2, &QAction::triggered, [=]{});
    connect(pAct3, &QAction::triggered, [=]{
        newinfo->setWindowModality(Qt::ApplicationModal);
        newinfo->show();
    });
    connect(pAct4, &QAction::triggered, [=]{});
    connect(pAct5, &QAction::triggered, [=]{
        AboutDialog d(this);
        d.exec();
    });
    m_pSetMenu->addAction(pAct1);
    m_pSetMenu->addAction(pAct2);
    m_pSetMenu->addAction(pAct4);
    m_pSetMenu->addAction(pAct5);
    ui->btn_Menu->setMenu(m_pSetMenu);
    m_pSetMenu->installEventFilter(this);

    ui->label_logo->hide();

    QButtonGroup *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);
    navGroup->addButton(ui->tBtn_DataImport, 0);
    navGroup->addButton(ui->tBtn_Process, 1);
    navGroup->addButton(ui->tBtn_Analysis, 2);
    navGroup->addButton(ui->tBtn_Report, 3);

    ui->tBtn_DataImport->setCheckable(true);
    ui->tBtn_Process->setCheckable(true);
    ui->tBtn_Analysis->setCheckable(true);
    ui->tBtn_Report->setCheckable(true);

    connect(navGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [=](int id) {
        switch (id) {
        case 0: emit signal_tBtn_DataImport_clicked(); break;
        case 1: emit signal_tBtn_Process_clicked(); break;
        case 2: emit signal_tBtn_Analysis_clicked(); break;
        case 3: emit signal_tbtn_Report_clicked(); break;
        }
    });

    ui->tBtn_DataImport->setChecked(true);
}

//{"数据导入", "采集配置", "信号预处理", "智能监测与解释", "成果报告"};

TitleWdgt::~TitleWdgt()
{
    delete ui;
}

void TitleWdgt::adjustSize()
{
    m_WdgtWidth = QGuiApplication::primaryScreen()->availableGeometry().width();
    ui->btn_Max->show();
    ui->btn_Middle->hide();
}

void TitleWdgt::adjustMaxSize()
{ 
    m_WdgtWidth = QGuiApplication::primaryScreen()->availableGeometry().width();
//    this->setFixedWidth(m_WdgtWidth);
   /* if(nullptr != m_TitleTopWdgt)
    {
        m_TitleTopWdgt->adjustMaxSize();
        m_TitleTopWdgt->move((this->width() - m_TitleTopWdgt->width()) / 2,0);
    }*/
    ui->btn_Max->hide();
    ui->btn_Middle->show();
}

void TitleWdgt::mousePressEvent(QMouseEvent *event)
{
    emit signal_mousePressEvent(event);
//    return QWidget::mousePressEvent(event);
}

void TitleWdgt::mouseReleaseEvent(QMouseEvent *event)
{
    emit signal_mouseReleaseEvent(event);
//    return QWidget::mouseReleaseEvent(event);
}

void TitleWdgt::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signal_mouseDoubleClickEvent(event);
//    return QWidget::mouseDoubleClickEvent(event);
}

void TitleWdgt::mouseMoveEvent(QMouseEvent *event)
{
    emit signal_mouseMoveEvent(event);
//    return QWidget::mouseMoveEvent(event);
}

void TitleWdgt::on_btn_User_clicked()
{
    emit signal_btn_User_clicked();
}

void TitleWdgt::on_btn_Setting_clicked()
{
    emit signal_btn_Setting_clicked();
}

void TitleWdgt::on_btn_Min_clicked()
{
    emit signal_btn_Min_clicked();
}

void TitleWdgt::on_btn_Max_clicked()
{
    emit signal_btn_Max_clicked();
    ui->btn_Max->hide();
    ui->btn_Middle->show();
}

void TitleWdgt::on_btn_Close_clicked()
{
    emit signal_btn_Close_clicked();
}

void TitleWdgt::showEvent(QShowEvent *event)
{

}

void TitleWdgt::on_btn_Middle_clicked()
{
    emit signal_btn_Middle_clicked();
    ui->btn_Middle->hide();
    ui->btn_Max->show();
}

void TitleWdgt::on_btn_Menu_clicked()
{
    emit signal_btn_Menu_clicked();
}

bool TitleWdgt::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_pSetMenu)
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *event_mouse = static_cast<QMouseEvent*>(event);
            if (event_mouse->localPos().x() > 130 || event_mouse->localPos().y() > 100 || event_mouse->localPos().x() < 0)
                m_pSetMenu->hide();
        }
        return QObject::eventFilter(watched, event);
    }
    return QWidget::eventFilter(watched, event);
}
