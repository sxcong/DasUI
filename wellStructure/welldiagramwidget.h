#ifndef WELLDIAGRAMWIDGET_H
#define WELLDIAGRAMWIDGET_H

#include "paintarea.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include "configmgr.h"
namespace Ui {
class WellDiagramWidget;
}

//void intToQColor(int iColor, QColor &qcolor)
//{
//    int redColor = iColor & 0xff;
//    int greenColor = (iColor >> 8) & 0xff;
//    int blueColor = (iColor >> 16) & 0xff;

//    qcolor = QColor(redColor, greenColor, blueColor);
//}

//int QColorToInt(const QColor &qcolor)
//{
//    int iColor = qcolor.red() | qcolor.green()<<8 | qcolor.blue()<<16;
//    return iColor;
//}

class WellDiagramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WellDiagramWidget(QWidget *parent = nullptr);

    void initWidgets();
    ~WellDiagramWidget();

private slots:
    void on_pushButton_2_clicked();

    void on_comboBox_type_currentIndexChanged(int index);
    void on_pushButton_4_clicked();

    QPixmap getNewPixMap(){return m_pixmap;}
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::WellDiagramWidget *ui;
    PaintArea * m_paintArea = nullptr;
    int m_index  =0;
    QPixmap m_pixmap;

    QVector<QColor> m_colors ;
};

#endif // WELLDIAGRAMWIDGET_H
