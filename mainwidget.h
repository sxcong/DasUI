#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class LeftNavBar;
class QStackedWidget;
class WellWidget;
class FileTreeListWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void setIndex(int index);
private:
    Ui::MainWidget *ui;
    LeftNavBar *leftNavBar;
    QStackedWidget *rightStack;

    WellWidget* m_pWellWidget = nullptr;
    FileTreeListWidget* m_pFileTreeListWidget = nullptr;


};

#endif // MAINWIDGET_H
