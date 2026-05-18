#include "informationwgt.h"
#include "ui_informationwgt.h"

#include <QMouseEvent>
#include <QPainter>

InformationWgt::InformationWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWgt)
{
    ui->setupUi(this);

    pic.load(":/res/images/table.png");
    pic =pic.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    this->setWindowModality(Qt::WindowModal);
    this->setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint/* | Qt::WindowStaysOnTopHint*/);
//    ui->tableWidget->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    initTableWidget(ui->tableWidget);

    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(4);


    QStringList keys;
    QStringList values;
    //m_oilWellInfo.getKeys(keys,values);

    QStringList listName1;
    listName1 << "井名"<< "井深" <<"射孔深度"<<"一级套管"<<"二级套管"<<"三级套管";

    QStringList info1;
//        info1 << "windows"<< "1.0" <<"You Jing"<<""<<""<<"";

    QStringList listName2;
        listName2 << "系统"<< "版本号" <<"公司"<<"编译器"<<""<<"";

    QStringList info2;
//        info2 << "windows"<< "1.0" <<"You Jing"<<""<<""<<"";
    if(values.size()< 12)
    {
        return;
    }
    for(int  i = 0;i < 6;i++)
    {
//        listName1.append(keys[i]);
        info1.append(values[i]);
//        listName2.append(keys[i+6]);
        info2.append(values[i+6]);
    }


    QFont boldFont;
    boldFont.setBold(true);
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        auto cell0 = new QTableWidgetItem(listName1[i]);
        cell0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        cell0->setForeground(QColor(100, 180, 255));
        cell0->setFont(boldFont);
        cell0->setBackground(QColor(21, 46, 84));
        ui->tableWidget->setItem(i, 0, cell0);

        auto cell1 = new QTableWidgetItem(info1[i]);
        cell1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        cell1->setForeground(QColor(220, 240, 255));
        ui->tableWidget->setItem(i, 1, cell1);

        auto cell2 = new QTableWidgetItem(listName2[i]);
        cell2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        cell2->setForeground(QColor(100, 180, 255));
        cell2->setFont(boldFont);
        cell2->setBackground(QColor(21, 46, 84));
        ui->tableWidget->setItem(i, 2, cell2);

        auto cell3 = new QTableWidgetItem(info2[i]);
        cell3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        cell3->setForeground(QColor(220, 240, 255));
        ui->tableWidget->setItem(i, 3, cell3);
    }

    this->installEventFilter(this);
}

InformationWgt::~InformationWgt()
{
    delete ui;
}

void InformationWgt::on_pushButton_clicked()
{
    this->close();
}
void InformationWgt::initTableWidget(QTableWidget *pTable)
{
    if (pTable == nullptr)
        return;

    pTable->setAlternatingRowColors(true);
    pTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    pTable->setSelectionMode(QAbstractItemView::NoSelection);

    pTable->verticalHeader()->setVisible(false);
    pTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    pTable->horizontalHeader()->setStretchLastSection(true);
    pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pTable->horizontalHeader()->setHighlightSections(false);
    pTable->setWordWrap(false);

    pTable->setShowGrid(false);
}



void InformationWgt::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);
   painter.drawPixmap(0, 0, pic);
}

void InformationWgt::on_pushButton_2_clicked()
{
    this->close();
}
bool InformationWgt::eventFilter(QObject *watched, QEvent *evt)
{
    static int index = 1;
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - this->pos();

//            if (index == 5) {
//                index = 1;
//            } else {
//                index++;
//            }

//            ui->widget->setStyleSheet(QString("background-image:url(:/image/%1.png);").arg(index));

            return true;
        } else {
            exit(0);
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            this->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}


