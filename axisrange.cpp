#include "axisrange.h"
#include "ui_axisrange.h"

AxisRange::AxisRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AxisRange)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::SubWindow);
    setWindowModality(Qt::WindowModal);
    this->setWindowTitle("坐标配置");
}

AxisRange::~AxisRange()
{
    delete ui;
}

void AxisRange::getValue(int &min, int &max)
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty())
    {
        return ;
    }
    min =m_min;
    max =m_max;
}

void AxisRange::on_pushButton_clicked()
{
    m_min = ui->lineEdit->text().toInt();
    m_max = ui->lineEdit_2->text().toInt();
    QDialog::accept();
    this->close();
}

void AxisRange::on_pushButton_2_clicked()
{
    QDialog::reject();
    this->close();
}
