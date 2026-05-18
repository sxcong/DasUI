#ifndef AXISRANGE_H
#define AXISRANGE_H

#include <QWidget>
#include "QDialog"
namespace Ui {
class AxisRange;
}

class AxisRange : public QDialog
{
    Q_OBJECT

public:
    explicit AxisRange(QWidget *parent = nullptr);
    ~AxisRange();
    void getValue(int &min ,int &max);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AxisRange *ui;
    int m_min = 0;
    int m_max = 1000;

};

#endif // AXISRANGE_H
