#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "leftnavbar.h"
#include "wellwidget.h"
#include "filetreelistwidget.h"
#include "advancedanalysiswidget.h"
#include "usercenterwidget.h"
#include "systemsettingswidget.h"
#include "reportwidget.h"
#include <QStackedWidget>
#include "projectdb.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    ProjectDB::Instance().initDB();

    leftNavBar = new LeftNavBar(this);
    ui->horizontalLayout->addWidget(leftNavBar); // 塞入最左侧


    rightStack = new QStackedWidget(this);
    ui->horizontalLayout->addWidget(rightStack);

    // 核心联动：左边点了谁，右边就切到对应的 index 面板
    connect(leftNavBar, &LeftNavBar::modulesChanged, rightStack, &QStackedWidget::setCurrentIndex);


    m_pWellWidget = new WellWidget(rightStack);
    m_pFileTreeListWidget = new FileTreeListWidget(rightStack);
    m_pAnalysisWidget = new AdvancedAnalysisWidget(rightStack);
    m_pUserCenterWidget = new UserCenterWidget(rightStack);
    m_pSystemSettingsWidget = new SystemSettingsWidget(rightStack);
    m_pReportWidget = new ReportWidget(rightStack);

    // 必须与 LeftNavBar 中按钮添加的顺序一致 (ID 从 0 开始)
    rightStack->addWidget(m_pFileTreeListWidget);
    rightStack->addWidget(m_pWellWidget);
    
    rightStack->addWidget(m_pReportWidget);        // ID 2: 报告
    
    rightStack->addWidget(m_pAnalysisWidget);      // ID 3: 分析
    rightStack->addWidget(m_pUserCenterWidget);    // ID 4: 用户
    rightStack->addWidget(m_pSystemSettingsWidget);// ID 5: 设置
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::setIndex(int index)
{
    qDebug()<<"setIndex"<<index;
    if (index >= 0 && index < rightStack->count())
        rightStack->setCurrentIndex(index);
}
