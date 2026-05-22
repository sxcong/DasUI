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
#include "filelistwidget.h"
#include "projectmgr.h"
#include "projectlistdialog.h"

FileTreeListWidget::FileTreeListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileTreeListWidget)
{
    ui->setupUi(this);

    ui->widget_right->setAttribute(Qt::WA_StyledBackground, true);
    // 确保在 UI 初始化后设置了名称
    ui->widget_right->setObjectName("widget_right");

    setupCentralLayout();
    //loadProjectPath();
}

FileTreeListWidget::~FileTreeListWidget()
{
    delete ui;
}

void FileTreeListWidget::loadProjectPath()
{
    // 清空旧树和 Tab，防止重复调用时残留
    ui->treeWidget->clear();
    tabWidget->clear();

    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget);
    QString projectName = "工区:" + ProjectMgr::Instance().m_curProjectName;
    rootItem->setText(0, projectName);

    wellItem = new QTreeWidgetItem(rootItem, QStringList() << "井信息");
    wellItem->setData(0, Qt::UserRole, 0);
    wellItem->setIcon(0, QIcon(":/res/png/das.png"));

    dasItem = new QTreeWidgetItem(rootItem, QStringList() << "DAS 监控模块");
    dasItem->setData(0, Qt::UserRole, 1);
    dasItem->setIcon(0, QIcon(":/res/png/das.png"));

    dtsItem = new QTreeWidgetItem(rootItem, QStringList() << "DTS 监测模块");
    dtsItem->setData(0, Qt::UserRole, 2);
    dtsItem->setIcon(0, QIcon(":/res/png/dts.png"));

    QStringList pathList;
    ProjectMgr::Instance().getProjectPath(pathList);
    for (const QString &str : pathList) {
        QDir directory(str);
        QTreeWidgetItem *item = new QTreeWidgetItem(dasItem);
        item->setText(0, directory.dirName());
        item->setToolTip(0, str);
        item->setIcon(0, QIcon(":/res/png/import.png"));
        item->setData(0, Qt::UserRole, str);
        createNewTab(directory.dirName(), str);
    }
    ui->treeWidget->expandAll();
}

void FileTreeListWidget::setupCentralLayout() {
    //--root
    //      --井信息
    //      --DAS
    //      --DTS
    mainSplitter = new AnimatedSplitter(Qt::Horizontal, this);

    //mainSplitter->setHandleWidth(1);

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->addWidget(mainSplitter);
    pageLayout->setContentsMargins(0, 0, 0, 0); // 如果想让分割器顶格，边距设为0


    //左上：button
    {
        ui->widget->setStyleSheet(R"(
        QWidget#widget {
            background-color: #1A1C23;
            border-bottom: 1px solid #2A2A2A;
        }
        )");

        QHBoxLayout *headerLayout = new QHBoxLayout(ui->widget);
        headerLayout->setContentsMargins(12, 6, 8, 6);

        QLabel *titleLabel = new QLabel("工区管理", ui->widget);
        titleLabel->setStyleSheet("font-weight: bold; color: #FFFFFF; font-size: 14px; background: transparent;");
        headerLayout->addWidget(titleLabel);

        QString btnCommon = R"(
        QToolButton {
            border: 1px solid #3A3A3A;
            border-radius: 4px;
            color: #CCCCCC;
            background: #2A2A2A;
            padding: 4px 12px;
            font-size: 12px;
        }
        QToolButton:hover {
            background: #3A3A3A;
            color: #FFFFFF;
            border-color: #4A8BC2;
        }
        QToolButton:pressed {
            background: #222222;
        }
        )";

        addButton = new QToolButton(ui->widget);
        addButton->setText("添加工区");
        addButton->setToolTip("添加工区");
        addButton->setStyleSheet(btnCommon);

        openButton = new QToolButton(ui->widget);
        openButton->setText("打开工区");
        openButton->setToolTip("打开工区");
        openButton->setStyleSheet(btnCommon);

        headerLayout->addStretch();
        headerLayout->addWidget(addButton);
        headerLayout->addWidget(openButton);

        connect(addButton, &QToolButton::clicked, this, [=]() {
            ProjectListDialog dlg;
            dlg.setRole(0);
            dlg.refreshList();
            if (dlg.exec() == QDialog::Accepted)
            {
            }
        });
        connect(openButton, &QToolButton::clicked, this, [=]() {
            ProjectListDialog dlg;
            dlg.setRole(1);
            dlg.refreshList();
            if (dlg.exec() == QDialog::Accepted)
            {
                //setupCentralLayout();
                loadProjectPath();
            }
        });
    }

    // --- 左侧：树形控件（节点由 loadProjectPath 创建）---
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setIconSize(QSize(20, 20));
    ui->treeWidget->setIndentation(20);
    ui->treeWidget->setFrameShape(QFrame::NoFrame);
    ui->treeWidget->setStyle(new ArrowTreeStyle);

    // 2. 右侧 Tab 控件
    tabWidget = new QTabWidget(ui->widget_right);
    tabWidget->setTabsClosable(true); // 允许关闭页签

    // === 核心修复：为右侧 widget_right 添加一个布局，把 tabWidget 塞进去撑满！ ===
    QVBoxLayout* rightWidgetLayout = new QVBoxLayout(ui->widget_right);
    rightWidgetLayout->setContentsMargins(0, 0, 0, 0); // 顶格显示
    rightWidgetLayout->addWidget(tabWidget);           // 让 tabWidget 自动缩放撑满右侧


   // connect(tabWidget, &QTabWidget::tabCloseRequested, tabWidget, &QTabWidget::removeTab);

    // --- 组装 ---
    mainSplitter->addWidget(ui->widget_left);
    mainSplitter->addWidget(ui->widget_right);
    mainSplitter->setStretchFactor(1, 1);
    //this->setCentralWidget(mainSplitter);

    // 绑定信号槽：点击树节点切换页面
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &FileTreeListWidget::onTreeItemClicked);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &FileTreeListWidget::showTreeContextMenu);

    // === 暗色主题 QSS ===
    ui->widget_left->setStyleSheet(R"(
    QWidget#widget_left {
        background-color: #141519;
    }
    QTreeView {
        background-color: #141519;
        border: none;
        outline: none;
    }
    QTreeView::item {
        padding: 6px 12px;
        margin: 1px 4px;
        color: #BBBBBB;
        border-radius: 4px;
    }
    QTreeView::item:selected {
        background-color: #2E436E;
        color: #FFFFFF;
        border: 1px solid #4A8BC2;
    }
    QTreeView::item:hover:!selected {
        background-color: #222438;
    }
    QTreeView::item:selected:hover {
        background-color: #34508A;
        border: 1px solid #5BA3D6;
    }
    )");

    tabWidget->setStyleSheet(R"(
    QTabWidget::pane {
        background-color: #141519;
        border: none;
        border-top: 1px solid #2A2A2A;
    }
    QTabBar::tab {
        background: #1E1E1E;
        color: #999999;
        border: none;
        min-width: 60px;
        padding: 6px 14px;
        margin-right: 1px;
    }
    QTabBar::tab:selected {
        background: #141519;
        color: #FFFFFF;
        border-top: 2px solid #4A8BC2;
    }
    QTabBar::tab:hover:!selected {
        background: #2A2A2A;
        color: #CCCCCC;
    }
    QTabBar::close-button {
        image: url(:/res/png/close.png);
        subcontrol-position: right;
    }
    QWidget#qt_tabwidget_stackedwidget {
        background: #141519;
    }
    )");
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
    ProjectMgr::Instance().deleteProjectPath(szPath);

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
    bool bRet = ProjectMgr::Instance().addProjectPath(dirPath);
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
    {
        FileListWidget* pWidget = new FileListWidget();
        pWidget->setProperty("folderPath", path);
        pWidget->setPath(title, path);
        int index = tabWidget->addTab(pWidget, title);
        tabWidget->setCurrentIndex(index);
    }
    return;
}
