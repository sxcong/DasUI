/********************************************************************************
** Form generated from reading UI file 'projectlistdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTLISTDIALOG_H
#define UI_PROJECTLISTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProjectListDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *ProjectListDialog)
    {
        if (ProjectListDialog->objectName().isEmpty())
            ProjectListDialog->setObjectName(QString::fromUtf8("ProjectListDialog"));
        ProjectListDialog->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(ProjectListDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(10);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, -1, -1, 10);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(ProjectListDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(40, 0));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(ProjectListDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(ProjectListDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(40, 0));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_2);

        lineEdit_2 = new QLineEdit(ProjectListDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_4->addWidget(lineEdit_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnAdd = new QPushButton(ProjectListDialog);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

        horizontalLayout_5->addWidget(btnAdd);

        btnDelete = new QPushButton(ProjectListDialog);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));

        horizontalLayout_5->addWidget(btnDelete);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        btnOk = new QPushButton(ProjectListDialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout_5->addWidget(btnOk);

        btnCancel = new QPushButton(ProjectListDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout_5->addWidget(btnCancel);


        verticalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout->addLayout(verticalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ProjectListDialog);

        QMetaObject::connectSlotsByName(ProjectListDialog);
    } // setupUi

    void retranslateUi(QDialog *ProjectListDialog)
    {
        ProjectListDialog->setWindowTitle(QCoreApplication::translate("ProjectListDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ProjectListDialog", "\345\267\245\345\214\272\345\220\215:", nullptr));
        label_2->setText(QCoreApplication::translate("ProjectListDialog", "\345\244\207\346\263\250:", nullptr));
        btnAdd->setText(QCoreApplication::translate("ProjectListDialog", "\346\226\260\345\242\236", nullptr));
        btnDelete->setText(QCoreApplication::translate("ProjectListDialog", "\345\210\240\351\231\244", nullptr));
        btnOk->setText(QCoreApplication::translate("ProjectListDialog", "OK", nullptr));
        btnCancel->setText(QCoreApplication::translate("ProjectListDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectListDialog: public Ui_ProjectListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTLISTDIALOG_H
