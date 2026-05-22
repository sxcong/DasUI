/********************************************************************************
** Form generated from reading UI file 'welldiagramwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELLDIAGRAMWIDGET_H
#define UI_WELLDIAGRAMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <paintarea.h>

QT_BEGIN_NAMESPACE

class Ui_WellDiagramWidget
{
public:
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    PaintArea *widget_mainpaint;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox_type;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QSpinBox *spinBox_depth_3;
    QSpinBox *spinBox_depth_1;
    QLabel *label_10;
    QSpinBox *spinBox_width;
    QSpinBox *spinBox_depth_2;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label_8;
    QSpinBox *spinBox_depth;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *WellDiagramWidget)
    {
        if (WellDiagramWidget->objectName().isEmpty())
            WellDiagramWidget->setObjectName(QString::fromUtf8("WellDiagramWidget"));
        WellDiagramWidget->resize(928, 627);
        WellDiagramWidget->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));
        horizontalLayout_5 = new QHBoxLayout(WellDiagramWidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        groupBox = new QGroupBox(WellDiagramWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_mainpaint = new PaintArea(groupBox);
        widget_mainpaint->setObjectName(QString::fromUtf8("widget_mainpaint"));
        widget_mainpaint->setMinimumSize(QSize(400, 0));
        widget_mainpaint->setMaximumSize(QSize(400, 16777215));
        widget_mainpaint->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(widget_mainpaint);


        horizontalLayout_5->addWidget(groupBox);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_2 = new QGroupBox(WellDiagramWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 25));
        lineEdit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(80, 30));
        pushButton->setMaximumSize(QSize(120, 30));

        horizontalLayout->addWidget(pushButton);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        comboBox_type = new QComboBox(groupBox_2);
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->setObjectName(QString::fromUtf8("comboBox_type"));

        horizontalLayout_3->addWidget(comboBox_type);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(80, 30));
        pushButton_2->setMaximumSize(QSize(120, 30));

        horizontalLayout_3->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_3);

        widget = new QWidget(groupBox_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(20);
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        spinBox_depth_3 = new QSpinBox(widget);
        spinBox_depth_3->setObjectName(QString::fromUtf8("spinBox_depth_3"));
        spinBox_depth_3->setMinimumSize(QSize(0, 25));
        spinBox_depth_3->setMaximumSize(QSize(16777215, 25));
        spinBox_depth_3->setMinimum(1);
        spinBox_depth_3->setMaximum(999999999);

        gridLayout->addWidget(spinBox_depth_3, 5, 1, 1, 1);

        spinBox_depth_1 = new QSpinBox(widget);
        spinBox_depth_1->setObjectName(QString::fromUtf8("spinBox_depth_1"));
        spinBox_depth_1->setMinimumSize(QSize(0, 25));
        spinBox_depth_1->setMaximumSize(QSize(16777215, 25));
        spinBox_depth_1->setMinimum(1);
        spinBox_depth_1->setMaximum(999999999);

        gridLayout->addWidget(spinBox_depth_1, 2, 1, 1, 1);

        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 2, 0, 1, 1);

        spinBox_width = new QSpinBox(widget);
        spinBox_width->setObjectName(QString::fromUtf8("spinBox_width"));
        spinBox_width->setMinimumSize(QSize(0, 25));
        spinBox_width->setMaximumSize(QSize(16777215, 25));
        spinBox_width->setMinimum(1);
        spinBox_width->setMaximum(999999999);

        gridLayout->addWidget(spinBox_width, 0, 1, 1, 1);

        spinBox_depth_2 = new QSpinBox(widget);
        spinBox_depth_2->setObjectName(QString::fromUtf8("spinBox_depth_2"));
        spinBox_depth_2->setMinimumSize(QSize(0, 25));
        spinBox_depth_2->setMaximumSize(QSize(16777215, 25));
        spinBox_depth_2->setMinimum(1);
        spinBox_depth_2->setMaximum(999999999);

        gridLayout->addWidget(spinBox_depth_2, 3, 1, 1, 1);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 5, 0, 1, 1);

        label_8 = new QLabel(widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 0, 1, 1);

        spinBox_depth = new QSpinBox(widget);
        spinBox_depth->setObjectName(QString::fromUtf8("spinBox_depth"));
        spinBox_depth->setMinimumSize(QSize(0, 25));
        spinBox_depth->setMaximumSize(QSize(16777215, 25));
        spinBox_depth->setMinimum(1);
        spinBox_depth->setMaximum(999999999);

        gridLayout->addWidget(spinBox_depth, 1, 1, 1, 1);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(120, 30));
        pushButton_4->setMaximumSize(QSize(120, 30));

        horizontalLayout_2->addWidget(pushButton_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(120, 0));
        pushButton_3->setMaximumSize(QSize(120, 30));

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox_2);


        horizontalLayout_5->addLayout(verticalLayout_3);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 1);

        retranslateUi(WellDiagramWidget);

        QMetaObject::connectSlotsByName(WellDiagramWidget);
    } // setupUi

    void retranslateUi(QWidget *WellDiagramWidget)
    {
        WellDiagramWidget->setWindowTitle(QCoreApplication::translate("WellDiagramWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("WellDiagramWidget", "\344\272\225\350\272\253\347\273\223\346\236\204\351\242\204\350\247\210\345\233\276", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("WellDiagramWidget", "\344\272\225\350\272\253\347\273\223\346\236\204\345\217\202\346\225\260\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("WellDiagramWidget", "\344\272\225\346\234\200\345\244\247\346\267\261\345\272\246(m)", nullptr));
        pushButton->setText(QCoreApplication::translate("WellDiagramWidget", "\347\241\256\345\256\232", nullptr));
        comboBox_type->setItemText(0, QCoreApplication::translate("WellDiagramWidget", "\345\245\227\347\256\241", nullptr));
        comboBox_type->setItemText(1, QCoreApplication::translate("WellDiagramWidget", "\346\262\271\347\256\241", nullptr));
        comboBox_type->setItemText(2, QCoreApplication::translate("WellDiagramWidget", "\345\260\204\345\255\224", nullptr));
        comboBox_type->setItemText(3, QCoreApplication::translate("WellDiagramWidget", "\346\260\264\346\263\245\351\227\264\351\232\231", nullptr));

        pushButton_2->setText(QCoreApplication::translate("WellDiagramWidget", "\345\272\224\347\224\250", nullptr));
        label_7->setText(QCoreApplication::translate("WellDiagramWidget", "\346\267\261\345\272\246(m)", nullptr));
        label_10->setText(QCoreApplication::translate("WellDiagramWidget", "\345\245\227\347\256\2411\346\267\261\345\272\246(m)", nullptr));
        label_6->setText(QCoreApplication::translate("WellDiagramWidget", "\347\233\264\345\276\204", nullptr));
        label_9->setText(QCoreApplication::translate("WellDiagramWidget", "\345\245\227\347\256\2413\346\267\261\345\272\246(m)", nullptr));
        label_8->setText(QCoreApplication::translate("WellDiagramWidget", "\345\245\227\347\256\2412\346\267\261\345\272\246(m)", nullptr));
        pushButton_4->setText(QCoreApplication::translate("WellDiagramWidget", "\345\272\224\347\224\250\345\210\260\344\270\273\347\225\214\351\235\242", nullptr));
        pushButton_3->setText(QCoreApplication::translate("WellDiagramWidget", "\345\217\246\345\255\230\344\270\272\345\233\276\347\211\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WellDiagramWidget: public Ui_WellDiagramWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELLDIAGRAMWIDGET_H
