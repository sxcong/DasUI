#include "reportwidget.h"
#include "ui_reportwidget.h"

ReportWidget::ReportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportWidget)
{
    ui->setupUi(this);
    // 设置暗色背景
    this->setStyleSheet("background-color: #141519; color: #FFFFFF;");
}

ReportWidget::~ReportWidget()
{
    delete ui;
}