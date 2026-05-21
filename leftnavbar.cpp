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
    btnToggle  = createButton(":/res/button/Home.png", "收起导航");
    btnWorkspace = createButton(":/res/button/user_switch.png", " 工区管理", true);
    btnMonitor   = createButton(":/res/button/curve_config.png", " 实时监测", true);
    btnReport    = createButton(":/res/button/software_config.png", " 成果报告", true);


    // 加一条淡淡的分割线
    QWidget *line = new QWidget(this);
    line->setFixedHeight(1);
    //line->setStyleSheet("background-color: #1a1a1a; margin: 5px 10px;");
    line->setStyleSheet("background-color: #1a1a1a; margin: 5px 10px;");
    mainLayout->addWidget(line);

    mainLayout->addWidget(btnWorkspace);
    mainLayout->addWidget(btnMonitor);
    mainLayout->addWidget(btnReport);

    btnWorkspace->setChecked(true); // 默认选中第一个


    mainLayout->addStretch(1);

    // ==========================================
    // 【底部区域】：系统设置、关于等按钮
    // ==========================================
    btnUser = createButton(":/res/button/view_switch.png", " 个人中心");
    btnSetting = createButton(":/res/button/software_config.png", " 系统设置");

    mainLayout->addWidget(btnUser);
    mainLayout->addWidget(btnSetting);


    mainLayout->addStretch();
    mainLayout->addWidget(line);
    mainLayout->addWidget(btnToggle);//伸缩按键放在最下边
    mainLayout->addWidget(line);
    // ==========================================
    // 信号连接
    // ==========================================
    // 点击折叠按钮
    connect(btnToggle, &QToolButton::clicked, this, &LeftNavBar::slotToggle);

    // 点击业务按钮（通知外部切换 QStackedWidget）
    connect(navGroup, &QButtonGroup::idClicked, this, [this](int id) {
        emit modulesChanged(id);
    });

    QString qss = R"(
        /* 设置整个侧边栏背景为深色（假设你的深色是 #222222） */
        LeftNavBar {
            background-color: #222222;
        }

        /* 核心：设置按钮样式 */
        QToolButton {
            background-color: transparent; /* 关键：背景透明 */
            color: white;                  /* 文字颜色 */
            border: none;                  /* 去掉边框 */
            padding: 10px;                 /* 内边距，让点击区域更好看 */
            text-align: left;              /* 文字左对齐 */
            font-size: 14px;
        }

        /* 鼠标悬停时的效果 */
        QToolButton:hover {
            background-color: rgba(255, 255, 255, 0.1); /* 轻微变亮 */
        }

        /* 选中状态（setChecked(true)）的效果 */
        QToolButton:checked {
            background-color: rgba(255, 255, 255, 0.2); /* 选中时背景更亮一点 */
            color: #00CCFF; /* 选中时文字变色（可选） */
        }
    )";
    this->setStyleSheet(qss);


    // 初始状态：默认收起
    setFixedWidth(60);
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


//使用对象名（Object Name）进行强绑定（适合有多种不同导航栏的情况）
// 在 C++ 中给控件起个名字
//leftNavBar->setObjectName("myLeftNavBar");
/* QSS 中这样写 */
//#myLeftNavBar QToolButton:hover {
//background-color: #333333;
//color: #ffffff;
//}
