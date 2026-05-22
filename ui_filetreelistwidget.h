/********************************************************************************
** Form generated from reading UI file 'filetreelistwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETREELISTWIDGET_H
#define UI_FILETREELISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileTreeListWidget
{
public:
    QWidget *widget_left;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QTreeWidget *treeWidget;
    QWidget *widget_right;

    void setupUi(QWidget *FileTreeListWidget)
    {
        if (FileTreeListWidget->objectName().isEmpty())
            FileTreeListWidget->setObjectName(QString::fromUtf8("FileTreeListWidget"));
        FileTreeListWidget->resize(968, 556);
        widget_left = new QWidget(FileTreeListWidget);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        widget_left->setGeometry(QRect(10, 10, 274, 226));
        widget_left->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(widget_left);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(widget_left);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);

        treeWidget = new QTreeWidget(widget_left);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        widget_right = new QWidget(FileTreeListWidget);
        widget_right->setObjectName(QString::fromUtf8("widget_right"));
        widget_right->setGeometry(QRect(490, 40, 211, 311));

        retranslateUi(FileTreeListWidget);

        QMetaObject::connectSlotsByName(FileTreeListWidget);
    } // setupUi

    void retranslateUi(QWidget *FileTreeListWidget)
    {
        FileTreeListWidget->setWindowTitle(QCoreApplication::translate("FileTreeListWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileTreeListWidget: public Ui_FileTreeListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETREELISTWIDGET_H
