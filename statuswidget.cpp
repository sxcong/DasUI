#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    ui->label_RunStatus->setStyleSheet("color:#12C17E;border-color:#36EB79;");
    ui->label_DAS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_DTS->setStyleSheet("color:#FF0000;border-color:#EE3D1A;");
    ui->label_fileSave->setStyleSheet("color:#FFFF00;border-color:#FFFF00;");


    QString qss = R"(StatusWidget QWidget {
            background-color: #202228;
            border-top: 1px solid #333333;
            color: #999999;
            font-size: 12px;
            padding: 4px;
        }

        /* --- 进度条样式 (CPU/内存占用) --- */
        QProgressBar {
            background-color: #333333;
            border: 1px solid #444444;
            border-radius: 3px;
            text-align: center;
            height: 12px; /* 细进度条 */
            width: 100px;
            margin: 2px;
        }

        /* 进度条滑块 - 统一使用蓝色系，通过代码动态改变颜色更佳，这里给个默认色 */
        QProgressBar::chunk {
            background-color: #4A90E2; /* 统一品牌蓝 */
            border-radius: 2px;
        }

        /* 如果想保留多色（CPU绿、内存紫），可以定义不同的 class 或 objectName */
        QProgressBar#cpuBar::chunk { background-color: #2ECC71; }
        QProgressBar#memBar::chunk { background-color: #9B59B6; }
        QProgressBar#diskBar::chunk { background-color: #E67E22; })";

  //  this->setStyleSheet(qss);


    QString qssMemery = R"(QProgressBar {
        border: 2px solid rgba(255, 255, 255, 0.2);
        border-radius: 8px;
        background-color:rgba(255,255,255,0.4);
        text-align: center;
        color: #000000;
        }

        QProgressBar::chunk {
        background-color:rgba(139,18,174,0.8);
        background-color: #4A90E2; /* 统一品牌蓝 */
        border-bottom-right-radius:8px;
        border-bottom-left-radius:8px;
        border-top-right-radius:8px;
        border-top-left-radius:8px;
        }

        )";
    ui->progressBar_mem->setStyleSheet(qssMemery);


    QString qssDisk = R"(
                QProgressBar {
                border: 2px solid rgba(255, 255, 255, 0.2);
                    border-radius: 8px;
                    background-color: rgba(255,255,255,0.4);
                    text-align: center;
                color: #000000;
                }

                QProgressBar::chunk {
                    /*background-color: rgba(77,166,12, 0.4);*/
                    background-color: #4A90E2; /* 统一品牌蓝 */
                    border-bottom-right-radius:8px;
                    border-bottom-left-radius:8px;
                    border-top-right-radius:8px;
                    border-top-left-radius:8px;
                }

        )";
    ui->progressBar_disk->setStyleSheet(qssDisk);

    QString qssCpu = R"(
            QProgressBar {
            border: 2px solid rgba(255, 255, 255, 0.2);
            border-radius: 8px;
            background-color:rgba(255,255,255,0.4);
            text-align: center;
            color: #000000;
            }

            QProgressBar::chunk {
            /*background-color:rgba(31,133,191,0.8);*/
            background-color: #4A90E2; /* 统一品牌蓝 */
            border-bottom-right-radius:8px;
            border-bottom-left-radius:8px;
            border-top-right-radius:8px;
            border-top-left-radius:8px;
            }
        )";
    ui->progressBar_cpu->setStyleSheet(qssCpu);
}




StatusWidget::~StatusWidget()
{
    delete ui;
}
