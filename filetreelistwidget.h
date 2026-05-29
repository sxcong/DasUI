#ifndef FILETREELISTWIDGET_H
#define FILETREELISTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QTableWidget>

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSplitter>
#include <QSplitterHandle>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainterPath>
#include <QProxyStyle>
#include <QDebug>
#include <QToolButton>

// 自定义树控件展开/收起箭头：VS Code 风格的三角箭头
class ArrowTreeStyle : public QProxyStyle {
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override {
        if (element == PE_IndicatorBranch) {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            
            // 填充背景以覆盖默认的绘制
            painter->fillRect(option->rect, QColor("#141519"));

            if (option->state & State_Children) {
                QRect r = option->rect;
                int s = 6; // 箭头半边长
                QPointF c = r.center();
                
                QPolygonF arrow;
                if (option->state & State_Open) {
                    // 向下的等腰三角形
                    arrow << QPointF(c.x() - s, c.y() - s / 1.5)
                          << QPointF(c.x() + s, c.y() - s / 1.5)
                          << QPointF(c.x(),     c.y() + s / 1.5);
                } else {
                    // 向右的等腰三角形
                    arrow << QPointF(c.x() - s / 1.5, c.y() - s)
                          << QPointF(c.x() - s / 1.5, c.y() + s)
                          << QPointF(c.x() + s / 1.5,   c.y());
                }
                
                // 选中行时箭头变亮
                QColor arrowColor = (option->state & State_Selected) ? QColor("#FFFFFF") : QColor("#888888");
                painter->setPen(Qt::NoPen);
                painter->setBrush(arrowColor);
                painter->drawPolygon(arrow);
            }
            painter->restore();
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

namespace Ui {
class FileTreeListWidget;
}

// 自定义支持动态变宽的 SplitterHandle
class AnimatedSplitterHandle : public QSplitterHandle {
    Q_OBJECT
    Q_PROPERTY(int handleWidth READ handleWidth WRITE setHandleWidth)

public:
    AnimatedSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
        : QSplitterHandle(orientation, parent) {
        // 开启鼠标追踪，以便响应悬停等事件
        setMouseTracking(true);
        setAttribute(Qt::WA_Hover, true);

        // 初始化动画对象
        animation = new QPropertyAnimation(this, "handleWidth", this);
        animation->setDuration(150); // 动画持续时间（毫秒）
        animation->setEasingCurve(QEasingCurve::OutQuad); // 缓动曲线，让动画更自然
    }

    // 获取当前的逻辑宽度（用于动画插值）
    int handleWidth() const {
        return orientation() == Qt::Horizontal ? width() : height();
    }

    // 设置宽度（动画的目标属性）
    void setHandleWidth(int w) {
        if (orientation() == Qt::Horizontal) {
            setMinimumWidth(w);
            setMaximumWidth(w);
        } else {
            setMinimumHeight(w);
            setMaximumHeight(w);
        }
    }

protected:
    // 鼠标按下时：触发动画，变宽（例如变为 8 像素）
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            animation->setStartValue(handleWidth());
            animation->setEndValue(8); // 拖动时的目标宽度
            animation->start();
        }
        QSplitterHandle::mousePressEvent(event);
    }

    // 鼠标释放时：触发动画，恢复原宽（例如恢复为 3 像素）
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            animation->setStartValue(handleWidth());
            animation->setEndValue(2); // 恢复的默认宽度
            animation->start();
        }
        QSplitterHandle::mouseReleaseEvent(event);
    }

    // 自定义绘制背景色（配合你之前的变色需求）
    void paintEvent(QPaintEvent *event) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);

        // 根据 pressed 状态绘制不同的颜色
        if (opt.state & QStyle::State_Sunken) {
            //p.fillRect(rect(), QColor("#005a9e")); // 按下时的深色
            p.fillRect(rect(), QColor("#357591")); // 悬停时的蓝色

        } else if (opt.state & QStyle::State_MouseOver) {
            //p.fillRect(rect(), QColor("#007acc")); // 悬停时的蓝色
            p.fillRect(rect(), QColor("#357591")); // 悬停时的蓝色

        } else {
            p.fillRect(rect(), QColor("#2b2b2c")); // 默认深灰色
        }
    }

private:
    QPropertyAnimation *animation;
};

class AnimatedSplitter : public QSplitter {
public:
    AnimatedSplitter(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSplitter(orientation, parent) {
        setHandleWidth(3); // 设置初始默认宽度
    }

protected:
    // 核心：让 Splitter 使用我们自定义的 Handle
    QSplitterHandle *createHandle() override {
        return new AnimatedSplitterHandle(orientation(), this);
    }
};



class FileTreeListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTreeListWidget(QWidget *parent = nullptr);
    ~FileTreeListWidget();

public slots:
    void openLfDasFilesFromCurrentFolder();
    void openFbeEnergyFilesFromCurrentFolder();
    void openDownsampleFilesFromCurrentFolder();
    void openSpectrumDbFilesFromCurrentFolder();

private:
    void setupCentralLayout();
    void removeFolder(const QString& szPath);
    void removeTab(const QString &title, const QString &path);
    void createNewTab(const QString &title, const QString &path);
    void loadProjectPath();
    QString currentFolderPath() const;

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);
    void showTreeContextMenu(const QPoint &pos);
    void showGeneralContextMenu(QTableWidget* table, const QPoint &pos);
    void importFolder();

private:
    Ui::FileTreeListWidget *ui;
    QTabWidget* tabWidget;
    AnimatedSplitter  *mainSplitter;


    //QTreeWidget *leftTree;
    QTreeWidgetItem *wellItem;//井节点
    QTreeWidgetItem *dasItem; //DAS节点
    QTreeWidgetItem *dtsItem; //DTS节点
    QWidget *rightContainer;

    QStringList m_dasFileList;

    QToolButton *addButton;
    QToolButton *openButton;
};

#endif // FILETREELISTWIDGET_H
