/********************************************************************************
** Form generated from reading UI file 'leftnavbar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEFTNAVBAR_H
#define UI_LEFTNAVBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LeftNavBar
{
public:

    void setupUi(QWidget *LeftNavBar)
    {
        if (LeftNavBar->objectName().isEmpty())
            LeftNavBar->setObjectName(QString::fromUtf8("LeftNavBar"));
        LeftNavBar->resize(207, 577);

        retranslateUi(LeftNavBar);

        QMetaObject::connectSlotsByName(LeftNavBar);
    } // setupUi

    void retranslateUi(QWidget *LeftNavBar)
    {
        LeftNavBar->setWindowTitle(QCoreApplication::translate("LeftNavBar", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LeftNavBar: public Ui_LeftNavBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEFTNAVBAR_H
