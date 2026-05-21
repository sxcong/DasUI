#include "advancedanalysiswidget.h"
#include "ui_advancedanalysiswidget.h"

AdvancedAnalysisWidget::AdvancedAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedAnalysisWidget)
{
    ui->setupUi(this);
    // 设置暗色背景
    this->setStyleSheet("background-color: #141519; color: #FFFFFF;");
    ui->label_title->setText("高级分析模块 - 算法处理中");
}

AdvancedAnalysisWidget::~AdvancedAnalysisWidget()
{
    delete ui;
}
