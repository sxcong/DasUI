#ifndef FRAMELINE_H
#define FRAMELINE_H

#include <QObject>
#include <QWidget>
#include "QDebug"

class FrameLine : public QWidget
{
    Q_OBJECT
public:
    explicit FrameLine(QWidget *parent = nullptr);

    void setBoxHeightSize(int height_Up,int height_Down)
    {
//        qDebug()<<__LINE__<<height_Up<<height_Down;
        m_height_Up = height_Up;
        m_height_Down =height_Down;
        update();
    }
    void setImage(QImage p_pic)
    {
        m_pic = p_pic;

    }
signals:

public slots:

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
     QImage m_pic;

     int m_height_Up = 0;
     int m_height_Down =0;

};

#endif // FRAMELINE_H
