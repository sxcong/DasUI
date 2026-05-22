/********************************************************************************
** Form generated from reading UI file 'wellwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELLWIDGET_H
#define UI_WELLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "chartselectwidget.h"
#include "curveview.h"
#include "frameline.h"
#include "ratecurveview.h"
#include "waterfallwdgttransf.h"

QT_BEGIN_NAMESPACE

class Ui_WellWidget
{
public:
    QVBoxLayout *verticalLayout_5;
    ChartSelectWidget *widget_chart_select;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QFrame *line_8;
    QWidget *widget_struct;
    QVBoxLayout *verticalLayout;
    QWidget *widget_title6;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_6;
    FrameLine *label_structure;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_title1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_1;
    CurveView *widget_DTS_curve;
    QFrame *line_2;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_title2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    WaterFallWdgtTransf *widget_DTS;
    QPushButton *btn_window_dts;
    QWidget *widget_rawAmCurve;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget_title7;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_5;
    CurveView *widget_DAS_rawAmCurve;
    QFrame *line_3;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_title3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    CurveView *widget_DAS_curve;
    QFrame *line_4;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget_title4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_4;
    WaterFallWdgtTransf *widget_DAS;
    QPushButton *btn_window_ample;
    QFrame *line_5;
    QVBoxLayout *verticalLayout_7;
    QWidget *widget_title5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    WaterFallWdgtTransf *widget_DAS_Phase;
    QPushButton *btn_window;
    QVBoxLayout *verticalLayout_10;
    QWidget *widget_title9;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_8;
    CurveView *widget_FBE_curve;
    QVBoxLayout *verticalLayout_11;
    QWidget *widget_title8;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_9;
    RateCurveView *widget_waterInject;
    QPushButton *btn_window_3;

    void setupUi(QWidget *WellWidget)
    {
        if (WellWidget->objectName().isEmpty())
            WellWidget->setObjectName(QString::fromUtf8("WellWidget"));
        WellWidget->resize(1756, 764);
        verticalLayout_5 = new QVBoxLayout(WellWidget);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget_chart_select = new ChartSelectWidget(WellWidget);
        widget_chart_select->setObjectName(QString::fromUtf8("widget_chart_select"));
        widget_chart_select->setMinimumSize(QSize(0, 30));
        widget_chart_select->setMaximumSize(QSize(16777215, 30));

        verticalLayout_5->addWidget(widget_chart_select);

        widget = new QWidget(WellWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 30));
        widget->setMaximumSize(QSize(16777215, 16777215));
        widget->setStyleSheet(QString::fromUtf8("QWidget#widget_title1,#widget_title2,#widget_title3,#widget_title4,#widget_title5,#widget_title6,#widget_title7,#widget_title8,#widget_tile9\n"
"{\n"
"	image: url(:/res/title/subtitile.png);\n"
"}\n"
"QLabel\n"
"{\n"
"	background-color: none;\n"
"}\n"
"background-color: #323E4C;"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, -1, -1, -1);
        line_8 = new QFrame(widget);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_8->sizePolicy().hasHeightForWidth());
        line_8->setSizePolicy(sizePolicy);
        line_8->setStyleSheet(QString::fromUtf8(""));
        line_8->setFrameShadow(QFrame::Sunken);
        line_8->setLineWidth(2);
        line_8->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_8);

        widget_struct = new QWidget(widget);
        widget_struct->setObjectName(QString::fromUtf8("widget_struct"));
        widget_struct->setMinimumSize(QSize(350, 0));
        widget_struct->setMaximumSize(QSize(350, 16777215));
        widget_struct->setStyleSheet(QString::fromUtf8("/*background-color: #04152F;*/\n"
""));
        verticalLayout = new QVBoxLayout(widget_struct);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 15, -1, 15);
        widget_title6 = new QWidget(widget_struct);
        widget_title6->setObjectName(QString::fromUtf8("widget_title6"));
        widget_title6->setMinimumSize(QSize(0, 30));
        widget_title6->setMaximumSize(QSize(16777215, 30));
        widget_title6->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_10 = new QHBoxLayout(widget_title6);
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(widget_title6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(0, 20));
        label_6->setMaximumSize(QSize(16777215, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8(""));
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_6);


        verticalLayout->addWidget(widget_title6);

        label_structure = new FrameLine(widget_struct);
        label_structure->setObjectName(QString::fromUtf8("label_structure"));
        label_structure->setMinimumSize(QSize(330, 300));
        label_structure->setMaximumSize(QSize(330, 1200));
        label_structure->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(label_structure);


        horizontalLayout->addWidget(widget_struct);

        line = new QFrame(widget);
        line->setObjectName(QString::fromUtf8("line"));
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setStyleSheet(QString::fromUtf8(""));
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(2);
        line->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 15, -1, -1);
        widget_title1 = new QWidget(widget);
        widget_title1->setObjectName(QString::fromUtf8("widget_title1"));
        widget_title1->setMaximumSize(QSize(16777215, 30));
        widget_title1->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_4 = new QHBoxLayout(widget_title1);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_1 = new QLabel(widget_title1);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setMinimumSize(QSize(0, 20));
        label_1->setMaximumSize(QSize(16777215, 20));
        label_1->setFont(font);
        label_1->setStyleSheet(QString::fromUtf8(""));
        label_1->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_1);


        verticalLayout_2->addWidget(widget_title1);

        widget_DTS_curve = new CurveView(widget);
        widget_DTS_curve->setObjectName(QString::fromUtf8("widget_DTS_curve"));
        widget_DTS_curve->setMinimumSize(QSize(200, 300));
        widget_DTS_curve->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(widget_DTS_curve);


        horizontalLayout->addLayout(verticalLayout_2);

        line_2 = new QFrame(widget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setStyleSheet(QString::fromUtf8(""));
        line_2->setFrameShadow(QFrame::Sunken);
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 15, -1, -1);
        widget_title2 = new QWidget(widget);
        widget_title2->setObjectName(QString::fromUtf8("widget_title2"));
        widget_title2->setMaximumSize(QSize(16777215, 30));
        widget_title2->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_5 = new QHBoxLayout(widget_title2);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget_title2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 20));
        label_2->setMaximumSize(QSize(16777215, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_2);


        verticalLayout_3->addWidget(widget_title2);

        widget_DTS = new WaterFallWdgtTransf(widget);
        widget_DTS->setObjectName(QString::fromUtf8("widget_DTS"));
        widget_DTS->setMinimumSize(QSize(0, 300));
        btn_window_dts = new QPushButton(widget_DTS);
        btn_window_dts->setObjectName(QString::fromUtf8("btn_window_dts"));
        btn_window_dts->setGeometry(QRect(1765, 1, 40, 43));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_window_dts->sizePolicy().hasHeightForWidth());
        btn_window_dts->setSizePolicy(sizePolicy1);
        btn_window_dts->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border:none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/title/maxico.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_window_dts->setIcon(icon);
        btn_window_dts->setIconSize(QSize(40, 40));
        btn_window_dts->setFlat(true);

        verticalLayout_3->addWidget(widget_DTS);


        horizontalLayout->addLayout(verticalLayout_3);

        widget_rawAmCurve = new QWidget(widget);
        widget_rawAmCurve->setObjectName(QString::fromUtf8("widget_rawAmCurve"));
        verticalLayout_9 = new QVBoxLayout(widget_rawAmCurve);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 15, 0, 0);
        widget_title7 = new QWidget(widget_rawAmCurve);
        widget_title7->setObjectName(QString::fromUtf8("widget_title7"));
        widget_title7->setMaximumSize(QSize(16777215, 30));
        widget_title7->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_12 = new QHBoxLayout(widget_title7);
        horizontalLayout_12->setSpacing(0);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget_title7);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(0, 20));
        label_5->setMaximumSize(QSize(16777215, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(label_5);


        verticalLayout_9->addWidget(widget_title7);

        widget_DAS_rawAmCurve = new CurveView(widget_rawAmCurve);
        widget_DAS_rawAmCurve->setObjectName(QString::fromUtf8("widget_DAS_rawAmCurve"));
        widget_DAS_rawAmCurve->setMinimumSize(QSize(0, 300));

        verticalLayout_9->addWidget(widget_DAS_rawAmCurve);


        horizontalLayout->addWidget(widget_rawAmCurve);

        line_3 = new QFrame(widget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setStyleSheet(QString::fromUtf8("QFrame#line\n"
"{\n"
"	color:rgb(26,43,69);\n"
"}\n"
""));
        line_3->setFrameShadow(QFrame::Sunken);
        line_3->setLineWidth(2);
        line_3->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 15, -1, -1);
        widget_title3 = new QWidget(widget);
        widget_title3->setObjectName(QString::fromUtf8("widget_title3"));
        widget_title3->setMaximumSize(QSize(16777215, 30));
        widget_title3->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_6 = new QHBoxLayout(widget_title3);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget_title3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 20));
        label_3->setMaximumSize(QSize(16777215, 20));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_3);


        verticalLayout_4->addWidget(widget_title3);

        widget_DAS_curve = new CurveView(widget);
        widget_DAS_curve->setObjectName(QString::fromUtf8("widget_DAS_curve"));
        widget_DAS_curve->setMinimumSize(QSize(0, 300));

        verticalLayout_4->addWidget(widget_DAS_curve);


        horizontalLayout->addLayout(verticalLayout_4);

        line_4 = new QFrame(widget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShadow(QFrame::Sunken);
        line_4->setLineWidth(2);
        line_4->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_4);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, 15, -1, -1);
        widget_title4 = new QWidget(widget);
        widget_title4->setObjectName(QString::fromUtf8("widget_title4"));
        widget_title4->setMaximumSize(QSize(16777215, 30));
        widget_title4->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_8 = new QHBoxLayout(widget_title4);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget_title4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(0, 20));
        label_4->setMaximumSize(QSize(16777215, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_4);


        verticalLayout_6->addWidget(widget_title4);

        widget_DAS = new WaterFallWdgtTransf(widget);
        widget_DAS->setObjectName(QString::fromUtf8("widget_DAS"));
        widget_DAS->setMinimumSize(QSize(0, 300));
        widget_DAS->setStyleSheet(QString::fromUtf8(""));
        btn_window_ample = new QPushButton(widget_DAS);
        btn_window_ample->setObjectName(QString::fromUtf8("btn_window_ample"));
        btn_window_ample->setGeometry(QRect(1765, 1, 40, 43));
        sizePolicy1.setHeightForWidth(btn_window_ample->sizePolicy().hasHeightForWidth());
        btn_window_ample->setSizePolicy(sizePolicy1);
        btn_window_ample->setLayoutDirection(Qt::LeftToRight);
        btn_window_ample->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border:none;"));
        btn_window_ample->setIcon(icon);
        btn_window_ample->setIconSize(QSize(40, 40));
        btn_window_ample->setFlat(true);

        verticalLayout_6->addWidget(widget_DAS);


        horizontalLayout->addLayout(verticalLayout_6);

        line_5 = new QFrame(widget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShadow(QFrame::Sunken);
        line_5->setLineWidth(2);
        line_5->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_5);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 15, -1, -1);
        widget_title5 = new QWidget(widget);
        widget_title5->setObjectName(QString::fromUtf8("widget_title5"));
        widget_title5->setMaximumSize(QSize(16777215, 30));
        widget_title5->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_9 = new QHBoxLayout(widget_title5);
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(widget_title5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(0, 20));
        label_7->setMaximumSize(QSize(16777215, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_7);


        verticalLayout_7->addWidget(widget_title5);

        widget_DAS_Phase = new WaterFallWdgtTransf(widget);
        widget_DAS_Phase->setObjectName(QString::fromUtf8("widget_DAS_Phase"));
        widget_DAS_Phase->setMinimumSize(QSize(0, 300));
        btn_window = new QPushButton(widget_DAS_Phase);
        btn_window->setObjectName(QString::fromUtf8("btn_window"));
        btn_window->setGeometry(QRect(1765, 1, 40, 43));
        sizePolicy1.setHeightForWidth(btn_window->sizePolicy().hasHeightForWidth());
        btn_window->setSizePolicy(sizePolicy1);
        btn_window->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border:none;"));
        btn_window->setIcon(icon);
        btn_window->setIconSize(QSize(40, 40));
        btn_window->setFlat(true);

        verticalLayout_7->addWidget(widget_DAS_Phase);


        horizontalLayout->addLayout(verticalLayout_7);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(-1, 15, -1, -1);
        widget_title9 = new QWidget(widget);
        widget_title9->setObjectName(QString::fromUtf8("widget_title9"));
        widget_title9->setMaximumSize(QSize(16777215, 30));
        widget_title9->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_13 = new QHBoxLayout(widget_title9);
        horizontalLayout_13->setSpacing(0);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(widget_title9);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(0, 20));
        label_8->setMaximumSize(QSize(16777215, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_13->addWidget(label_8);


        verticalLayout_10->addWidget(widget_title9);

        widget_FBE_curve = new CurveView(widget);
        widget_FBE_curve->setObjectName(QString::fromUtf8("widget_FBE_curve"));
        widget_FBE_curve->setMinimumSize(QSize(0, 300));

        verticalLayout_10->addWidget(widget_FBE_curve);


        horizontalLayout->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(-1, 15, -1, -1);
        widget_title8 = new QWidget(widget);
        widget_title8->setObjectName(QString::fromUtf8("widget_title8"));
        widget_title8->setMaximumSize(QSize(16777215, 30));
        widget_title8->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_14 = new QHBoxLayout(widget_title8);
        horizontalLayout_14->setSpacing(0);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(widget_title8);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(0, 20));
        label_9->setMaximumSize(QSize(16777215, 20));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(label_9);


        verticalLayout_11->addWidget(widget_title8);

        widget_waterInject = new RateCurveView(widget);
        widget_waterInject->setObjectName(QString::fromUtf8("widget_waterInject"));
        widget_waterInject->setMinimumSize(QSize(0, 300));
        btn_window_3 = new QPushButton(widget_waterInject);
        btn_window_3->setObjectName(QString::fromUtf8("btn_window_3"));
        btn_window_3->setGeometry(QRect(1765, 1, 40, 43));
        sizePolicy1.setHeightForWidth(btn_window_3->sizePolicy().hasHeightForWidth());
        btn_window_3->setSizePolicy(sizePolicy1);
        btn_window_3->setStyleSheet(QString::fromUtf8("background:transparent;\n"
"border:none;"));
        btn_window_3->setIcon(icon);
        btn_window_3->setIconSize(QSize(40, 40));
        btn_window_3->setFlat(true);

        verticalLayout_11->addWidget(widget_waterInject);


        horizontalLayout->addLayout(verticalLayout_11);


        verticalLayout_5->addWidget(widget);


        retranslateUi(WellWidget);

        QMetaObject::connectSlotsByName(WellWidget);
    } // setupUi

    void retranslateUi(QWidget *WellWidget)
    {
        WellWidget->setWindowTitle(QCoreApplication::translate("WellWidget", "Form", nullptr));
        label_6->setText(QCoreApplication::translate("WellWidget", "\344\272\225\350\272\253\347\273\223\346\236\204\345\233\276", nullptr));
        label_1->setText(QCoreApplication::translate("WellWidget", "\346\270\251\345\272\246\346\233\262\347\272\277", nullptr));
        label_2->setText(QCoreApplication::translate("WellWidget", "\346\270\251\345\272\246\347\200\221\345\270\203\345\233\276", nullptr));
        btn_window_dts->setText(QString());
        label_5->setText(QCoreApplication::translate("WellWidget", "\345\271\205\345\200\274\346\233\262\347\272\277(\345\216\237\345\247\213)", nullptr));
        label_3->setText(QCoreApplication::translate("WellWidget", "\345\271\205\345\200\274\346\233\262\347\272\277", nullptr));
        label_4->setText(QCoreApplication::translate("WellWidget", "\345\271\205\345\200\274\347\200\221\345\270\203\345\233\276", nullptr));
        btn_window_ample->setText(QString());
        label_7->setText(QCoreApplication::translate("WellWidget", "\347\233\270\344\275\215\347\200\221\345\270\203\345\233\276", nullptr));
        btn_window->setText(QString());
        label_8->setText(QCoreApplication::translate("WellWidget", "\345\220\270\346\260\264\345\211\226\351\235\242FBE", nullptr));
        label_9->setText(QCoreApplication::translate("WellWidget", "\346\263\250\346\260\264\345\211\226\351\235\242", nullptr));
        btn_window_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WellWidget: public Ui_WellWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELLWIDGET_H
