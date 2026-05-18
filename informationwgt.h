#ifndef INFORMATIONWGT_H
#define INFORMATIONWGT_H

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class InformationWgt;
}

class InformationWgt : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWgt(QWidget *parent = nullptr);
    ~InformationWgt();
    void initTableWidget(QTableWidget *pTable);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::InformationWgt *ui;

    // QPaintDevice interface

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    QPixmap pic;

    //InfoMgr m_oilWellInfo;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
};

#endif // INFORMATIONWGT_H
