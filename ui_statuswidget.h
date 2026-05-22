/********************************************************************************
** Form generated from reading UI file 'statuswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUSWIDGET_H
#define UI_STATUSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label_RunStatus;
    QLabel *label_DTS;
    QLabel *label_DAS;
    QLabel *label_fileSave;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QProgressBar *progressBar_cpu;
    QLabel *label_11;
    QProgressBar *progressBar_disk;
    QLabel *label_12;
    QProgressBar *progressBar_mem;

    void setupUi(QWidget *StatusWidget)
    {
        if (StatusWidget->objectName().isEmpty())
            StatusWidget->setObjectName(QString::fromUtf8("StatusWidget"));
        StatusWidget->resize(1920, 30);
        StatusWidget->setMinimumSize(QSize(0, 30));
        horizontalLayout = new QHBoxLayout(StatusWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_RunStatus = new QLabel(StatusWidget);
        label_RunStatus->setObjectName(QString::fromUtf8("label_RunStatus"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_RunStatus->sizePolicy().hasHeightForWidth());
        label_RunStatus->setSizePolicy(sizePolicy);
        label_RunStatus->setMinimumSize(QSize(0, 26));
        label_RunStatus->setMaximumSize(QSize(16777215, 26));
        label_RunStatus->setStyleSheet(QString::fromUtf8(""));
        label_RunStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_RunStatus->setWordWrap(false);

        horizontalLayout->addWidget(label_RunStatus);

        label_DTS = new QLabel(StatusWidget);
        label_DTS->setObjectName(QString::fromUtf8("label_DTS"));
        label_DTS->setMinimumSize(QSize(0, 26));
        label_DTS->setMaximumSize(QSize(16777215, 26));
        label_DTS->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_DTS);

        label_DAS = new QLabel(StatusWidget);
        label_DAS->setObjectName(QString::fromUtf8("label_DAS"));
        label_DAS->setMinimumSize(QSize(0, 26));
        label_DAS->setMaximumSize(QSize(16777215, 26));
        label_DAS->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_DAS);

        label_fileSave = new QLabel(StatusWidget);
        label_fileSave->setObjectName(QString::fromUtf8("label_fileSave"));
        label_fileSave->setMinimumSize(QSize(0, 26));
        label_fileSave->setMaximumSize(QSize(16777215, 26));
        label_fileSave->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_fileSave);

        horizontalSpacer_2 = new QSpacerItem(1034, 9, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label = new QLabel(StatusWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label);

        progressBar_cpu = new QProgressBar(StatusWidget);
        progressBar_cpu->setObjectName(QString::fromUtf8("progressBar_cpu"));
        progressBar_cpu->setMaximumSize(QSize(130, 20));
        progressBar_cpu->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"border: 2px solid rgba(255, 255, 255, 0.2);\n"
"border-radius: 8px;\n"
"background-color:rgba(255,255,255,0.4);\n"
"text-align: center;\n"
"color: #000000;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"background-color:rgba(31,133,191,0.8);\n"
"border-bottom-right-radius:8px; \n"
"border-bottom-left-radius:8px;\n"
"border-top-right-radius:8px; \n"
"border-top-left-radius:8px;\n"
"}\n"
"\n"
""));
        progressBar_cpu->setValue(20);

        horizontalLayout->addWidget(progressBar_cpu);

        label_11 = new QLabel(StatusWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_11);

        progressBar_disk = new QProgressBar(StatusWidget);
        progressBar_disk->setObjectName(QString::fromUtf8("progressBar_disk"));
        progressBar_disk->setMaximumSize(QSize(130, 20));
        progressBar_disk->setStyleSheet(QString::fromUtf8(""));
        progressBar_disk->setValue(80);

        horizontalLayout->addWidget(progressBar_disk);

        label_12 = new QLabel(StatusWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_12);

        progressBar_mem = new QProgressBar(StatusWidget);
        progressBar_mem->setObjectName(QString::fromUtf8("progressBar_mem"));
        progressBar_mem->setMaximumSize(QSize(130, 20));
        progressBar_mem->setStyleSheet(QString::fromUtf8(""));
        progressBar_mem->setValue(80);

        horizontalLayout->addWidget(progressBar_mem);


        retranslateUi(StatusWidget);

        QMetaObject::connectSlotsByName(StatusWidget);
    } // setupUi

    void retranslateUi(QWidget *StatusWidget)
    {
        StatusWidget->setWindowTitle(QCoreApplication::translate("StatusWidget", "Form", nullptr));
        label_RunStatus->setText(QCoreApplication::translate("StatusWidget", " \350\275\257\344\273\266\350\277\220\350\241\214\347\212\266\346\200\201 ", nullptr));
        label_DTS->setText(QCoreApplication::translate("StatusWidget", " DTS\346\234\215\345\212\241 ", nullptr));
        label_DAS->setText(QCoreApplication::translate("StatusWidget", " DAS\346\234\215\345\212\241 ", nullptr));
        label_fileSave->setText(QCoreApplication::translate("StatusWidget", " \345\216\237\345\247\213\346\225\260\346\215\256\344\277\235\345\255\230 ", nullptr));
        label->setText(QCoreApplication::translate("StatusWidget", "CPU\345\215\240\347\224\250", nullptr));
        label_11->setText(QCoreApplication::translate("StatusWidget", "\347\241\254\347\233\230\345\215\240\347\224\250", nullptr));
        label_12->setText(QCoreApplication::translate("StatusWidget", "\345\206\205\345\255\230\345\215\240\347\224\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatusWidget: public Ui_StatusWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUSWIDGET_H
