/********************************************************************************
** Form generated from reading UI file 'titlewdgt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TITLEWDGT_H
#define UI_TITLEWDGT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TitleWdgt
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_top;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_logo;
    QLabel *label;
    QToolButton *tBtn_DataImport;
    QToolButton *tBtn_Process;
    QToolButton *tBtn_Analysis;
    QToolButton *tBtn_Report;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_User;
    QPushButton *btn_Setting;
    QPushButton *btn_Menu;
    QPushButton *btn_Min;
    QPushButton *btn_Middle;
    QPushButton *btn_Max;
    QPushButton *btn_Close;

    void setupUi(QWidget *TitleWdgt)
    {
        if (TitleWdgt->objectName().isEmpty())
            TitleWdgt->setObjectName(QString::fromUtf8("TitleWdgt"));
        TitleWdgt->resize(1920, 68);
        TitleWdgt->setStyleSheet(QString::fromUtf8("QPushButton#widget_top\n"
"{\n"
"    background:none;\n"
"}\n"
"QPushButton#widget_top:hover\n"
"{\n"
"    background:none;\n"
"}\n"
"QPushButton#widget_top:pressed\n"
"{\n"
"    background:none;\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(TitleWdgt);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_top = new QWidget(TitleWdgt);
        widget_top->setObjectName(QString::fromUtf8("widget_top"));
        widget_top->setMinimumSize(QSize(0, 68));
        widget_top->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(widget_top);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(widget_top);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(400, 0));
        widget_2->setStyleSheet(QString::fromUtf8("QToolButton\n"
"{\n"
"	background-color:none;\n"
"	color:white;\n"
"	border:none;\n"
"	border-radius:5px;\n"
"}\n"
"QToolButton:hover\n"
"{\n"
"	background:rgb(50,70,120);\n"
"	color:white;\n"
"	border:none;\n"
"	border-radius:5px;\n"
"	\n"
"}\n"
"QToolButton:pressed\n"
"{\n"
"	background:rgb(46,79,129);\n"
"	color:white;\n"
"	border:none;\n"
"	border-radius:5px;\n"
"}\n"
"QToolButton:checked\n"
"{\n"
"	background:rgb(46,79,129);\n"
"	color:white;\n"
"	border:none;\n"
"	border-radius:5px;\n"
"	border-bottom:2px solid rgb(0,120,215);\n"
"}"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, 0);
        label_logo = new QLabel(widget_2);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_logo->sizePolicy().hasHeightForWidth());
        label_logo->setSizePolicy(sizePolicy);
        label_logo->setMinimumSize(QSize(150, 30));
        label_logo->setMaximumSize(QSize(150, 30));
        label_logo->setPixmap(QPixmap(QString::fromUtf8(":/res/images/logo_big_white.png")));
        label_logo->setScaledContents(true);

        horizontalLayout_3->addWidget(label_logo);

        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(20);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 238);"));

        horizontalLayout_3->addWidget(label);

        tBtn_DataImport = new QToolButton(widget_2);
        tBtn_DataImport->setObjectName(QString::fromUtf8("tBtn_DataImport"));
        tBtn_DataImport->setEnabled(true);
        tBtn_DataImport->setMinimumSize(QSize(130, 50));
        tBtn_DataImport->setMaximumSize(QSize(200, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(12);
        tBtn_DataImport->setFont(font1);
        tBtn_DataImport->setFocusPolicy(Qt::NoFocus);
        tBtn_DataImport->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/title/tBtn_DataAnalysis.png"), QSize(), QIcon::Normal, QIcon::Off);
        tBtn_DataImport->setIcon(icon);
        tBtn_DataImport->setIconSize(QSize(50, 50));
        tBtn_DataImport->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tBtn_DataImport->setAutoRaise(true);

        horizontalLayout_3->addWidget(tBtn_DataImport);

        tBtn_Process = new QToolButton(widget_2);
        tBtn_Process->setObjectName(QString::fromUtf8("tBtn_Process"));
        tBtn_Process->setEnabled(true);
        tBtn_Process->setMinimumSize(QSize(130, 50));
        tBtn_Process->setMaximumSize(QSize(200, 50));
        tBtn_Process->setFont(font1);
        tBtn_Process->setFocusPolicy(Qt::NoFocus);
        tBtn_Process->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/title/tBtn_FFTData.png"), QSize(), QIcon::Normal, QIcon::Off);
        tBtn_Process->setIcon(icon1);
        tBtn_Process->setIconSize(QSize(50, 50));
        tBtn_Process->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tBtn_Process->setAutoRaise(true);

        horizontalLayout_3->addWidget(tBtn_Process);

        tBtn_Analysis = new QToolButton(widget_2);
        tBtn_Analysis->setObjectName(QString::fromUtf8("tBtn_Analysis"));
        tBtn_Analysis->setEnabled(true);
        tBtn_Analysis->setMinimumSize(QSize(130, 50));
        tBtn_Analysis->setMaximumSize(QSize(200, 50));
        tBtn_Analysis->setFont(font1);
        tBtn_Analysis->setFocusPolicy(Qt::NoFocus);
        tBtn_Analysis->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/title/tBtn_Config.png"), QSize(), QIcon::Normal, QIcon::Off);
        tBtn_Analysis->setIcon(icon2);
        tBtn_Analysis->setIconSize(QSize(50, 50));
        tBtn_Analysis->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tBtn_Analysis->setAutoRaise(true);

        horizontalLayout_3->addWidget(tBtn_Analysis);

        tBtn_Report = new QToolButton(widget_2);
        tBtn_Report->setObjectName(QString::fromUtf8("tBtn_Report"));
        tBtn_Report->setEnabled(true);
        tBtn_Report->setMinimumSize(QSize(130, 50));
        tBtn_Report->setMaximumSize(QSize(200, 50));
        tBtn_Report->setFont(font1);
        tBtn_Report->setFocusPolicy(Qt::NoFocus);
        tBtn_Report->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/title/tBtn_History.png"), QSize(), QIcon::Normal, QIcon::Off);
        tBtn_Report->setIcon(icon3);
        tBtn_Report->setIconSize(QSize(50, 50));
        tBtn_Report->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tBtn_Report->setAutoRaise(true);

        horizontalLayout_3->addWidget(tBtn_Report);


        horizontalLayout_2->addWidget(widget_2);

        widget = new QWidget(widget_top);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background: rgba(31, 101, 222, 0.2);\n"
"border: 1px solid #0256FF;\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #FFFFFF;\n"
"\n"
"\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"color:#FFFFFF;\n"
"margin:1px 1px 2px 1px;\n"
"background-color: rgb(2, 86, 200);\n"
"\n"
"}\n"
"\n"
"QPushButton::menu-indicator{image:none}\n"
""));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 20, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_User = new QPushButton(widget);
        btn_User->setObjectName(QString::fromUtf8("btn_User"));
        btn_User->setMinimumSize(QSize(20, 20));
        btn_User->setMaximumSize(QSize(20, 20));
        btn_User->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_user.png);\n"
"border:none;\n"
"}"));
        btn_User->setIconSize(QSize(20, 20));
        btn_User->setFlat(true);

        horizontalLayout->addWidget(btn_User);

        btn_Setting = new QPushButton(widget);
        btn_Setting->setObjectName(QString::fromUtf8("btn_Setting"));
        btn_Setting->setMinimumSize(QSize(20, 20));
        btn_Setting->setMaximumSize(QSize(20, 20));
        btn_Setting->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_setting.png);\n"
"border:none;\n"
"}"));
        btn_Setting->setIconSize(QSize(20, 20));
        btn_Setting->setFlat(true);

        horizontalLayout->addWidget(btn_Setting);

        btn_Menu = new QPushButton(widget);
        btn_Menu->setObjectName(QString::fromUtf8("btn_Menu"));
        btn_Menu->setMinimumSize(QSize(88, 26));
        btn_Menu->setMaximumSize(QSize(88, 26));
        btn_Menu->setCursor(QCursor(Qt::ArrowCursor));
        btn_Menu->setLayoutDirection(Qt::LeftToRight);
        btn_Menu->setStyleSheet(QString::fromUtf8(""));
        btn_Menu->setIconSize(QSize(26, 26));
        btn_Menu->setFlat(true);

        horizontalLayout->addWidget(btn_Menu);

        btn_Min = new QPushButton(widget);
        btn_Min->setObjectName(QString::fromUtf8("btn_Min"));
        btn_Min->setMinimumSize(QSize(26, 26));
        btn_Min->setMaximumSize(QSize(26, 26));
        btn_Min->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_min.png);\n"
"border:none;\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"background-image: url(:/res/title/btn_min_hover.png);\n"
"border:none;\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"background-image: url(:/res/title/btn_min.png);\n"
"border:none;\n"
"}"));
        btn_Min->setIconSize(QSize(26, 26));
        btn_Min->setFlat(true);

        horizontalLayout->addWidget(btn_Min);

        btn_Middle = new QPushButton(widget);
        btn_Middle->setObjectName(QString::fromUtf8("btn_Middle"));
        btn_Middle->setMinimumSize(QSize(26, 26));
        btn_Middle->setMaximumSize(QSize(26, 26));
        btn_Middle->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_middle.png);\n"
"border:none;\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"background-image: url(:/res/title/btn_middle_hover.png);\n"
"border:none;\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"background-image: url(:/res/title/btn_middle.png);\n"
"border:none;\n"
"}"));
        btn_Middle->setIconSize(QSize(26, 26));
        btn_Middle->setFlat(true);

        horizontalLayout->addWidget(btn_Middle);

        btn_Max = new QPushButton(widget);
        btn_Max->setObjectName(QString::fromUtf8("btn_Max"));
        btn_Max->setMinimumSize(QSize(26, 26));
        btn_Max->setMaximumSize(QSize(26, 26));
        btn_Max->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_max.png);\n"
"border:none;\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"background-image: url(:/res/title/btn_max_hover.png);\n"
"border:none;\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"background-image: url(:/res/title/btn_max.png);\n"
"border:none;\n"
"}"));
        btn_Max->setIconSize(QSize(26, 26));
        btn_Max->setFlat(true);

        horizontalLayout->addWidget(btn_Max);

        btn_Close = new QPushButton(widget);
        btn_Close->setObjectName(QString::fromUtf8("btn_Close"));
        btn_Close->setMinimumSize(QSize(26, 26));
        btn_Close->setMaximumSize(QSize(26, 26));
        btn_Close->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"background-image: url(:/res/title/btn_close.png);\n"
"border:none;\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"background-image: url(:/res/title/btn_close_hover.png);\n"
"border:none;\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"background-image: url(:/res/title/btn_close.png);\n"
"border:none;\n"
"}"));
        btn_Close->setIconSize(QSize(26, 26));
        btn_Close->setFlat(true);

        horizontalLayout->addWidget(btn_Close);


        horizontalLayout_2->addWidget(widget);


        verticalLayout->addWidget(widget_top);


        retranslateUi(TitleWdgt);

        QMetaObject::connectSlotsByName(TitleWdgt);
    } // setupUi

    void retranslateUi(QWidget *TitleWdgt)
    {
        TitleWdgt->setWindowTitle(QCoreApplication::translate("TitleWdgt", "Form", nullptr));
        label_logo->setText(QString());
        label->setText(QCoreApplication::translate("TitleWdgt", "\345\205\211\347\272\244\346\265\213\344\272\225\347\263\273\347\273\237", nullptr));
        tBtn_DataImport->setText(QCoreApplication::translate("TitleWdgt", "\346\225\260\346\215\256\345\257\274\345\205\245", nullptr));
        tBtn_Process->setText(QCoreApplication::translate("TitleWdgt", "\344\277\241\345\217\267\351\242\204\345\244\204\347\220\206", nullptr));
        tBtn_Analysis->setText(QCoreApplication::translate("TitleWdgt", "\346\231\272\350\203\275\347\233\221\346\265\213\344\270\216\350\247\243\351\207\212", nullptr));
        tBtn_Report->setText(QCoreApplication::translate("TitleWdgt", "\346\210\220\346\236\234\346\212\245\345\221\212", nullptr));
        btn_User->setText(QString());
        btn_Setting->setText(QString());
        btn_Menu->setText(QCoreApplication::translate("TitleWdgt", " \350\217\234 \345\215\225", nullptr));
        btn_Min->setText(QString());
        btn_Middle->setText(QString());
        btn_Max->setText(QString());
        btn_Close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TitleWdgt: public Ui_TitleWdgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TITLEWDGT_H
