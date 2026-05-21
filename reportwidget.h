#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>

namespace Ui {
class ReportWidget;
}

class ReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWidget(QWidget *parent = nullptr);
    ~ReportWidget();

private:
    Ui::ReportWidget *ui;
};

#endif // REPORTWIDGET_H