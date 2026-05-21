#include "systemsettingswidget.h"
#include "ui_systemsettingswidget.h"

SystemSettingsWidget::SystemSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemSettingsWidget)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #141519; color: white;");
    ui->label_title->setText("系统设置 - 全局参数配置");
}

SystemSettingsWidget::~SystemSettingsWidget()
{
    delete ui;
}
