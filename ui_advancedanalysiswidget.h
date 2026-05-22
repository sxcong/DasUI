/********************************************************************************
** Form generated from reading UI file 'advancedanalysiswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDANALYSISWIDGET_H
#define UI_ADVANCEDANALYSISWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdvancedAnalysisWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_placeholder;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AdvancedAnalysisWidget)
    {
        if (AdvancedAnalysisWidget->objectName().isEmpty())
            AdvancedAnalysisWidget->setObjectName(QString::fromUtf8("AdvancedAnalysisWidget"));
        AdvancedAnalysisWidget->resize(371, 273);
        verticalLayout = new QVBoxLayout(AdvancedAnalysisWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_title = new QLabel(AdvancedAnalysisWidget);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #5dade2;"));

        verticalLayout->addWidget(label_title);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label_placeholder = new QLabel(AdvancedAnalysisWidget);
        label_placeholder->setObjectName(QString::fromUtf8("label_placeholder"));
        label_placeholder->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #5dade2;"));
        label_placeholder->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_placeholder);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(AdvancedAnalysisWidget);

        QMetaObject::connectSlotsByName(AdvancedAnalysisWidget);
    } // setupUi

    void retranslateUi(QWidget *AdvancedAnalysisWidget)
    {
        label_title->setText(QCoreApplication::translate("AdvancedAnalysisWidget", "TextLabel", nullptr));
        label_placeholder->setText(QCoreApplication::translate("AdvancedAnalysisWidget", "\351\253\230\347\272\247\345\210\206\346\236\220\346\250\241\345\235\227 - \347\256\227\346\263\225\345\244\204\347\220\206\344\270\255...", nullptr));
        (void)AdvancedAnalysisWidget;
    } // retranslateUi

};

namespace Ui {
    class AdvancedAnalysisWidget: public Ui_AdvancedAnalysisWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDANALYSISWIDGET_H
