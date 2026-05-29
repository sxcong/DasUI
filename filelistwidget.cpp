#include "filelistwidget.h"
#include "ui_filelistwidget.h"
#include "waterfalldialog.h"
#include <QDir>
#include "fbeplotdialog.h" // Include the new FBE plot dialog
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QMenu>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QEvent>

#include <QDir>
#include <QMenu>
#include <QAction>

#include <QFileDialog>

#include <QHeaderView>
#include <QElapsedTimer>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QStyleFactory>
#include <QMessageBox>
#include "projectmgr.h"
#include "spectrumanalysiswidget.h"
#include "lfdaswidget.h"
#include "fbeenergywidget.h"
#include "downsamplewidget.h"
#include "spectrumdbwidget.h"

FileListWidget::FileListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileListWidget)
{
    ui->setupUi(this);
    initTable(ui->tableWidget);
}

FileListWidget::~FileListWidget()
{
    delete ui;
}

QStringList GmGetByteUnits()
{
    QStringList units;
    units << "Bytes" << "KB" << "MB" << "GB" << "TB";
    return units;
}


QString formatBytes(long long bytes) {
    double size = static_cast<double>(bytes);
    static const QStringList units = { "B", "KB", "MB", "GB", "TB" };
    int i = 0;

    // 每次除以1024，直到找到合适的单位
    while (size >= 1024 && i < units.size() - 1) {
        size /= 1024;
        i++;
    }

    // 返回格式化后的字符串，保留两位小数
    // 例如：1.25 GB
    return QString("%1 %2").arg(size, 0, 'f', 2).arg(units[i]);
}


void FileListWidget::setPath(const QString &title, const QString &path)
{
    m_title = title;
    m_path = path;

    m_dasFileList.clear();
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.h5");
    // QDir::Files 表示只列出文件（不包含目录）
    // QDir::NoDotAndDotDot 表示不列出 "." 和 ".."
    //dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList(QDir::Files); // 仅获取文件

    ui->tableWidget->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        //1 文件名
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(fileInfo.fileName()));
        m_dasFileList.append(fileInfo.filePath());

        //2 大小 (格式化显示)
        qint64 size = fileInfo.size();
        QString sizeStr = formatBytes(size);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sizeStr));

        //3 修改时间
        QString timeStr = fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss");
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(timeStr));
        //4 操作
        QWidget *fullRowWidget = createCustomRowWidget(fileInfo.fileName(), sizeStr, timeStr);
        ui->tableWidget->setCellWidget(i, 3, fullRowWidget);
    }

    setupContextMenu(ui->tableWidget);

    //双击处理相当文件
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked,
            this, [=](int row, int column) {
                // 这里写你的逻辑
                if (column > 2)
                    return;
                QTableWidgetItem *item = ui->tableWidget->item(row, column);


                if (item) {
                    QString szFile = m_dasFileList.at(row);
                    qDebug() << "双击了:" << item->text()<<szFile;
                } else {
                    qDebug() << "双击了空白单元格";
                }
            });
}


// 辅助函数：绑定信号
void FileListWidget::setupContextMenu(QTableWidget* table) {
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        // 调用通用处理函数，把当前的 table 传进去
        this->showGeneralContextMenu(table, pos);
    });
}

// 通用处理函数
void FileListWidget::showGeneralContextMenu(QTableWidget* table, const QPoint &pos) {

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
        openLfDasFiles(QFileInfo(szFile).absolutePath());
    });

    QAction *act3 = new QAction("FBE分频能量瀑布图", &menu);
    connect(act3, &QAction::triggered, this, [=]() {
        openFbeEnergyFiles(QFileInfo(szFile).absolutePath());
    });

    QAction *act4 = new QAction("F-K", &menu);
    connect(act4, &QAction::triggered, this, [=]() {
        qDebug()<<"szFile = "<<szFile;
        // FKDemoDialog dlg;
        // dlg.setH5File(szFile);
        // dlg.exec();
    });

    QAction *act5 = new QAction("降采样原始数据图", &menu);
    connect(act5, &QAction::triggered, this, [=]() {
        openDownsampleFiles(QFileInfo(szFile).absolutePath());
    });

    QAction *act6 = new QAction("频谱图", &menu);
    connect(act6, &QAction::triggered, this, [=]() {
        openSpectrumDbFiles(QFileInfo(szFile).absolutePath());
    });

    menu.addAction(act1);
    menu.addAction(act2);
    menu.addAction(act3);
    menu.addAction(act4);
    menu.addAction(act5);
    menu.addAction(act6);


    // 将点击位置 pos 转换为全局坐标
    QPoint globalPos = table->viewport()->mapToGlobal(pos);
    menu.exec(globalPos);
}

void FileListWidget::initTable(QTableWidget *table)
{
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"文件名", "大小", "修改时间", "操作"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //table->setAlternatingRowColors(true); // 开启隔行换色
    //table->setSelectionMode(QAbstractItemView::NoSelection); // 禁止默认选中高亮（可选）
    table->horizontalHeader()->setStretchLastSection(true);
    //table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    //table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    table->setColumnWidth(0, 400);
    table->setColumnWidth(1, 120);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 200);

    // 设置表头的高度
    //table->horizontalHeader()->setFixedHeight(35);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //table->verticalHeader()->setDefaultSectionSize(60);
    table->setShowGrid(false); // 工业软件通常隐藏网格线会显得更干净
    table->setAlternatingRowColors(false);

    table->verticalHeader()->hide();
    //table->horizontalHeader()->hide();
    // 在构造函数或 UI 初始化位置添加

    // 以下几行使鼠标hover时整行变色，且不影响点击选中
    table->setMouseTracking(true);
    table->viewport()->setMouseTracking(true);

    HoverRowDelegate *delegate = new HoverRowDelegate(table);
    table->setItemDelegate(delegate);

    connect(table, &QTableWidget::cellEntered, this, [delegate, table](int row, int){
        delegate->hoveredRow = row;
        table->viewport()->update();
    });

    // 鼠标离开表格时清除 hover 高亮
    table->viewport()->installEventFilter(this);


    table->horizontalHeader()->setFixedHeight(50);
    table->verticalHeader()->setDefaultSectionSize(50);

    QString qss = R"(
    /* 1. 表格整体 */
    QTableWidget {
        background-color: #141519;
        border: none;
        outline: none;
        padding-left: 10px;
        padding-right: 10px;
    }

    /* 2. 数据行卡片 */
    QTableWidget::item {
        background-color: #222222;
        color: #D1D1D1;
        border: none;
        margin-bottom: 6px;
        padding-left: 15px;
    }

    /* 选中状态 (Selected) - 整行变色靠 cellEntered + selectRow */
    QTableWidget::item:selected {
        background-color: #4A4A4A; /* 明显的选中色 */
        color: #FFFFFF;

 selection-background-color: #4A4A4A;
    }
    /* 5. 表头 */
    QTableWidget QHeaderView::section {
        background-color: #222222;
        color: #ffffff;
        border: none;
        margin-bottom: 8px;
        font-weight: bold;
        border-right: 1px solid #333333;
    }
)";
    table->setStyleSheet(qss);
}

bool FileListWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Leave && obj == ui->tableWidget->viewport()) {
        auto *d = qobject_cast<HoverRowDelegate*>(ui->tableWidget->itemDelegate());
        if (d && d->hoveredRow >= 0) {
            d->hoveredRow = -1;
            ui->tableWidget->viewport()->update();
        }
    }
    return QWidget::eventFilter(obj, event);
}


QWidget* FileListWidget::createCustomRowWidget(const QString &fileName, const QString &fileSize, const QString &fileTime)
{
    // 1. 创建容器 Widget
    QWidget *container = new QWidget();
    container->setObjectName("myCard"); // 给对象起个名字

    // 容器背景透明，让 delegate 统一绘制行背景色（hover/选中同步）
    container->setAttribute(Qt::WA_TranslucentBackground);
    container->setStyleSheet("QWidget#myCard { background: transparent; }");

    // container->setMouseTracking(true);

    // 3. 主布局 (水平布局)
    QHBoxLayout *mainLayout = new QHBoxLayout(container);
    //mainLayout->setContentsMargins(15, 10, 15, 10); // 左右留白
    mainLayout->setContentsMargins(8, 0, 8, 0);
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



    // --- 最右侧：按钮组 ---
    QWidget *btnWidget = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(6);
    btnLayout->setAlignment(Qt::AlignCenter); // 按钮组在列内居中

    // 简单的按钮样式，防止按钮背景也是黑的看不见
    /*QString btnStyle = "QPushButton { color: #FFFFFF; background: transparent; border: 1px solid #444; border-radius: 4px; padding: 2px 0px; }"
                       "QPushButton:hover { background: #555; }"
                       "QPushButton:pressed { background-color: #333; border: 1px solid #222; }";*/

    QString btnStyle = "QToolButton { color: #FFFFFF; background: #C1C1C1; border: 1px solid #B0B0B0; border-radius: 4px; }"
                       "QToolButton:hover { background: #357591; border-color: #5BA3D6; }"
                       "QToolButton:pressed { background-color: #333; border: 1px solid #222; }";

    // 这里模拟你的 1 2 3 dowr 按钮
    QToolButton *btn1 = new QToolButton(this);
    QToolButton *btn2 = new QToolButton(this);
    QToolButton *btn3 = new QToolButton(this);
    QToolButton *btn4 = new QToolButton(this);
    QToolButton *btn5 = new QToolButton(this);
    QToolButton *btn6 = new QToolButton(this);
    btn1->setToolTip("tip1");
    btn2->setToolTip("tip2");
    btn3->setToolTip("LF-DAS瀑布图");
    btn4->setToolTip("FBE分频能量瀑布图");
    btn5->setToolTip("降采样原始数据图");
    btn6->setToolTip("频谱图");


    connect(btn1, &QToolButton::clicked, this, [fileName, this]() {
        QString folderPath = this->property("folderPath").toString();
        QString szFullFile = QDir::toNativeSeparators(QDir(folderPath).absoluteFilePath(fileName));
        WaterfallDialog *dlg = new WaterfallDialog(szFullFile, this);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    });

    connect(btn2, &QToolButton::clicked, this, [fileName, this]() {
       // QApplication::setOverrideCursor(Qt::WaitCursor);
        QString folderPath = this->property("folderPath").toString();
        QString szFullFile = QDir::toNativeSeparators(QDir(folderPath).absoluteFilePath(fileName));
        // Open FBE Plot dialog
        /*FbePlotDialog *dlg = new FbePlotDialog(szFullFile, this);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();*/

        SpectrumAnalysisWidget* pWidget = new SpectrumAnalysisWidget("d:/output/5/band_energy.merge.bin", "d:/output/5/spectrum.merge.bin", this);
        pWidget->setAttribute(Qt::WA_DeleteOnClose);
        pWidget->resize(1000, 600);
        pWidget->exec();
       // QApplication::restoreOverrideCursor();
    });

    connect(btn3, &QToolButton::clicked, this, [this]() {
        QString folderPath = this->property("folderPath").toString();
        openLfDasFiles(folderPath);
    });

    connect(btn4, &QToolButton::clicked, this, [this]() {
        QString folderPath = this->property("folderPath").toString();
        openFbeEnergyFiles(folderPath);
    });

    connect(btn5, &QToolButton::clicked, this, [this]() {
        QString folderPath = this->property("folderPath").toString();
        openDownsampleFiles(folderPath);
    });

    connect(btn6, &QToolButton::clicked, this, [this]() {
        QString folderPath = this->property("folderPath").toString();
        openSpectrumDbFiles(folderPath);
    });

    QList<QToolButton*> btnList = {btn1, btn2, btn3, btn4, btn5, btn6};
    QList<QString> iconList = {
        ":/res/png/play.png",
        ":/res/png/pause.png",
        ":/res/png/stop.png",
        ":/res/png/recyclebin.png",
        ":/res/png/play.png",
        ":/res/png/pause.png"
    };

    for (int i=0; i<btnList.size(); ++i) {
        QToolButton *btn = btnList[i];
        btn->setFixedSize(24, 24); // 解决变形问题：设置固定的正方形尺寸
        btn->setIconSize(QSize(18, 18)); // 稍微增大图标尺寸，使其在按钮内更显眼
        btn->setStyleSheet(btnStyle);
        btn->setIcon(QIcon(iconList[i]));
        btn->setCursor(Qt::PointingHandCursor);
        // 设置简单的策略防止拉伸
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    btnLayout->addWidget(btn1);
    btnLayout->addWidget(btn2);
    btnLayout->addWidget(btn3);
    btnLayout->addWidget(btn4);
    btnLayout->addWidget(btn5);
    btnLayout->addWidget(btn6);



    // --- 将所有区域加入主布局 ---
    leftWidget->hide();
    //mainLayout->addWidget(leftWidget);   // 左侧
    // mainLayout->addWidget(sizeLabel);    // 中间大小
    //mainLayout->addWidget(timeLabel);   // 右侧时间
    mainLayout->addWidget(btnWidget);    // 最右侧按钮

    // 如果希望按钮靠最右，可以在左侧和大小之间加伸缩项，或者调整布局策略
    //  mainLayout->setStretch(0, 1); // 让左侧拉伸
    return container;
}

void FileListWidget::openLfDasFiles(const QString& initialDir)
{
    QString startDir = initialDir;
    if (startDir.isEmpty()) {
        startDir = this->property("folderPath").toString();
    }
    if (startDir.isEmpty()) {
        startDir = QDir::homePath();
    }

    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择 LF-DAS 二进制文件",
        startDir,
        "LF-DAS Binary (*.lfdas *.bin);;All Files (*)");
    if (files.isEmpty()) {
        return;
    }

    auto* widget = new LfDasWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->resize(1200, 720);
    widget->setFiles(files);
    widget->show();
}

void FileListWidget::openDownsampleFiles(const QString& initialDir)
{
    QString startDir = initialDir;
    if (startDir.isEmpty()) {
        startDir = this->property("folderPath").toString();
    }
    if (startDir.isEmpty()) {
        startDir = QDir::homePath();
    }

    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择降采样二进制文件",
        startDir,
        "Downsample Binary (*.down);;Binary Files (*.bin);;All Files (*)");
    if (files.isEmpty()) {
        return;
    }

    auto* widget = new DownsampleWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->resize(1200, 720);
    widget->setFiles(files);
    widget->show();
}

void FileListWidget::openSpectrumDbFiles(const QString& initialDir)
{
    QString startDir = initialDir;
    if (startDir.isEmpty()) {
        startDir = this->property("folderPath").toString();
    }
    if (startDir.isEmpty()) {
        startDir = QDir::homePath();
    }

    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择 spectrum_db 二进制文件",
        startDir,
        "Spectrum DB (*.spectrum_db);;Binary Files (*.bin);;All Files (*)");
    if (files.isEmpty()) {
        return;
    }

    auto* widget = new SpectrumDbWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->resize(1200, 720);
    widget->setFiles(files);
    widget->show();
}

void FileListWidget::openFbeEnergyFiles(const QString& initialDir)
{
    QString startDir = initialDir;
    if (startDir.isEmpty()) {
        startDir = this->property("folderPath").toString();
    }
    if (startDir.isEmpty()) {
        startDir = QDir::homePath();
    }

    const QStringList files = QFileDialog::getOpenFileNames(
        this,
        "选择 FBE 分频能量二进制文件",
        startDir,
        "FBE Binary (*.fbe);;Binary Files (*.bin);;All Files (*)");
    if (files.isEmpty()) {
        return;
    }

    auto* widget = new FbeEnergyWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->resize(1200, 720);
    widget->setFiles(files);
    widget->show();
}
