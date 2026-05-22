/********************************************************************************
** Form generated from reading UI file 'filelistwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILELISTWIDGET_H
#define UI_FILELISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileListWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *FileListWidget)
    {
        if (FileListWidget->objectName().isEmpty())
            FileListWidget->setObjectName(QString::fromUtf8("FileListWidget"));
        FileListWidget->resize(561, 486);
        verticalLayout = new QVBoxLayout(FileListWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(FileListWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        retranslateUi(FileListWidget);

        QMetaObject::connectSlotsByName(FileListWidget);
    } // setupUi

    void retranslateUi(QWidget *FileListWidget)
    {
        FileListWidget->setWindowTitle(QCoreApplication::translate("FileListWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileListWidget: public Ui_FileListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILELISTWIDGET_H
