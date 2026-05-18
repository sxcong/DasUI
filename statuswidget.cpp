#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    ui->label_RunStatus->setStyleSheet("color:#12C17E;border-color:#36EB79;");
    ui->label_DAS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_DTS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_fileSave->setStyleSheet("color:#FFFF00;border-color:#FFFF00;");
}

StatusWidget::~StatusWidget()
{
    delete ui;
}
