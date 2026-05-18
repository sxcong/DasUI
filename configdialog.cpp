#include "configdialog.h"
#include "ui_configdialog.h"
#include "QDebug"
#include "QMouseEvent"
//#include "globalmodule.h"
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include "defines.h"

ConfigDialog &ConfigDialog::instance()
{
    static ConfigDialog tmp;
    return tmp;
}

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint/* | Qt::WindowStaysOnTopHint*/);
//    this->setProperty("configbackground",true);
    if(nullptr == buttonGroup)
    {
        buttonGroup = new QButtonGroup(this);
        buttonGroup->setExclusive(true);// 如果此属性为真，则在任何给定时间只能选中组中的一个按钮，即设置这个按键组为互斥模式
        buttonGroup->addButton(ui->Btn_AlarmConfig, 0);
        buttonGroup->addButton(ui->Btn_SysConfig, 1);
        buttonGroup->addButton(ui->Btn_FFTconfig, 2);
        buttonGroup->addButton(ui->Btn_StructConfig, 3);
        // 该槽函数做了一件事：修改被选中按键的样式
        QObject::connect(buttonGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled),this, &ConfigDialog::slot_buttonToggled);
        buttonGroup->button(0)->setChecked(true);  // 为customGroup组设置初选项
    }
    loadConfig();
    ui->label_15->hide();
    ui->doubleSpinBox->hide();
    ui->stackedWidget->setCurrentIndex(0);


    ui->comboBox->setStyleSheet(
                "QComboBox{border:1px red solid;}QComboBox::drop-down "
                "{width:0px;border:1px red solid;}"//设置下拉箭头所在的框不显示，即：即使不显示下拉箭头，也不占用右侧空间
                "QComboBox::down-arrow{image:none;}");//设置不显示下拉箭头
//    ui->comboBox->setLineEdit(new QLineEdit);//需要添加行编辑器，否则无法设置对齐方式
//    ui->comboBox->lineEdit()->setAlignment(Qt::AlignRight);
//    ui->comboBox->lineEdit()->setStyleSheet("background:transparent;border:0px;");
    ui->comboBox_saveTime->setCurrentText(QString::number(300));

    ui->spinBox_19->hide();
    ui->label_22->hide();
    initTableWidget(ui->tableWidget);
    ui->Btn_FFTconfig->hide();
}

ConfigDialog::~ConfigDialog()
{
    delete buttonGroup;
    delete ui;
}

void ConfigDialog::slot_buttonToggled(int id, bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(id);
    }
}

void ConfigDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_currentWidgetPos = event->pos();
        m_currentPos = event->pos();

        if(event->pos().y() < 60)
        {
            m_bLeftButtonPressed = true;
        }
    }

    QDialog::mousePressEvent(event);
}

void ConfigDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftButtonPressed = false;
    }

    QDialog::mouseReleaseEvent(event);
}

void ConfigDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if (m_bLeftButtonPressed)
        {
            resize(this->width(),this->height());
            move(event->globalPos() - m_currentWidgetPos);
            return;
        }
    }

    QDialog::mouseMoveEvent(event);
}

void ConfigDialog::on_tBtn_Close_clicked()
{
    this->close();
}

void ConfigDialog::on_btn_Save_2_clicked()
{
    QString filePath = QApplication::applicationDirPath() + "/config/DTSWaterfall.ini";

    QString channel = QString("%1").arg(1);
    QSettings fileConfig(filePath,QSettings::IniFormat);
    fileConfig.setIniCodec("UTF8");
//    fileConfig.setValue("Time_Length_" + channel,ui->spinBox_7->value());
    fileConfig.setValue("AxisY_Min_" + channel,ui->spinBox_9->value());
    fileConfig.setValue("AxisY_Max_" + channel,ui->spinBox_10->value());
    fileConfig.setValue("Blue_Min_" + channel,ui->spinBox_11->value());
    fileConfig.setValue("Red_Max_" + channel,ui->spinBox_12->value());

    fileConfig.setValue("temperature_Min_" + channel,ui->spinBox_25->value());
    fileConfig.setValue("temperature_Max_" + channel,ui->spinBox_9->value());



//    fileConfig.setValue("DTSCalibrationLength",ui->spinBox_14->value()/GlobalParam::Instance().DTSData_Interval);
//    GlobalParam::Instance().DTSCalibrationLength =ui->spinBox_14->value()/GlobalParam::Instance().DTSData_Interval;
    emit signal_ParamsChangedDTS();
    this->close();
}

void ConfigDialog::on_btn_Save_clicked()
{
    QString filePath = QApplication::applicationDirPath() + "/config/DASWaterfall.ini";
    QString channel = QString("%1").arg(1);
    QSettings fileConfig(filePath,QSettings::IniFormat);
    fileConfig.setIniCodec("UTF8");
//    fileConfig.setValue("Time_Length_" + channel,ui->spinBox->value());


    fileConfig.setValue("AxisX_Min_" + channel,ui->spinBox_7->value());
    fileConfig.setValue("AxisX_Max_" + channel,ui->spinBox_26->value());
    fileConfig.setValue("AxisX_Max_2_" + channel,ui->spinBox_27->value());

    fileConfig.setValue("AxisY_Min_" + channel,ui->spinBox_3->value());
    fileConfig.setValue("AxisY_Max_" + channel,ui->spinBox_4->value());
    fileConfig.setValue("Blue_Min_" + channel,ui->spinBox_5->value());
    fileConfig.setValue("Red_Max_" + channel,ui->spinBox_6->value());
    fileConfig.setValue("Blue_Min_Phase_" + channel,ui->spinBox_8->value());
    fileConfig.setValue("Red_Max_Phase_" + channel,ui->spinBox_13->value());
//    fileConfig.setValue("DASCalibrationLength",ui->spinBox_2->value()/GlobalParam::Instance().DASData_Interval);
//    GlobalParam::Instance().DASCalibrationLength =ui->spinBox_2->value()/GlobalParam::Instance().DASData_Interval;
    emit signal_ParamsChanged();
    this->close();
}
void ConfigDialog::loadConfig()
{
    QString filePath = QApplication::applicationDirPath() + "/config/DASWaterfall.ini";

    QString channel = QString("%1").arg(1);
    QString key = "";
    QSettings fileConfig(filePath,QSettings::IniFormat);
    fileConfig.setIniCodec("UTF8");


    if(fileConfig.contains("Time_Length_" + channel))
    {
        ui->spinBox->setValue(fileConfig.value("Time_Length_" + channel).toInt());
    }

    if(fileConfig.contains("AxisX_Min_" + channel))
    {
        ui->spinBox_7->setValue(fileConfig.value("AxisX_Min_" + channel).toInt());
    }

    if(fileConfig.contains("AxisX_Max_2_" + channel))
    {
        ui->spinBox_27->setValue(fileConfig.value("AxisX_Max_2_" + channel).toInt());
    }

    if(fileConfig.contains("AxisX_Max_" + channel))
    {
        ui->spinBox_26->setValue(fileConfig.value("AxisX_Max_" + channel).toInt());
    }

    if(fileConfig.contains("AxisY_Min_" + channel))
    {
        ui->spinBox_3->setValue(fileConfig.value("AxisY_Min_" + channel).toInt());
    }

    if(fileConfig.contains("AxisY_Max_" + channel))
    {
        ui->spinBox_4->setValue(fileConfig.value("AxisY_Max_" + channel).toInt());
    }

    if(fileConfig.contains("Blue_Min_" + channel))
    {
        ui->spinBox_5->setValue(fileConfig.value("Blue_Min_" + channel).toInt());
    }

    if(fileConfig.contains("Red_Max_" + channel))
    {
        ui->spinBox_6->setValue(fileConfig.value("Red_Max_" + channel).toInt());
    }
    if(fileConfig.contains("Blue_Min_Phase_" + channel))
    {
        ui->spinBox_8->setValue(fileConfig.value("Blue_Min_Phase_" + channel).toInt());
    }

    if(fileConfig.contains("Red_Max_Phase_" + channel))
    {
        ui->spinBox_13->setValue(fileConfig.value("Red_Max_Phase_" + channel).toInt());
    }

    if(fileConfig.contains("DASCalibrationLength"))
    {
        //GlobalParam::Instance().DASCalibrationLength = fileConfig.value("DASCalibrationLength").toInt();
        //ui->spinBox_2->setValue(GlobalParam::Instance().DASCalibrationLength);
    }
    if(fileConfig.contains("FFT_Time_Length"))
    {
        ui->spinBox_19->setValue(fileConfig.value("FFT_Time_Length").toInt());
    }

    filePath = QApplication::applicationDirPath() + "/config/DTSWaterfall.ini";
    QSettings fileConfigDTS(filePath,QSettings::IniFormat);
    fileConfigDTS.setIniCodec("UTF8");

    if(fileConfigDTS.contains("DTSCalibrationLength"))
    {
        //GlobalParam::Instance().DTSCalibrationLength = fileConfigDTS.value("DTSCalibrationLength").toInt();
        //ui->spinBox_14->setValue(GlobalParam::Instance().DTSCalibrationLength);
    }

    if(fileConfigDTS.contains("Time_Length_" + channel))
    {
        ui->comboBox->setCurrentText(QString::number(fileConfigDTS.value("Time_Length_" + channel).toInt()));
    }


    if(fileConfigDTS.contains("AxisY_Min_" + channel))
    {
        ui->spinBox_9->setValue(fileConfigDTS.value("AxisY_Min_" + channel).toInt());
    }

    if(fileConfigDTS.contains("AxisY_Max_" + channel))
    {
        ui->spinBox_10->setValue(fileConfigDTS.value("AxisY_Max_" + channel).toInt());
    }

    if(fileConfigDTS.contains("Blue_Min_" + channel))
    {
        ui->spinBox_11->setValue(fileConfigDTS.value("Blue_Min_" + channel).toInt());
    }

    if(fileConfigDTS.contains("Red_Max_" + channel))
    {
        ui->spinBox_12->setValue(fileConfigDTS.value("Red_Max_" + channel).toInt());
    }

    if(fileConfigDTS.contains("temperature_Min_" + channel))
    {
        ui->spinBox_25->setValue(fileConfigDTS.value("temperature_Min_" + channel).toInt());
    }

    if(fileConfigDTS.contains("temperature_Max_" + channel))
    {
        ui->spinBox_9->setValue(fileConfigDTS.value("temperature_Max_" + channel).toInt());
    }


}
void ConfigDialog::getAxisY(int &min, int &max)
{
    min = ui->spinBox_3->value();
    max = ui->spinBox_4->value();
}


void ConfigDialog::getColor(int &minBlue, int &maxRed)
{
    minBlue = ui->spinBox_5->value();
    maxRed = ui->spinBox_6->value();
}
void ConfigDialog::getColorPhase(int &minBlue, int &maxRed)
{
    minBlue = ui->spinBox_8->value();
    maxRed = ui->spinBox_13->value();
}

void ConfigDialog::setColor(int minBlue, int maxRed, int p_minBlue, int p_maxRed)
{
    ui->spinBox_5->setValue(minBlue);
    ui->spinBox_6->setValue(maxRed);
    ui->spinBox_8->setValue(p_minBlue);
    ui->spinBox_13->setValue(p_maxRed);
    on_btn_Save_clicked();
}

int ConfigDialog::getDasDeductLength()
{
    return ui->spinBox_2->value();
}

void ConfigDialog::setDasDeductLength(int length)
{
    ui->spinBox_2->setValue(length);
    on_btn_Save_4_clicked();
}

void ConfigDialog::saveToFile(int type, const WaterFallConfig &config)
{
    QString filePath = QApplication::applicationDirPath() + "/config/FFTWaterfall.ini";

    QString channel = QString("%1").arg(1);
    QSettings fileConfig(filePath,QSettings::IniFormat);

    switch (type)
    {
    case 1:
        qDebug()<< "amplitude config";
        fileConfig.beginGroup("amplitude");
        fileConfig.setValue("x_Min_" + channel,config.x_Min);
        fileConfig.setValue("x_Max_"+ channel,config.x_Max);
        fileConfig.setValue("y_Min_" + channel,config.y_Min);
        fileConfig.setValue("y_Max_"+ channel,config.y_Max);
        fileConfig.setValue("color_Min_" + channel,config.color_Min);
        fileConfig.setValue("color_Max_"+ channel,config.color_Max);
        fileConfig.endGroup();
        break;
    case 2:
        qDebug()<< "phase config";
        fileConfig.beginGroup("phase");
        fileConfig.setValue("x_Min_" + channel,config.x_Min);
        fileConfig.setValue("x_Max_"+ channel,config.x_Max);
        fileConfig.setValue("y_Min_" + channel,config.y_Min);
        fileConfig.setValue("y_Max_"+ channel,config.y_Max);
        fileConfig.setValue("color_Min_" + channel,config.color_Min);
        fileConfig.setValue("color_Max_"+ channel,config.color_Max);
        fileConfig.endGroup();
        break;
    }
}

void ConfigDialog::getAxisXDas(int &min, int &max)
{
    min = ui->spinBox_7->value();
    max = ui->spinBox_26->value();
}

void ConfigDialog::getAxisXPhase(int &min, int &max)
{
    min = ui->spinBox_7->value();
    max = ui->spinBox_27->value();
}

void ConfigDialog::getAxisYDTS(int &min, int &max)
{
    min = ui->spinBox_20->value();
    max = ui->spinBox_10->value();
}

void ConfigDialog::getTempYDTS(int &min, int &max)
{
    min = ui->spinBox_25->value();
    max = ui->spinBox_9->value();
}


void ConfigDialog::getColorDTS(int &minBlue, int &maxRed)
{
    minBlue = ui->spinBox_11->value();
    maxRed = ui->spinBox_12->value();
}

void ConfigDialog::getXtime(int &time)
{
    time = ui->spinBox->value();
}
void ConfigDialog::getXtimeDTS(int &time)
{
    time = ui->comboBox->currentText().toInt();
}


void ConfigDialog::on_btn_Save_3_clicked()
{
    QString filePath = QApplication::applicationDirPath() + "/config/DTSWaterfall.ini";

    QString channel = QString("%1").arg(1);
    QSettings fileConfig(filePath,QSettings::IniFormat);
    fileConfig.setIniCodec("UTF8");
    fileConfig.setValue("Time_Length_" + channel,ui->comboBox->currentText().toInt());
    fileConfig.setValue("DTSCalibrationLength",ui->spinBox_14->value());
    //GlobalParam::Instance().DTSCalibrationLength = ui->spinBox_14->value();
    fileConfig.setValue("DTSData_Interval",ui->doubleSpinBox->value());

    //GlobalParam::Instance().g_DTSSaveTime_Interval = ui->comboBox_saveTime->currentText().toInt();
    emit signal_TimeChanged(false);
    this->close();
}

void ConfigDialog::on_btn_Save_4_clicked()
{
    QString filePath = QApplication::applicationDirPath() + "/config/DASWaterfall.ini";
    QString channel = QString("%1").arg(1);
    QSettings fileConfig(filePath,QSettings::IniFormat);
    fileConfig.setIniCodec("UTF8");
    fileConfig.setValue("Time_Length_" + channel,ui->spinBox->value());
    fileConfig.setValue("DASCalibrationLength",ui->spinBox_2->value());
    fileConfig.setValue("FFT_Time_Length",ui->spinBox_19->value());
    //GlobalParam::Instance().DASCalibrationLength =ui->spinBox_2->value();
    emit signal_TimeChanged(true);
    emit signal_FFTTimeChanged(ui->spinBox_19->value());
    this->close();
}

void ConfigDialog::initTableWidget(QTableWidget* pTable)
{
    if (pTable == nullptr)
        return;

    pTable->setAlternatingRowColors(true);            // 隔行变色
    pTable->setPalette(QPalette(QColor(21, 46, 84))); // 设置隔行变色的颜色  gray灰色

    pTable->setWindowTitle("");
    pTable->setSelectionBehavior(QAbstractItemView::SelectItems); // 设置选择行为，以行为单位
    pTable->setSelectionMode(QAbstractItemView::SingleSelection); // 设置选择模式，选择单行

    pTable->verticalHeader()->setVisible(false); // 隐藏列表头
    // pTable->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    // if(GlobalParam::g_alarmAuto == 0)
    //     pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // else
    // pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // pTable->horizontalHeader()->setFixedHeight(35);//设置表头的高度
    pTable->horizontalHeader()->setHighlightSections(false);
    pTable->setWordWrap(false);
    for (int i = 0; i < pTable->columnCount(); ++i)
    {
        pTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void ConfigDialog::on_btn_Insert_clicked()
{
    int rowCount     = ui->tableWidget->rowCount();
    bool isInsertRow = true; // 默认插入新行
    if (0 != rowCount)
    {
        for (int i = 0; i < rowCount; i++)
        {
            if (!isInsertRow)
            {
                break;
            }

            for (int j = 0; j < ui->tableWidget->columnCount(); ++j)
            {
                {
                    auto rowItem = ui->tableWidget->item(i, j);
                    if (nullptr == rowItem || rowItem->text().isEmpty())
                    {
                        isInsertRow = false;
                        break;
                    }
                }
            }
        }
    }

    if (isInsertRow)
    {
        int rowIndex = ui->tableWidget->currentRow();
        if (0 == rowCount)
        {
            ui->tableWidget->insertRow(0);
        }
        else
        {
            ui->tableWidget->insertRow(rowIndex + 1);
        }

        for (int k = 0; k < ui->tableWidget->columnCount(); ++k)
        {
            auto cell = new QTableWidgetItem("");
            cell->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(rowIndex + 1, k, cell);
        }

        ui->tableWidget->clearSelection();
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(rowIndex + 1, 0), QItemSelectionModel::SelectCurrent);
    }
    else
    {
        QMessageBox::information(this,"提示",QObject::tr("Please complete the data first!"));
    }
}

void ConfigDialog::on_btn_Delete_clicked()
{
    int rowIndex = ui->tableWidget->currentRow();
    if (rowIndex < 0)
        return;

    ui->tableWidget->removeRow(rowIndex);
}



void ConfigDialog::on_btn_save_clicked()
{

}
