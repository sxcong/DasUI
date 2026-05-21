#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QScreen>
#include <QDebug>
#include <QElapsedTimer>
#include <QToolBar>
#include "titlewdgt.h"
#include "statuswidget.h"
#include "filetreelistwidget.h"
#include "leftnavbar.h"
#include "mainwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化成员变量，防止未定义行为
    m_TitleWdgt = nullptr;
    m_IsMax = false;
    m_bLeftButtonPressed = false;

    ui->menubar->hide();
    ui->statusbar->hide();

    loadStyle(":/qss/blacksoft.css");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);


    m_pMainWidget = new MainWidget(this);
    ui->verticalLayout->addWidget(m_pMainWidget);

    m_StatusWidget = new StatusWidget(this);
    ui->verticalLayout->addWidget(m_StatusWidget);
    m_StatusWidget->setMaximumHeight(30);

   // m_stackedWidget->setCurrentIndex(0);
    // 让 QStackedWidget 及其直接子 widget 背景透明
   // m_stackedWidget->setStyleSheet("QStackedWidget, QStackedWidget > QWidget { background-color: transparent; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStyle(const QString &qssFile)
{
    QElapsedTimer time;
    time.start();

    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QString::fromUtf8(file.readAll());
        file.close();

        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(paletteColor));
        qApp->setStyleSheet(qss);
    }

    qDebug() << "loadStyle用时:" << time.elapsed();
}
// 辅助函数：创建一个深色面板卡片
QFrame* MainWindow::createCard(const QString &title, const QString &content, const QString &color) {
    QFrame *card = new QFrame();
    card->setStyleSheet("QFrame { border: 1px solid #333; padding: 10px; background-color: #121212; }");
    QVBoxLayout *layout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #5dade2; font-weight: bold; border: none;");

    QLabel *contentLabel = new QLabel(content);
    contentLabel->setWordWrap(true);
    contentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    //contentLabel->setFixedWidth(120);  // 限制宽度以演示换行效果
    contentLabel->setStyleSheet(QString("color: %1; font-size: 20px; font-weight: bold; border: none;").arg(color));
    Q_UNUSED(content);

    layout->addWidget(titleLabel);
    layout->addWidget(contentLabel);
    return card;
}


void MainWindow::showEvent(QShowEvent *event)
{
    if(nullptr == m_TitleWdgt)
    {
        m_TitleWdgt = new TitleWdgt(this);
        connect(m_TitleWdgt,&TitleWdgt::signal_btn_Min_clicked,this,&MainWindow::slot_btn_Min_clicked);
       // connect(ui->buttonPageSurveyNext,&QPushButton::clicked,[=](){}
        connect(m_TitleWdgt,&TitleWdgt::signal_btn_Middle_clicked,this,&MainWindow::slot_btn_Middle_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_btn_Max_clicked,this,&MainWindow::slot_btn_Max_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_btn_Close_clicked,this,&MainWindow::slot_btn_Close_clicked);

        connect(m_TitleWdgt,&TitleWdgt::signal_mousePressEvent,this,&MainWindow::slot_mousePressEvent);
        connect(m_TitleWdgt,&TitleWdgt::signal_mouseReleaseEvent,this,&MainWindow::slot_mouseReleaseEvent);
        connect(m_TitleWdgt,&TitleWdgt::signal_mouseDoubleClickEvent,this,&MainWindow::slot_mouseDoubleClickEvent);
        connect(m_TitleWdgt,&TitleWdgt::signal_mouseMoveEvent,this,&MainWindow::slot_mouseMoveEvent);

        connect(m_TitleWdgt,&TitleWdgt::signal_tBtn_DataImport_clicked,this,&MainWindow::slot_tBtn_DataImport_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tBtn_Process_clicked,this,&MainWindow::slot_tBtn_Process_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tBtn_Analysis_clicked,this,&MainWindow::slot_tBtn_Analysis_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tbtn_Report_clicked,this,&MainWindow::slot_tbtn_Report_clicked);

/*      connect(m_TitleWdgt,&TitleWdgt::signal_tBtn_DataAnalysis_clicked,this,&MainWindow::slot_tBtn_DataAnalysis_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tBtn_Alarmconfig_clicked,this,&MainWindow::slot_tBtn_Alarmconfig_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tbtn_FFtData_clicked,this,&MainWindow::slot_tbtn_FFtData_clicked);
        connect(m_TitleWdgt,&TitleWdgt::signal_tbtn_History_clicked,this,&MainWindow::slot_tbtn_History_clicked);

        connect(m_TitleWdgt,&TitleWdgt::signal_DTS_Connect,this,&MainWindow::slot_DTS_Connect);


*/
        ui->horizontalLayout->addWidget(m_TitleWdgt);
        m_TitleWdgt->show();
    }

    if(m_IsMax == true)
    {
        slot_btn_Max_clicked();
        qDebug()<<"max";
    }
    else
    {

        slot_btn_Middle_clicked();
        qDebug()<<"midlle";
    }
}


void MainWindow::timerEvent(QTimerEvent *event)
{

}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    QMouseEvent *mouse = dynamic_cast<QMouseEvent *>(event);

    //拖动
    if (mouse)
    {
        if (mouse->button() == Qt::LeftButton && mouse->type() == QEvent::MouseButtonPress)
        {
            m_bDragFlag = true;
            /*if (target == m_box_lightconfig)
            {
                m_pointDrag = mouse->globalPos();
                m_windowTopLeftPoint = m_box_lightconfig->frameGeometry().topLeft();
                if(m_lightConfig!= nullptr)
                {
                    m_lightConfig->show();
                }
            }*/

        }
        else if (mouse->type() == QEvent::MouseButtonRelease)
        {
            m_bDragFlag = false;
        }
        else if (m_bDragFlag &&  mouse->type() == QEvent::MouseMove)
        {
           // QPoint distance = mouse->globalPos() - m_pointDrag;
           // m_box_lightconfig->move(m_windowTopLeftPoint+distance);
        }
    }

    return QWidget::eventFilter(target, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        return;
    QMainWindow::keyPressEvent(event);
}


void MainWindow::slot_btn_Min_clicked()
{
    showMinimized();
}
void MainWindow::slot_btn_Middle_clicked()
{

    if(nullptr != m_TitleWdgt)
    {
        m_TitleWdgt->adjustSize();
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    resize(screenRect.width() * 7 / 10, screenRect.height() * 7 / 10);
    move(screenRect.width() * 1 / 10, screenRect.height() * 1 / 10);

    //    ui->label_structure->resize(330,510);
   /* m_width = ui->label_structure->width();
    m_height = ui->label_structure->height();
    QPixmap fitpixmap=QPixmap::fromImage(*img).scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_structure->setImage(fitpixmap.toImage());
    ui->label_structure->setBoxHeightSize(0,0);
    m_scale = ui->label_structure->height()*1.0/img->size().height();
    */
    m_IsMax = false;
    return;
}
void MainWindow::slot_btn_Max_clicked()
{
    this->setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    m_IsMax = true;

    if (m_TitleWdgt)
        m_TitleWdgt->adjustMaxSize();

}

void MainWindow::slot_btn_Close_clicked()
{
    close();
}

void MainWindow::slot_mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(m_IsMax)
        {
            m_IsClickMax = true;
            m_currentWidgetPos = (event->pos());
            m_currentPos = (event->pos());
        }
        else
        {
            m_IsClickMax = false;
            m_currentWidgetPos = (event->pos());
            m_currentPos = (event->pos());
        }

        if(event->pos().y() < 60)
        {
            m_bLeftButtonPressed = true;
        }
    }

    MainWindow::mousePressEvent(event);
}

void MainWindow::slot_mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftButtonPressed = false;
    }

    MainWindow::mouseReleaseEvent(event);
}

void MainWindow::slot_mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->pos().y() < 60)
    {
        //slot_btn_Max_clicked();
        m_IsMax = !m_IsMax;
        if(m_IsMax == true)
        {
            slot_btn_Max_clicked();
            qDebug()<<"max";
        }
        else
        {

            slot_btn_Middle_clicked();
            qDebug()<<"midlle";
        }
    }
}

void MainWindow::slot_mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if (m_bLeftButtonPressed)
        {
            if(m_IsMax)
            {
                //                if(nullptr != m_TitleWdgt)
                //                {
                //                    m_TitleWdgt->adjustSize();
                //                }

                //                int desktop_width = QApplication::desktop()->width();
                //                int dv = m_currentPos.x() * m_RESOLUTION_WIDTH / desktop_width;
                //                resize(m_RESOLUTION_WIDTH,m_RESOLUTION_HEIGHT);
                //                move(m_currentPos.x() - dv, m_currentPos.y());
                slot_btn_Middle_clicked();
                m_IsMax = false;
                return;
            }
            else
            {
                if(m_IsClickMax)
                {
                    //                    int desktop_width = QApplication::desktop()->width();
                    int dv = m_currentPos.x() * 7 / 10;
                    //                    resize(this->width(),this->height());
                    move(event->globalPos().x() - dv, event->globalPos().y());
                    return;
                }
                else
                {
                    //                    resize(this->width(),this->height());
                    move(event->globalPos().x() - m_currentWidgetPos.x(), event->globalPos().y() - m_currentWidgetPos.y());
                    return;
                }
            }
        }
    }

    MainWindow::mouseMoveEvent(event);
}

void MainWindow::slot_tBtn_DataImport_clicked()
{
    qDebug()<<"slot_tBtn_DataImport_clicked";
    //m_stackedWidget->setCurrentIndex(0);
    m_pMainWidget->setIndex(0);
}

void MainWindow::slot_tBtn_Process_clicked()
{
    qDebug()<<"slot_tBtn_Process_clicked";
    //m_stackedWidget->setCurrentIndex(1);
    m_pMainWidget->setIndex(1);
}

void MainWindow::slot_tBtn_Analysis_clicked()
{
    qDebug()<<"slot_tBtn_Analysis_clicked";
   // m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::slot_tbtn_Report_clicked()
{
    qDebug()<<"slot_tbtn_Report_clicked";
   // m_stackedWidget->setCurrentIndex(1);
}
