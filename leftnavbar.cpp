#include "leftnavbar.h"
#include "ui_leftnavbar.h"
#include <QFile>
#include <QStyleOption>

LeftNavBar::LeftNavBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LeftNavBar)
{
    ui->setupUi(this);

    // 1. 设置外层整体垂直布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. 核心：创建单选按钮组（只针对业务模块互斥）
    navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);

    // ==========================================
    // 【顶部区域】：折叠控制 + 核心业务模块
    // ==========================================
    btnToggle  = createButton(":/icons/menu.png", "收起导航");
    btnWorkspace = createButton(":/icons/workspace.png", " 工区管理", true);
    btnMonitor   = createButton(":/icons/monitor.png", " 实时监测", true);
    btnReport    = createButton(":/icons/report.png", " 成果报告", true);

    mainLayout->addWidget(btnToggle);
    // 加一条淡淡的分割线
    QWidget *line = new QWidget(this);
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #1a1a1a; margin: 5px 10px;");
    mainLayout->addWidget(line);

    mainLayout->addWidget(btnWorkspace);
    mainLayout->addWidget(btnMonitor);
    mainLayout->addWidget(btnReport);

    btnWorkspace->setChecked(true); // 默认选中第一个

    // ==========================================
    // 【中间区域】：放置一个神奇的弹簧（Spacer）
    // 它会把上下的组件往两边拼命推，从而实现中间留空
    // ==========================================
    mainLayout->addStretch();

    // ==========================================
    // 【底部区域】：系统设置、关于等按钮
    // ==========================================
    btnUser = createButton(":/icons/user.png", " 个人中心");
    btnSetting = createButton(":/icons/setting.png", " 系统设置");

    mainLayout->addWidget(btnUser);
    mainLayout->addWidget(btnSetting);

    // 初始状态：默认收起
    setFixedWidth(60);

    // ==========================================
    // 信号连接
    // ==========================================
    // 点击折叠按钮
    connect(btnToggle, &QToolButton::clicked, this, &LeftNavBar::slotToggle);

    // 点击业务按钮（通知外部切换 QStackedWidget）
    connect(navGroup, &QButtonGroup::idClicked, this, [this](int id) {
        emit modulesChanged(id);
    });


    QFile qssFile(":/qss/leftnavi.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(qssFile.readAll());
        setStyleSheet(qss);
        qssFile.close();
    }

    //setupStylesheetText();
}

LeftNavBar::~LeftNavBar()
{
    delete ui;
}

void LeftNavBar::slotToggle() {
    if (isExpanded) {
        // 收起：只显示图标
        for (auto btn : allButtons) {
            btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        }
        setFixedWidth(60);
        btnToggle->setText("展开导航");
        isExpanded = false;
    } else {
        // 展开：显示图标加文字
        for (auto btn : allButtons) {
            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        }
        setFixedWidth(160);
        btnToggle->setText(" 收起导航");
        isExpanded = true;
    }
}

// 辅助函数：快速创建统一风格的按钮
QToolButton* LeftNavBar::createButton(const QString &iconPath, const QString &text, bool isSubModule /*= false*/) {
    QToolButton *btn = new QToolButton(this);
    btn->setIcon(QIcon(iconPath));
    btn->setText(text);
    btn->setToolTip(text.trimmed());
    btn->setIconSize(QSize(24, 24));
    btn->setToolButtonStyle(Qt::ToolButtonIconOnly); // 默认只显图标
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // 横向撑满

    if (isSubModule) {
        btn->setCheckable(true);
        // 把业务逻辑按钮按顺序编 ID (0, 1, 2...) 方便右侧 StackedWidget 切换
        navGroup->addButton(btn, navGroup->buttons().size());
    }

    allButtons.append(btn);
    return btn;
}

void LeftNavBar::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event); // 保留父类原有的绘制
}

//可以用文件，也可以直接用下面文本
void LeftNavBar::setupStylesheetText() {
    QString qss = R"(
        /* ===================================================================
           1. 左侧竖向窄条导航美化
           =================================================================== */
        /* 假设你的左侧窄条载体叫 leftNavContainer */
        LeftNavBar {
            background-color: #1e1e1e; /* 更深的底色，衬托出层次 */
            border-right: 1px solid #2d2d2d;
        }

        LeftNavBar QToolButton {
            background-color: transparent;
            border: none;
            padding: 12px;
            margin: 4px 6px;
            border-radius: 4px; /* 按钮微微圆角，更现代 */
        }

        /* 鼠标悬停在左侧图标上 */
        LeftNavBar QToolButton:hover {
            background-color: #2a2a2a;
        }

        /* 类似你图中第二个选中的蓝色按钮效果 */
        LeftNavBar QToolButton:checked {
            background-color: #2b364c; /* 淡淡的科技蓝底色 */
            border-left: 3px solid #3574F0; /* 标志性高亮左边条 */
        }


        /* ===================================================================
           2. 中间树状图上方工具栏 (工区管理表头) 美化
           =================================================================== */
        /* 假设这个表头区域叫 treeHeaderWidget */
        #treeHeaderWidget {
            background-color: #262626; /* 稍淡的灰色 */
            border-bottom: 1px solid #1a1a1a;
            min-height: 35px;
        }

        /* "工区管理:" 标签 */
        #treeHeaderWidget QLabel {
            font-weight: bold;
            color: #ffffff;
            font-size: 13px;
            padding-left: 8px;
        }

        /* 表头上的 +工区列表、×添加工区 按钮 */
        #treeHeaderWidget QToolButton {
            background-color: transparent;
            color: #b5b5b5; /* 默认浅灰，不刺眼 */
            font-size: 12px;
            border: none;
            padding: 4px 8px;
            margin: 2px;
            border-radius: 3px;
        }

        /* 鼠标划过表头按钮 */
        #treeHeaderWidget QToolButton:hover {
            background-color: #333333;
            color: #ffffff; /* 划过时文字变亮 */
        }

        /* 鼠标按下表头按钮 */
        #treeHeaderWidget QToolButton:pressed {
            background-color: #444444;
        }
        )";
    this->setStyleSheet(qss);
}

//使用对象名（Object Name）进行强绑定（适合有多种不同导航栏的情况）
// 在 C++ 中给控件起个名字
//leftNavBar->setObjectName("myLeftNavBar");
/* QSS 中这样写 */
//#myLeftNavBar QToolButton:hover {
//background-color: #333333;
//color: #ffffff;
//}
