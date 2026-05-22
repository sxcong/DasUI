/********************************************************************************
** Form generated from reading UI file 'systemsettingswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMSETTINGSWIDGET_H
#define UI_SYSTEMSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SystemSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *SystemSettingsWidget)
    {
        if (SystemSettingsWidget->objectName().isEmpty())
            SystemSettingsWidget->setObjectName(QString::fromUtf8("SystemSettingsWidget"));
        SystemSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(SystemSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_title = new QLabel(SystemSettingsWidget);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #e67e22;"));
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_title);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(SystemSettingsWidget);

        QMetaObject::connectSlotsByName(SystemSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *SystemSettingsWidget)
    {
        label_title->setText(QCoreApplication::translate("SystemSettingsWidget", "\347\263\273\347\273\237\350\256\276\347\275\256", nullptr));
        (void)SystemSettingsWidget;
    } // retranslateUi

};

namespace Ui {
    class SystemSettingsWidget: public Ui_SystemSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMSETTINGSWIDGET_H
