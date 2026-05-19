#ifndef LEFTNAVBAR_H
#define LEFTNAVBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QSpacerItem>
#include <QDebug>
#include <QPainter>

namespace Ui {
class LeftNavBar;
}

class LeftNavBar : public QWidget
{
    Q_OBJECT

public:
    explicit LeftNavBar(QWidget *parent = nullptr);
    ~LeftNavBar();
signals:
    void modulesChanged(int index); // 业务切换信号

private slots:
    void slotToggle();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QToolButton* createButton(const QString &iconPath, const QString &text, bool isSubModule = false);
private:
    Ui::LeftNavBar *ui;
    QVBoxLayout *mainLayout;
    QButtonGroup *navGroup;
    bool isExpanded;

    QToolButton *btnToggle, *btnWorkspace, *btnMonitor, *btnReport, *btnUser, *btnSetting;
    QList<QToolButton*> allButtons;
};

#endif // LEFTNAVBAR_H
