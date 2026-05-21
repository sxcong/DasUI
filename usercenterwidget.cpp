#include "usercenterwidget.h"
#include "ui_usercenterwidget.h"

UserCenterWidget::UserCenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserCenterWidget)
{
    ui->setupUi(this);
    // 这里可以添加个人中心的初始化逻辑，如加载用户信息等
    this->setStyleSheet("background-color: #141519; color: white;");
    ui->label_title->setText("个人中心 - 用户权限管理");
}

UserCenterWidget::~UserCenterWidget()
{
    delete ui;
}
