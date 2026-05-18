#include "filetreelistwidget.h"
#include "ui_filetreelistwidget.h"
#include <QVBoxLayout>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QHeaderView>
#include <QElapsedTimer>
#include <QLabel>
#include <QToolButton>

FileTreeListWidget::FileTreeListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileTreeListWidget)
{
    ui->setupUi(this);

    setupCentralLayout();
    //setupStylesheetText();
   setupStylesheetText_vscode();
}

FileTreeListWidget::~FileTreeListWidget()
{
    delete ui;
}


void FileTreeListWidget::setupCentralLayout() {
    //--root
    //      --井信息
    //      --DAS
    //      --DTS
    mainSplitter = new QSplitter(Qt::Horizontal, this);

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->addWidget(mainSplitter);
    pageLayout->setContentsMargins(0, 0, 0, 0); // 如果想让分割器顶格，边距设为0


    //左上：button
    {
        QHBoxLayout *headerLayout = new QHBoxLayout(ui->widget);
        headerLayout->setContentsMargins(5, 2, 5, 2); // 紧凑间距

        QLabel *titleLabel = new QLabel("工区管理：", ui->widget);
        titleLabel->setStyleSheet("font-weight: bold; color: #fff; text-transform: uppercase; font-size: 16px;"); // 搭配你的深色皮肤
        headerLayout->addWidget(titleLabel);

        QToolButton *addButton = new QToolButton(ui->widget);
        addButton->setText("+工区列表");
        addButton->setToolTip("添加工区");
        // 可以设置无边框样式，更符合你的扁平化UI
        addButton->setStyleSheet("QToolButton { border: none; color: white; } QToolButton:hover { background: #444; }");

        QToolButton *delButton = new QToolButton(ui->widget);
        delButton->setText("×添加工区");
        delButton->setToolTip("删除工区");
        delButton->setStyleSheet("QToolButton { border: none; color: white; } QToolButton:hover { background: #444; }");

        headerLayout->addStretch();
        headerLayout->addWidget(addButton);
        headerLayout->addWidget(delButton);

        //ui->verticalLayout
    }

    QIcon folderIcon;
    folderIcon.addFile(":/res/png/datarootnode.png", QSize(), QIcon::Normal, QIcon::Off);
    //folderIcon.addFile(":/icons/datarootnode.png", QSize(), QIcon::Normal, QIcon::On);
    // --- 左侧：树形控件 ---
    //leftTree = new QTreeWidget(ui->widget_left);
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setHeaderLabel("工区--" );
    ui->treeWidget->setIconSize(QSize(20, 20));


   /* {
        //换上自定义表头，增加几个button
        // 1. 隐藏原有的表头
        leftTree->setHeaderHidden(true);

        // 2. 创建一个自定义的表头组合 Widget
        QWidget *customHeader = new QWidget(this);
        QHBoxLayout *headerLayout = new QHBoxLayout(customHeader);
        headerLayout->setContentsMargins(5, 2, 5, 2); // 紧凑间距

        // 3. 添加标题标签
        QLabel *titleLabel = new QLabel("工区--", customHeader);
        titleLabel->setStyleSheet("font-weight: bold; color: #fff;"); // 搭配你的深色皮肤
        headerLayout->addWidget(titleLabel);

        // 4. 添加弹簧，把按钮推到最右侧
        headerLayout->addStretch();

        // 5. 添加功能按钮（用 QToolButton 更精致）
        QToolButton *addButton = new QToolButton(customHeader);
        addButton->setText("+");
        addButton->setToolTip("添加工区");
        // 可以设置无边框样式，更符合你的扁平化UI
        addButton->setStyleSheet("QToolButton { border: none; color: white; } QToolButton:hover { background: #444; }");

        QToolButton *delButton = new QToolButton(customHeader);
        delButton->setText("×");
        delButton->setToolTip("删除工区");
        delButton->setStyleSheet("QToolButton { border: none; color: white; } QToolButton:hover { background: #444; }");

        headerLayout->addWidget(addButton);
        headerLayout->addWidget(delButton);

        // 6. 核心：在垂直布局中，把 customHeader 放在 leftTree 的正上方
        QVBoxLayout *leftMenuLayout = new QVBoxLayout();
        leftMenuLayout->setSpacing(0);
        leftMenuLayout->setContentsMargins(0, 0, 0, 0);
        leftMenuLayout->addWidget(customHeader); // 假的表头在上面
        leftMenuLayout->addWidget(leftTree);     // 没有表头的树在下面


        //ui->verticalLayout->addLayout(headerLayout);
        //ui->verticalLayout->addLayout(leftMenuLayout);
    }*/



    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget);
    rootItem->setText(0, "数据");
    rootItem->setIcon(0, QIcon(":/res/png/datarootnode.png")); // 可以设置一个文件夹或方块图标

    // 创建节点并关联索引（使用 Data 角色存储索引）
    wellItem = new QTreeWidgetItem(rootItem, QStringList() << "井信息");
    wellItem->setData(0, Qt::UserRole, 0); // 对应 stackWidget 的第 0 页
    wellItem->setIcon(0, QIcon(":/res/png/das.png")); // 设置第一列的图标

    dasItem = new QTreeWidgetItem(rootItem, QStringList() << "DAS 监控模块");
    dasItem->setData(0, Qt::UserRole, 1); // 对应 stackWidget 的第 0 页
    dasItem->setIcon(0, QIcon(":/res/png/das.png")); // 设置第一列的图标

    dtsItem = new QTreeWidgetItem(rootItem, QStringList() << "DTS 监测模块");
    dtsItem->setData(0, Qt::UserRole, 2); // 对应 stackWidget 的第 1 页
    dtsItem->setIcon(0, QIcon(":/res/png/dts.png")); // 设置第一列的图标

    // 2. 右侧 Tab 控件
    tabWidget = new QTabWidget(ui->widget_right);
    tabWidget->setTabsClosable(true); // 允许关闭页签

    // === 核心修复：为右侧 widget_right 添加一个布局，把 tabWidget 塞进去撑满！ ===
    QVBoxLayout* rightWidgetLayout = new QVBoxLayout(ui->widget_right);
    rightWidgetLayout->setContentsMargins(0, 0, 0, 0); // 顶格显示
    rightWidgetLayout->addWidget(tabWidget);           // 让 tabWidget 自动缩放撑满右侧


    connect(tabWidget, &QTabWidget::tabCloseRequested, tabWidget, &QTabWidget::removeTab);

    // --- 组装 ---
    mainSplitter->addWidget(ui->widget_left);
    mainSplitter->addWidget(ui->widget_right);
    mainSplitter->setStretchFactor(1, 1);
    //this->setCentralWidget(mainSplitter);

    // 绑定信号槽：点击树节点切换页面
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &FileTreeListWidget::onTreeItemClicked);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &FileTreeListWidget::showTreeContextMenu);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &FileTreeListWidget::onTreeItemClicked);
    ui->treeWidget->expandAll();
}

void FileTreeListWidget::onTreeItemClicked(QTreeWidgetItem *item, int column) {
    // 获取存储在节点中的路径（我们在 importFolder 时需要存进去）
    QString targetPath = item->toolTip(0);

    // 遍历所有 Tab，寻找路径匹配的那一个
    for (int i = 0; i < tabWidget->count(); ++i) {
        QWidget *page = tabWidget->widget(i);
        if (page->property("folderPath").toString() == targetPath) {
            tabWidget->setCurrentIndex(i); // 找到并切换
            return;
        }
    }
}
void FileTreeListWidget::showTreeContextMenu(const QPoint &pos)
{
    // 获取鼠标点击位置的节点
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item) return;

    // 检查是否为 DAS 节点（通过之前设置的 Data 识别）
    if (item->data(0, Qt::UserRole).toInt() == 1) {
        QMenu menu(this);

        // 创建“导入DAS数据”动作并添加图标
        QAction *importAction = new QAction(QIcon(":/res/png/import.png"), "导入DAS数据", this);
        menu.addAction(importAction);

        // 连接动作到打开文件夹的逻辑
        // connect(importAction, &QAction::triggered, this, &MainWindow::onImportDasData); //两种实现方式

        // 在鼠标当前位置显示菜单
        // menu.exec(leftTree->mapToGlobal(pos));

        // 获取点击位置的动作
        QAction *selectedAction = menu.exec(ui->treeWidget->mapToGlobal(pos));
        if (selectedAction == importAction) {
            importFolder();
        }
    }
    else
    {
        QTreeWidgetItem *root = item->parent();
        if (root != nullptr)
        {
            if (root->data(0, Qt::UserRole).toInt() == 1)//父节点是DAS
            {
                QMenu menu(this);
                // 创建“移除DAS数据”动作并添加图标
                QAction *removeAction = new QAction(QIcon(":/res/png/import.png"), "移除DAS数据", this);
                menu.addAction(removeAction);
                // 获取点击位置的动作
                QAction *selectedAction = menu.exec(ui->treeWidget->mapToGlobal(pos));
                if (selectedAction == removeAction) {
                    QString str = item->data(0, Qt::UserRole).toString();
                    removeFolder(str);
                    removeTab(item->text(0), str);
                    //leftTree->removeItemWidget(item, 0);
                    root->removeChild(item);
                    delete item;
                }
            }
        }
    }
}

void FileTreeListWidget::removeFolder(const QString& szPath)
{
    qDebug()<<"removeFolder"<<szPath;
   // ProjectMgr::Instance().deleteProjectPath(szPath);

}

void FileTreeListWidget::importFolder() {
    QString dirPath = QFileDialog::getExistingDirectory(this, "选择文件夹","/", QFileDialog::ShowDirsOnly);
    if (dirPath.isEmpty()) return;

    // 检查是否已经打开过这个文件夹（避免重复打开同名的 Tab）
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->widget(i)->property("folderPath").toString() == dirPath) {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    QDir directory(dirPath);
    QTreeWidgetItem *item = new QTreeWidgetItem(dasItem);
    item->setText(0, directory.dirName());
    item->setToolTip(0, dirPath); // 关键：将路径存入 ToolTip 或 Data
    item->setIcon(0, QIcon(":/res/png/import.png")); // 设置第一列的图标

    item->setData(0, Qt::UserRole, dirPath);

    createNewTab(directory.dirName(), dirPath);
   // bool bRet = ProjectMgr::Instance().addProjectPath(dirPath);
   // qDebug()<<"addProjectPath"<<dirPath<<bRet;
}


void FileTreeListWidget::removeTab(const QString &title, const QString &path)
{
    QString targetPath = path;

    // 遍历所有 Tab，寻找路径匹配的那一个
    for (int i = 0; i < tabWidget->count(); ++i) {
        QWidget *page = tabWidget->widget(i);
        if (page->property("folderPath").toString() == targetPath) {
            //tabWidget->setCurrentIndex(i); // 找到并切换
            tabWidget->removeTab(i);
            return;
        }
    }
}
// 通用处理函数
void FileTreeListWidget::showGeneralContextMenu(QTableWidget* table, const QPoint &pos) {

    QMenu menu(table);
    QTableWidgetItem *item = table->itemAt(pos);
    qDebug() << "showGeneralContextMenu:" << item->text();
    if (!item)
        return;

    QString szFile = m_dasFileList.at(item->row());

    QAction *act1 = new QAction("分频能量-频谱分析", &menu);
    connect(act1, &QAction::triggered, this, [=]() {
       // DASFBECurveFFTDlg dlg;
       // dlg.setF5File(szFile);
       // dlg.exec();
    });

    QAction *act2 = new QAction("LF-DAS瀑布图", &menu);
    connect(act2, &QAction::triggered, this, [=]() {
        //LF_DAS_Dlg dlg;

       // dlg.setH5File(szFile);
       // dlg.exec();
    });

    QAction *act3 = new QAction("FBE分频能量瀑布图", &menu);
    connect(act3, &QAction::triggered, this, [=]() {
        //FracturingDlg dlg;
        //dlg.exec();
    });

    QAction *act4 = new QAction("F-K", &menu);
    connect(act4, &QAction::triggered, this, [=]() {
        qDebug()<<"szFile = "<<szFile;
       // FKDemoDialog dlg;
       // dlg.setH5File(szFile);
       // dlg.exec();
    });

    menu.addAction(act1);
    menu.addAction(act2);
    menu.addAction(act3);
    menu.addAction(act4);


    // 将点击位置 pos 转换为全局坐标
    QPoint globalPos = table->viewport()->mapToGlobal(pos);
    menu.exec(globalPos);
}



void FileTreeListWidget::createNewTab(const QString &title, const QString &path)
{
    // 创建容器 Widget
    QWidget *container = new QWidget();
    container->setProperty("folderPath", path);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建表格
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"文件名", "大小", "修改时间"});
    //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    table->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #1E2A3A; color: #D0D0D0;"
        "    padding: 6px; border: none;"
        "    border-right: 1px solid #2A3A4A; border-bottom: 1px solid #2A3A4A;"
        "    font: 10pt '微软雅黑';"
        "}"
        "QTableWidget {"
        "    background-color: #1A1A2E; color: #C0C0C0;"
        "    gridline-color: #2A3A4A; border: none;"
        "    font: 10pt '微软雅黑';"
        "}"
        "QTableWidget::item {"
        "    padding: 4px 8px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #2A5A8A; color: #FFFFFF;"
        "}"
        "QTableWidget::item:hover {"
        "    background-color: #2A4A6A;"
        "}"
        "QTableCornerButton::section {"
        "    background-color: #1E2A3A; border: none;"
        "}"
        );

    // 遍历文件夹并填充数据
    m_dasFileList.clear();
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.h5");
    // QDir::Files 表示只列出文件（不包含目录）
    // QDir::NoDotAndDotDot 表示不列出 "." 和 ".."
    //dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList(QDir::Files); // 仅获取文件

    table->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        // 文件名
        table->setItem(i, 0, new QTableWidgetItem(fileInfo.fileName()));

        m_dasFileList.append(fileInfo.filePath());

        // 大小 (格式化显示)
        qint64 size = fileInfo.size();
        QString sizeStr = QString::number(size / 1024.0, 'f', 2) + " KB";
        table->setItem(i, 1, new QTableWidgetItem(sizeStr));

        // 修改时间
        QString timeStr = fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss");
        table->setItem(i, 2, new QTableWidgetItem(timeStr));
    }

    setupContextMenu(table);

    //双击处理相当文件
    connect(table, &QTableWidget::cellDoubleClicked,
            this, [=](int row, int column) {
                // 这里写你的逻辑
                QTableWidgetItem *item = table->item(row, column);


                if (item) {
                    QString szFile = m_dasFileList.at(row);
                    qDebug() << "双击了:" << item->text()<<szFile;
                    /* SecondaryDlg secondaryDlg;
                    secondaryDlg.loadData(szFile);
                    secondaryDlg.exec();*/
                    std::vector<std::pair<float, float>> band_limits = {
                        {0, 1000}, {10, 50}, {50, 100}, {100, 200}, {200, 500}
                    };

                    QElapsedTimer timer;
                    timer.start();
                    //createFBE(szFile, "e:/fbe/1_fbe", 2000, band_limits);
                    //createFBESegment(szFile, "e:/fbe/1_fbe", 0, band_limits);


                   // DASFBECurveFFTDlg dlg;
                   // dlg.setF5File(szFile);
                   // dlg.exec();

                    //qDebug() << "createFBE Total processing time:" << timer.elapsed() << "ms";

                } else {
                    qDebug() << "双击了空白单元格";
                }
            });

  //  ProjectMgr::Instance().m_projectFile.setDASPath(path, m_dasFileList);

    layout->addWidget(table);

    // 添加到 Tab 并切换至当前页
    int index = tabWidget->addTab(container, title);
    tabWidget->setCurrentIndex(index);


    // 获取水平表头指针
    QHeaderView *header = table->horizontalHeader();

    // 1. 允许用户手动拖动改变列宽
    header->setSectionsMovable(false); // 是否允许交换列顺序（可选）
    header->setSectionResizeMode(QHeaderView::Interactive); // 关键：设置为交互模式

    // 2. 设置初始宽度或默认拉伸
    table->setColumnWidth(0, 150); // 第一列初始宽度
    table->setColumnWidth(1, 200); // 第二列初始宽度

    // 3. (进阶) 如果你希望最后一列自动填满剩余空间，同时前面的列可以手动拖动：
    header->setStretchLastSection(true);

    // 4. 开启表头点击排序（推荐，非常实用）
    table->setSortingEnabled(true);

    //ProjectMgr::Instance().m_curProjectName = nameItem->text();

}

// 辅助函数：绑定信号
void FileTreeListWidget::setupContextMenu(QTableWidget* table) {
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        // 调用通用处理函数，把当前的 table 传进去
        this->showGeneralContextMenu(table, pos);
    });
}

void FileTreeListWidget::setupStylesheetText() {
    QString qss = R"(
       /* 左侧树状图容器（包含上方的工区管理小控件） */
        #widget_left {
            background-color: #151522;
        }

        /* 顶部的“工区管理”自定义工具栏 */
        #widget_left > QWidget {
            background-color: #1E2A3A;
            border-bottom: 1px solid #10101A;
        }

        /* 树控件本身 */
        QTreeWidget {
            background-color: #151522;
            color: #C0C0C0;
            border: none;
            font-size: 13px;
        }

        /* 树节点的每一行 */
        QTreeWidget::item {
            padding: 6px 4px;
            border-bottom: 1px solid #1A1A2A; /* 隐约的水平分割线 */
        }

        /* 鼠标悬停在树节点上 */
        QTreeWidget::item:hover {
            background-color: #2A4A6A;
            color: #FFFFFF;
        }

        /* 选中某个树节点（比如当前激活的工区） */
        QTreeWidget::item:selected {
            background-color: #2A5A8A;
            color: #FFFFFF;
            font-weight: bold;
        }

        /* 树左侧的展开/折叠小箭头（加分细节：变成亮灰色） */
        QTreeWidget::branch {
            background-color: transparent;
        }
        )";
    this->setStyleSheet(qss);
}

void FileTreeListWidget::setupStylesheetText_vscode()
{
    QString qss = R"(
/* 1. 作用于整个树控件（底色与右侧表格稍微呼应，但更暗一点） */
QTreeWidget {
    background-color: #151522; /* 方案一的深蓝黑，或改成 #1E1E1E 炭黑 */
    color: #C0C0C0;
    border: none;
    font-size: 13px;
}

/* 2. 核心修复：必须同时显式重置 branch 和 item 的背景 */
QTreeWidget::branch {
    background-color: transparent; /* 强行让左侧分支线和箭头区域背景透明 */
}

QTreeWidget::item {
    background-color: transparent; /* 默认节点行背景透明，跟随树的整体背景 */
    color: #C0C0C0;
    padding: 6px 4px; /* 增加呼吸间距 */
    border-bottom: 1px solid #1A1A2A; /* 淡淡的水平分割线，让列表更有秩序 */
}

/* 3. 鼠标悬停状态（只改变 item 区域，防止支线跟着变色混乱） */
QTreeWidget::item:hover {
    background-color: #2A4A6A; /* 悬停蓝灰色 */
    color: #FFFFFF;
}

/* 4. 选中状态 */
QTreeWidget::item:selected {
    background-color: #2A5A8A; /* 亮蓝色选中状态 */
    color: #FFFFFF;
    font-weight: bold;
}

/* 5. 可选高级微调：如果悬停或选中时，左边小箭头区域也想跟着变色，可以这样追加： */
QTreeWidget::branch:hover {
    background-color: #2A4A6A;
}
QTreeWidget::branch:selected {
    background-color: #2A5A8A;
}
        )";
    this->setStyleSheet(qss);
}
