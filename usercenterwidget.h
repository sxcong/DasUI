#ifndef USERCENTERWIDGET_H
#define USERCENTERWIDGET_H

#include <QWidget>

namespace Ui {
class UserCenterWidget;
}

class UserCenterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserCenterWidget(QWidget *parent = nullptr);
    ~UserCenterWidget();

private:
    Ui::UserCenterWidget *ui;
};

#endif // USERCENTERWIDGET_H