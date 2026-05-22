/********************************************************************************
** Form generated from reading UI file 'usercenterwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERCENTERWIDGET_H
#define UI_USERCENTERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserCenterWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *UserCenterWidget)
    {
        if (UserCenterWidget->objectName().isEmpty())
            UserCenterWidget->setObjectName(QString::fromUtf8("UserCenterWidget"));
        UserCenterWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(UserCenterWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_title = new QLabel(UserCenterWidget);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #5dade2;"));
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_title);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(UserCenterWidget);

        QMetaObject::connectSlotsByName(UserCenterWidget);
    } // setupUi

    void retranslateUi(QWidget *UserCenterWidget)
    {
        label_title->setText(QCoreApplication::translate("UserCenterWidget", "\344\270\252\344\272\272\344\270\255\345\277\203", nullptr));
        (void)UserCenterWidget;
    } // retranslateUi

};

namespace Ui {
    class UserCenterWidget: public Ui_UserCenterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERCENTERWIDGET_H
