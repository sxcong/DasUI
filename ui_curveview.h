/********************************************************************************
** Form generated from reading UI file 'curveview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVEVIEW_H
#define UI_CURVEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_CurveView
{
public:
    QVBoxLayout *verticalLayout;
    QCustomPlot *widget;

    void setupUi(QWidget *CurveView)
    {
        if (CurveView->objectName().isEmpty())
            CurveView->setObjectName(QString::fromUtf8("CurveView"));
        CurveView->resize(958, 693);
        CurveView->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(CurveView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(9, 0, -1, 0);
        widget = new QCustomPlot(CurveView);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);


        retranslateUi(CurveView);

        QMetaObject::connectSlotsByName(CurveView);
    } // setupUi

    void retranslateUi(QWidget *CurveView)
    {
        CurveView->setWindowTitle(QCoreApplication::translate("CurveView", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CurveView: public Ui_CurveView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVEVIEW_H
