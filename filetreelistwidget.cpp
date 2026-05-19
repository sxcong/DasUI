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
#include <QPushButton>
#include <QStyleFactory>


FileTreeListWidget::FileTreeListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileTreeListWidget)
{
    ui->setupUi(this);

    ui->widget_right->setAttribute(Qt::WA_StyledBackground, true);
    // 确保在 UI 初始化后设置了名称
    ui->widget_right->setObjectName("widget_right");

    setupCentralLayout();

    return;
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
    mainSplitter = new AnimatedSplitter(Qt::Horizontal, this);

    //mainSplitter->setHandleWidth(1);

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
    }

    QIcon folderIcon;
    folderIcon.addFile(":/res/png/datarootnode.png", QSize(), QIcon::Normal, QIcon::Off);
    //folderIcon.addFile(":/icons/datarootnode.png", QSize(), QIcon::Normal, QIcon::On);
    // --- 左侧：树形控件 ---
    //leftTree = new QTreeWidget(ui->widget_left);
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setHeaderLabel("工区--" );
    ui->treeWidget->setIconSize(QSize(20, 20));

    ui->treeWidget->setStyle(QStyleFactory::create("windows")); // 强行使用支持标准连接线的样式包（可选）
    ui->treeWidget->setIndentation(20);                       // 设置合适的层级缩进像素
    ui->treeWidget->setFrameShape(QFrame::NoFrame);


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
    //layout->setContentsMargins(0, 0, 0, 0);

    layout->setContentsMargins(20, 20, 20, 20);
    // 创建表格
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(1);
    table->setHorizontalHeaderLabels({"文件名", "大小", "修改时间"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    //table->setAlternatingRowColors(true); // 开启隔行换色
    table->setSelectionMode(QAbstractItemView::NoSelection); // 禁止默认选中高亮（可选）
    table->horizontalHeader()->setStretchLastSection(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    //table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    table->setColumnWidth(1, 120);
    table->setColumnWidth(2, 120);

    // 设置表头的高度
    table->horizontalHeader()->setFixedHeight(35);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(80);
    table->setShowGrid(false); // 工业软件通常隐藏网格线会显得更干净
    table->setAlternatingRowColors(false);

    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();

    QString qss = R"(QTableWidget {
    background-color: transparent; /* 去掉冗余背景 */
    padding: 10px;                /* 这里给了呼吸感 */

    /*background-color: #141519;  这是底色，也就是你看到的“间隔”颜色 */
    border: none;
    gridline-color: transparent;
    /* 给整个表格容器设置一个顶部内边距，这样表头与第一行之间就会产生自然的空隙
    padding-top: 10px; */
    }

QTableWidget::item {
    /* 关键：给 item 设置 margin，露出底部的 #141519 */
    margin-top: 5px;      /* 上间距 */
    margin-bottom: 5px;   /* 下间距 */
    margin-left: 10px;    /* 左间距 */
    margin-right: 10px;   /* 右间距 */

    /* 这里的背景色是卡片的颜色 */
    background-color: #222222;
    border-radius: 8px;
})";

    table->setStyleSheet(qss);




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
        //table->setItem(i, 0, new QTableWidgetItem(fileInfo.fileName()));
        //table->setCellWidget(i, 0, row);
        m_dasFileList.append(fileInfo.filePath());

        // 大小 (格式化显示)
        qint64 size = fileInfo.size();
        QString sizeStr = QString::number(size / 1024.0, 'f', 2) + " KB";
        //table->setItem(i, 1, new QTableWidgetItem(sizeStr));

        // 修改时间
        QString timeStr = fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss");
        //table->setItem(i, 2, new QTableWidgetItem(timeStr));

        QWidget *fullRowWidget = createCustomRowWidget(fileInfo.fileName(), sizeStr, timeStr);
        table->setCellWidget(i, 0, fullRowWidget);
        //table->setRowHeight(i, 100); // 根据内容调整高度
    }

    setupContextMenu(table);

    //双击处理相当文件
    connect(table, &QTableWidget::cellDoubleClicked,
            this, [=](int row, int column) {
                // 这里写你的逻辑
                if (column > 2)
                    return;
                QTableWidgetItem *item = table->item(row, column);


                if (item) {
                    QString szFile = m_dasFileList.at(row);
                    qDebug() << "双击了:" << item->text()<<szFile;
                } else {
                    qDebug() << "双击了空白单元格";
                }
            });

  //  ProjectMgr::Instance().m_projectFile.setDASPath(path, m_dasFileList);

    layout->addWidget(table);
    // 添加到 Tab 并切换至当前页
    int index = tabWidget->addTab(container, title);
    tabWidget->setCurrentIndex(index);
}

// 辅助函数：绑定信号
void FileTreeListWidget::setupContextMenu(QTableWidget* table) {
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        // 调用通用处理函数，把当前的 table 传进去
        this->showGeneralContextMenu(table, pos);
    });
}

void FileTreeListWidget::initCardList()
{

}



QWidget* FileTreeListWidget::createCustomRowWidget(const QString &fileName, const QString &fileSize, const QString &fileTime)
{
    // 1. 创建容器 Widget
    QWidget *container = new QWidget();
    container->setObjectName("myCard"); // 给对象起个名字

    QString qss = R"(
    /* 使用 'this' 确保样式只应用到 container 自身 */
    /* 合并了背景色、圆角和边框 */
    QWidget#myCard {
        background-color: #222222;
        border-radius: 8px;
        border: 2px none #141519; /* 这里的颜色必须和表格背景色一致 */
    }
    /* 设置悬停样式 */
    QWidget#myCard:hover {
        background-color: #2A2A2A; /* 悬停时变亮 */
        border: 1px solid #444444; /* 保持边框颜色，防止闪烁 */
    }
    )";

    container->setStyleSheet(qss);

   // container->setMouseTracking(true);

    // 3. 主布局 (水平布局)
    QHBoxLayout *mainLayout = new QHBoxLayout(container);
    //mainLayout->setContentsMargins(15, 10, 15, 10); // 左右留白
    mainLayout->setContentsMargins(20, 10, 20, 2);
    mainLayout->setSpacing(20); // 各部分之间的间距

    // --- 左侧区域：图标 + 文件名 + 状态 ---
    QWidget *leftWidget = new QWidget();
    QHBoxLayout *leftLayout = new QHBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(10);

    QLabel *iconLabel = new QLabel();
    {
        iconLabel->setFixedSize(36, 36); // 稍微增大容器，留出内边距空间
        iconLabel->setPixmap(QPixmap(":/res/png/das.png").scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter); // 居中显示

        // 在样式表中添加圆角
        iconLabel->setStyleSheet(
            "background-color: #333333; " // 给图标一个底色，显得不那么飘
            "border-radius: 6px; "        // 圆角是精致的关键
            "padding: 3px;"               // 让图标内部有留白
            );
    }
    //iconLabel->setPixmap(QPixmap(":/res/png/das.png").scaled(24, 24, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)); // 替换为你的图标路径
    //iconLabel->setFixedSize(24, 24);
    //iconLabel->setStyleSheet("background-color: transparent;");
    //  建议：如果图标本身颜色太暗，可以在这里给它加个浅色背景调试一下
   // iconLabel->setStyleSheet("background-color: rgba(255, 0, 0, 50);"); // 临时调试用：红色半透明背景

    /*//以下代码生成一个圆形区域
     {
        // --- 核心修改：绘制圆形背景 + 图标 ---
        QPixmap sourcePixmap(":/res/png/das.png");

        // 1. 创建一个透明画布
        QPixmap finalPixmap(32, 32);
        finalPixmap.fill(Qt::transparent);

        QPainter painter(&finalPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true); // 开启抗锯齿

        // 2. 绘制圆形背景 (比如一个好看的蓝色或深灰色)
        // QColor bgColor(33, 150, 243); // 蓝色
        QColor bgColor(50, 50, 50);    // 深灰色，根据你的 UI 风格调整
        painter.setBrush(bgColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 32, 32);

        // 3. 在圆形背景上绘制图标 (留一点边距，比如 8px)
        QPixmap scaledIcon = sourcePixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(8, 8, 16, 16, scaledIcon);

        painter.end();

        iconLabel->setPixmap(finalPixmap);
        iconLabel->setStyleSheet("background-color: transparent;"); // 确保 label 本身没背景
    }*/

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(4);


    QLabel *nameLabel = new QLabel(fileName);
    nameLabel->setObjectName("fileNameLabel"); // 用于QSS单独设置样式
    nameLabel->setStyleSheet("color: #D5D5D5; font-size: 16px; font-weight: bold;"); // 明确指定白色
    textLayout->addWidget(nameLabel);


    QLabel *fileTimeLabel = new QLabel(fileTime); // 这里你可以传入动态状态
    fileTimeLabel->setFixedWidth(160);
    fileTimeLabel->setAlignment(Qt::AlignLeft);//左对齐
    fileTimeLabel->setObjectName("fileTimeLabel");
    fileTimeLabel->setStyleSheet("color: #A0A0A0; font-size: 12px;font-weight: bold;font-family: Consolas, Courier New, Monospace;");

    QLabel *sizeLabel = new QLabel(fileSize);
    sizeLabel->setAlignment(Qt::AlignRight);//右对齐，保证上下整齐
    sizeLabel->setFixedWidth(200); // 固定宽度，保证对齐
    sizeLabel->setStyleSheet("color: #A0A0A0;font-size: 12px;font-weight: bold;font-family: Consolas, Courier New, Monospace;");//Monospace family

    QHBoxLayout *textLayout2 = new QHBoxLayout();
    textLayout2->addWidget(fileTimeLabel);
    textLayout2->addWidget(sizeLabel);
    textLayout2->setContentsMargins(0, 4, 0, 0);//与文件名上下拉开距离

    textLayout->addLayout(textLayout2);
    textLayout->setContentsMargins(10, 6, 0, 0); // 增加图标与文字的左间距

    leftLayout->addWidget(iconLabel);
    leftLayout->addLayout(textLayout);
    // 让左侧内容靠左
    leftLayout->addStretch();

    // --- 中间区域：大小 ---


    // --- 右侧区域：时间 ---
   /* QLabel *timeLabel = new QLabel(fileTime);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setFixedWidth(160); // 固定宽度，保证对齐
    timeLabel->setStyleSheet("color: #CCCCCC;");*/

    // --- 最右侧：按钮组 ---
    QWidget *btnWidget = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setContentsMargins(5, 5, 100, 0);
    btnLayout->setSpacing(10);

    // 简单的按钮样式，防止按钮背景也是黑的看不见
    QString btnStyle = "QPushButton { color: #FFFFFF; background: transparent; border: 1px solid #444; border-radius: 4px; padding: 2px 0px; }"
                       "QPushButton:hover { background: #555; }"
                       "QPushButton:pressed { background-color: #333; border: 1px solid #222; }";

    // 这里模拟你的 1 2 3 dowr 按钮
    QToolButton *btn1 = new QToolButton();
    QToolButton *btn2 = new QToolButton();
    QToolButton *btn3 = new QToolButton();
    QToolButton *btn4 = new QToolButton();
    btn1->setStyleSheet(btnStyle);
    btn2->setStyleSheet(btnStyle);
    btn3->setStyleSheet(btnStyle);
    btn4->setStyleSheet(btnStyle);

    btn1->setIcon(QIcon(":/res/png/play.png"));
    btn2->setIcon(QIcon(":/res/png/pause.png"));
    btn3->setIcon(QIcon(":/res/png/stop.png"));
    btn4->setIcon(QIcon(":/res/png/recyclebin.png"));

    btnLayout->addWidget(btn1);
    btnLayout->addWidget(btn2);
    btnLayout->addWidget(btn3);
    btnLayout->addWidget(btn4);

    // --- 将所有区域加入主布局 ---
    mainLayout->addWidget(leftWidget);   // 左侧
   // mainLayout->addWidget(sizeLabel);    // 中间大小
   //mainLayout->addWidget(timeLabel);   // 右侧时间
    mainLayout->addWidget(btnWidget);    // 最右侧按钮

    // 如果希望按钮靠最右，可以在左侧和大小之间加伸缩项，或者调整布局策略
   //  mainLayout->setStretch(0, 1); // 让左侧拉伸
    return container;
}
