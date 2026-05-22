/********************************************************************************
** Form generated from reading UI file 'axisrange.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AXISRANGE_H
#define UI_AXISRANGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AxisRange
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *AxisRange)
    {
        if (AxisRange->objectName().isEmpty())
            AxisRange->setObjectName(QString::fromUtf8("AxisRange"));
        AxisRange->resize(265, 55);
        horizontalLayout_2 = new QHBoxLayout(AxisRange);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(AxisRange);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(50, 0));

        horizontalLayout->addWidget(lineEdit);

        label = new QLabel(AxisRange);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(20, 0));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        lineEdit_2 = new QLineEdit(AxisRange);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMinimumSize(QSize(50, 0));
        lineEdit_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lineEdit_2);

        pushButton_2 = new QPushButton(AxisRange);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(AxisRange);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(pushButton);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(AxisRange);

        QMetaObject::connectSlotsByName(AxisRange);
    } // setupUi

    void retranslateUi(QWidget *AxisRange)
    {
        AxisRange->setWindowTitle(QCoreApplication::translate("AxisRange", "Form", nullptr));
        label->setText(QCoreApplication::translate("AxisRange", "-", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AxisRange", "\345\217\226\346\266\210", nullptr));
        pushButton->setText(QCoreApplication::translate("AxisRange", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AxisRange: public Ui_AxisRange {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AXISRANGE_H
