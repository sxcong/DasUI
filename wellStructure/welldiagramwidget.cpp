#include "welldiagramwidget.h"
#include "ui_welldiagramwidget.h"

#include <QColorDialog>
#include <QFileDialog>
#include "QDebug"
#include "configdialog.h"

WellDiagramWidget::WellDiagramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WellDiagramWidget)
{
    ui->setupUi(this);

    m_colors<< QColor(168,168,168)<<QColor(208,208,208)<<QColor(253,103,50)<<QColor(101,104,113);

    initWidgets();
    ConfigMgr::instance().loadConfig();
    on_comboBox_type_currentIndexChanged(0);
    ui->label_6->hide();
    ui->spinBox_width->hide();

    on_pushButton_clicked();
}

WellDiagramWidget::~WellDiagramWidget()
{
    delete ui;
}
void WellDiagramWidget::initWidgets()
{

    m_paintArea = ui->widget_mainpaint;

}



void WellDiagramWidget::on_pushButton_2_clicked()
{
    int index = ui->comboBox_type->currentIndex();
//    笔颜色
    QColor penColor = QColor(0,0,0);
    //笔宽
    int penWidth = 1;
    //画笔样式
    Qt::PenStyle penStyle = Qt::PenStyle(Qt::SolidLine);

    QColor brushColor =  m_colors[index];
    QBrush brush = QBrush(brushColor, Qt::BrushStyle(Qt::SolidPattern));

    int width = ui->spinBox_width->value();
    int height = ui->spinBox_depth->value();

    if(index == 0)
    {
        m_paintArea->setCannulaH(ui->spinBox_depth_1->value(),ui->spinBox_depth_2->value(),ui->spinBox_depth_3->value());
        ConfigMgr::instance().m_compara[0].m_h= ui->spinBox_depth_1->value();
        ConfigMgr::instance().m_compara[4].m_h= ui->spinBox_depth_2->value();
        ConfigMgr::instance().m_compara[5].m_h= ui->spinBox_depth_3->value();
    }
    else {

        ConfigMgr::instance().m_compara[index].m_h = height;
    }

    ConfigMgr::instance().m_compara[index].m_w = width;
    ConfigMgr::instance().saveConfig();

    PaintArea::Shape shape = PaintArea::Shape(ui->comboBox_type->currentIndex());
    m_paintArea->setShape(shape,QPen(penColor,penWidth,penStyle),brush,width,height);
    qDebug()<<"cc"<<QPen(penColor,penWidth,penStyle)<<brush;
}






void WellDiagramWidget::on_comboBox_type_currentIndexChanged(int index)
{
    int width = ConfigMgr::instance().m_compara[index].m_w;
    int height = ConfigMgr::instance().m_compara[index].m_h;
    ui->spinBox_width->setValue(width);

    int depth = ConfigMgr::instance().m_Fulllength;
    ui->lineEdit->setText(QString::number(depth));

    if(index == 0)
    {
        ui->spinBox_depth_1->setValue(ConfigMgr::instance().m_compara[0].m_h);
        ui->spinBox_depth_2->setValue(ConfigMgr::instance().m_compara[4].m_h);
        ui->spinBox_depth_3->setValue(ConfigMgr::instance().m_compara[5].m_h);

    }else {
        ui->spinBox_depth->setValue(height);
    }


    if(index == 0 )
    {
        ui->label_8->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->spinBox_depth_1->show();
        ui->spinBox_depth_2->show();
        ui->spinBox_depth_3->show();

        ui->label_7->hide();
        ui->spinBox_depth->hide();
    }
    else {
        ui->label_8->hide();
        ui->label_9->hide();
        ui->label_10->hide();
        ui->spinBox_depth_1->hide();
        ui->spinBox_depth_2->hide();
        ui->spinBox_depth_3->hide();

        ui->label_7->show();
        ui->spinBox_depth->show();
    }
}



void WellDiagramWidget::on_pushButton_4_clicked()
{
    m_pixmap = ui->widget_mainpaint->grab();
    QString outPictureName = QCoreApplication::applicationDirPath()+"/config/WellBoreStructure.png";
    m_pixmap.save(outPictureName,"png",100);
    ConfigDialog::instance().signal_ImagChanged();
}

void WellDiagramWidget::on_pushButton_clicked()
{
    int depth  =ui->lineEdit->text().toInt();
    ConfigMgr::instance().m_Fulllength = depth;
    ConfigMgr::instance().saveConfig();
    double scale = (5000.0/depth)/10.0;
    m_paintArea->setScale(scale);
}

void WellDiagramWidget::on_pushButton_3_clicked()
{
    m_pixmap = ui->widget_mainpaint->grab();
    QString fileName = QFileDialog::getSaveFileName(this, tr("文件另存为"),
                               "D:/untitled.png",
                               tr("Images (*.png )"));
    m_pixmap.save(fileName,"png",100);
}
