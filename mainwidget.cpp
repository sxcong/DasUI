#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "leftnavbar.h"
#include "wellwidget.h"
#include "filetreelistwidget.h"
#include <QStackedWidget>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    leftNavBar = new LeftNavBar(this);
    ui->horizontalLayout->addWidget(leftNavBar); // 塞入最左侧

    rightStack = new QStackedWidget(this);
    ui->horizontalLayout->addWidget(rightStack);

    // 核心联动：左边点了谁，右边就切到对应的 index 面板
    connect(leftNavBar, &LeftNavBar::modulesChanged, rightStack, &QStackedWidget::setCurrentIndex);


    m_pWellWidget = new WellWidget(rightStack);
    m_pFileTreeListWidget = new FileTreeListWidget(rightStack);

    rightStack->addWidget(m_pFileTreeListWidget);
    rightStack->addWidget(m_pWellWidget);

    //ui->horizontalLayout->addWidget(m_stackedWidget);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::setIndex(int index)
{
    if (index >= 0 && index < rightStack->count())
        rightStack->setCurrentIndex(index);
}
