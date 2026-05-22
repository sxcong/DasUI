/********************************************************************************
** Form generated from reading UI file 'chartselectwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTSELECTWIDGET_H
#define UI_CHARTSELECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartSelectWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox_struct;
    QSpacerItem *horizontalSpacer_8;
    QCheckBox *checkBox_TemperatureCurve;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *checkBox_TemperatureWaterfallPlot;
    QSpacerItem *horizontalSpacer_9;
    QCheckBox *checkBox_AmplitudeCurveRaw;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *checkBox_AmplitudeCurve;
    QSpacerItem *horizontalSpacer_6;
    QCheckBox *checkBox_AmplitudeWaterfallPlot;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkBox_PhaseWaterfallPlot;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *checkBox_IntakeProfileFBE;
    QSpacerItem *horizontalSpacer_10;
    QCheckBox *checkBox_InjectionProfile;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkBox_PhaseEnergy;
    QSpacerItem *horizontalSpacer_11;

    void setupUi(QWidget *ChartSelectWidget)
    {
        if (ChartSelectWidget->objectName().isEmpty())
            ChartSelectWidget->setObjectName(QString::fromUtf8("ChartSelectWidget"));
        ChartSelectWidget->resize(1338, 30);
        ChartSelectWidget->setMinimumSize(QSize(1280, 30));
        ChartSelectWidget->setMaximumSize(QSize(4000, 30));
        ChartSelectWidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(ChartSelectWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalSpacer = new QSpacerItem(462, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        checkBox_struct = new QCheckBox(ChartSelectWidget);
        checkBox_struct->setObjectName(QString::fromUtf8("checkBox_struct"));

        horizontalLayout->addWidget(checkBox_struct);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        checkBox_TemperatureCurve = new QCheckBox(ChartSelectWidget);
        checkBox_TemperatureCurve->setObjectName(QString::fromUtf8("checkBox_TemperatureCurve"));

        horizontalLayout->addWidget(checkBox_TemperatureCurve);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        checkBox_TemperatureWaterfallPlot = new QCheckBox(ChartSelectWidget);
        checkBox_TemperatureWaterfallPlot->setObjectName(QString::fromUtf8("checkBox_TemperatureWaterfallPlot"));

        horizontalLayout->addWidget(checkBox_TemperatureWaterfallPlot);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        checkBox_AmplitudeCurveRaw = new QCheckBox(ChartSelectWidget);
        checkBox_AmplitudeCurveRaw->setObjectName(QString::fromUtf8("checkBox_AmplitudeCurveRaw"));

        horizontalLayout->addWidget(checkBox_AmplitudeCurveRaw);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        checkBox_AmplitudeCurve = new QCheckBox(ChartSelectWidget);
        checkBox_AmplitudeCurve->setObjectName(QString::fromUtf8("checkBox_AmplitudeCurve"));

        horizontalLayout->addWidget(checkBox_AmplitudeCurve);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        checkBox_AmplitudeWaterfallPlot = new QCheckBox(ChartSelectWidget);
        checkBox_AmplitudeWaterfallPlot->setObjectName(QString::fromUtf8("checkBox_AmplitudeWaterfallPlot"));

        horizontalLayout->addWidget(checkBox_AmplitudeWaterfallPlot);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        checkBox_PhaseWaterfallPlot = new QCheckBox(ChartSelectWidget);
        checkBox_PhaseWaterfallPlot->setObjectName(QString::fromUtf8("checkBox_PhaseWaterfallPlot"));

        horizontalLayout->addWidget(checkBox_PhaseWaterfallPlot);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        checkBox_IntakeProfileFBE = new QCheckBox(ChartSelectWidget);
        checkBox_IntakeProfileFBE->setObjectName(QString::fromUtf8("checkBox_IntakeProfileFBE"));

        horizontalLayout->addWidget(checkBox_IntakeProfileFBE);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_10);

        checkBox_InjectionProfile = new QCheckBox(ChartSelectWidget);
        checkBox_InjectionProfile->setObjectName(QString::fromUtf8("checkBox_InjectionProfile"));

        horizontalLayout->addWidget(checkBox_InjectionProfile);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        checkBox_PhaseEnergy = new QCheckBox(ChartSelectWidget);
        checkBox_PhaseEnergy->setObjectName(QString::fromUtf8("checkBox_PhaseEnergy"));

        horizontalLayout->addWidget(checkBox_PhaseEnergy);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);


        retranslateUi(ChartSelectWidget);

        QMetaObject::connectSlotsByName(ChartSelectWidget);
    } // setupUi

    void retranslateUi(QWidget *ChartSelectWidget)
    {
        ChartSelectWidget->setWindowTitle(QCoreApplication::translate("ChartSelectWidget", "Form", nullptr));
        checkBox_struct->setText(QCoreApplication::translate("ChartSelectWidget", "\344\272\225\350\272\253\347\273\223\346\236\204", nullptr));
        checkBox_TemperatureCurve->setText(QCoreApplication::translate("ChartSelectWidget", "\346\270\251\345\272\246\346\233\262\347\272\277", nullptr));
        checkBox_TemperatureWaterfallPlot->setText(QCoreApplication::translate("ChartSelectWidget", "\346\270\251\345\272\246\347\200\221\345\270\203\345\233\276", nullptr));
        checkBox_AmplitudeCurveRaw->setText(QCoreApplication::translate("ChartSelectWidget", "\345\271\205\345\200\274\346\233\262\347\272\277\357\274\210\345\216\237\345\247\213\357\274\211", nullptr));
        checkBox_AmplitudeCurve->setText(QCoreApplication::translate("ChartSelectWidget", "\345\271\205\345\200\274\346\233\262\347\272\277", nullptr));
        checkBox_AmplitudeWaterfallPlot->setText(QCoreApplication::translate("ChartSelectWidget", "\345\271\205\345\200\274\347\200\221\345\270\203\345\233\276", nullptr));
        checkBox_PhaseWaterfallPlot->setText(QCoreApplication::translate("ChartSelectWidget", "\347\233\270\344\275\215\347\200\221\345\270\203\345\233\276", nullptr));
        checkBox_IntakeProfileFBE->setText(QCoreApplication::translate("ChartSelectWidget", "\345\220\270\346\260\264\345\211\226\351\235\242FBE", nullptr));
        checkBox_InjectionProfile->setText(QCoreApplication::translate("ChartSelectWidget", "\346\263\250\346\260\264\345\211\226\351\235\242", nullptr));
        checkBox_PhaseEnergy->setText(QCoreApplication::translate("ChartSelectWidget", "\347\233\270\344\275\215\350\203\275\351\207\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartSelectWidget: public Ui_ChartSelectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTSELECTWIDGET_H
