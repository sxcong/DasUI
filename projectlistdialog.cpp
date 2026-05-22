#include "projectlistdialog.h"
#include "ui_projectlistdialog.h"
#include "projectdb.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include "projectdb.h"
#include "projectmgr.h"

ProjectListDialog::ProjectListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProjectListDialog)
{
    ui->setupUi(this);

    // 设置对话框自身的样式，确保背景为深色，文字为浅色
    this->setStyleSheet("QDialog { background-color: #141519; color: #FFFFFF; }"
                        "QLabel { color: #FFFFFF; }" // 确保对话框内的 QLabel 文本为白色
                        "QLineEdit { background-color: #2A2A2A; color: #FFFFFF; border: 1px solid #444; padding: 3px; border-radius: 3px; }" // 确保输入框可读
                        "QPushButton { background-color: #3A3A3A; color: #FFFFFF; border: 1px solid #555; padding: 5px 10px; border-radius: 3px; }"
                        "QPushButton:hover { background-color: #4A4A4A; }"
                        "QPushButton:pressed { background-color: #2A2A2A; }"
                       );

    // 创建表格
    m_pTableWidget = new QTableWidget(this);
    m_pTableWidget->setColumnCount(4);
    m_pTableWidget->setHorizontalHeaderLabels({"工区名", "大小","文件目录", "修改时间"});
   // m_pTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    // 美化表格样式 (符合整体深色/简洁风格)
    m_pTableWidget->setStyleSheet(
        "QTableWidget {"
        "   background-color: #1A1C23;" // 表格主体背景色
        "   color: #D1D1D1;"            // 表格文字颜色
        "   gridline-color: #333333;"   // 网格线颜色
        "   border: none;"              // 无边框
        "   outline: none;"             // 移除焦点虚线框
        "}"
        "QTableWidget::item {"
        "   background-color: #1A1C23;" // 单元格背景色
        "   color: #D1D1D1;"            // 单元格文字颜色
        "   padding: 5px;"              // 单元格内边距
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #2E436E;" // 选中单元格背景色
        "   color: #FFFFFF;"            // 选中单元格文字颜色
        "}"
        "QHeaderView::section {"
        "   background-color: #222222;" // 表头背景色
        "   color: #FFFFFF;"            // 表头文字颜色
        "   padding: 4px;"
        "   border: 1px solid #333333;" // 表头边框
        "   font-weight: bold;"
        "}"
        "QTableCornerButton::section {" // 表格左上角空白区域的样式
        "   background-color: #222222;"
        "   border: 1px solid #333333;"
        "}"
        );
    QHeaderView *header = m_pTableWidget->horizontalHeader();
    // 1. 允许用户手动拖动改变列宽
    header->setSectionsMovable(false); // 是否允许交换列顺序（可选）
    header->setSectionResizeMode(QHeaderView::Interactive); // 关键：设置为交互模式


    m_pTableWidget->setColumnWidth(0, 150); // 第一列初始宽度
    m_pTableWidget->setColumnWidth(1, 100); // 第二列初始宽度

    // 最后一列自动填满剩余空间，同时前面的列可以手动拖动：
    header->setStretchLastSection(true);

    ui->verticalLayout_3->addWidget(m_pTableWidget);

    resize(640, 480);
}

ProjectListDialog::~ProjectListDialog()
{
    delete ui;
}

void ProjectListDialog::refreshList()
{

    QVector<ProjectData> projects = ProjectDB::Instance().getAllProjects();

    // 2. 初始化 TableWidget 结构
    m_pTableWidget->clearContents(); // 只清除内容，保留表头
    m_pTableWidget->setColumnCount(3);
    m_pTableWidget->setRowCount(projects.size()); // 根据数据量动态设置行数
   // m_pTableWidget->setHorizontalHeaderLabels({"项目名称", "备注", "导入时间"});

    // 3. 循环填充
    for (int i = 0; i < projects.size(); ++i) {
        // 创建单元格项
        QTableWidgetItem *nameItem = new QTableWidgetItem(projects[i].name);
        QTableWidgetItem *memoItem = new QTableWidgetItem(projects[i].memo);
        QTableWidgetItem *timeItem = new QTableWidgetItem(projects[i].datetime);

        // 设置内容不可编辑（可选）
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
        memoItem->setFlags(memoItem->flags() ^ Qt::ItemIsEditable);
        timeItem->setFlags(timeItem->flags() ^ Qt::ItemIsEditable);

        // 填入表格
        m_pTableWidget->setItem(i, 0, nameItem);
        m_pTableWidget->setItem(i, 1, memoItem);
        m_pTableWidget->setItem(i, 2, timeItem);
    }
}

void ProjectListDialog::setRole(int role)
{
    m_nRole = role;
    switch (role) {
    case 0:
        ui->btnOk->hide();
        break;
    case 1:
        ui->label->hide();
        ui->label_2->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();

        ui->btnAdd->hide();
        ui->btnDelete->hide();
        break;
    default:
        break;
    }

    if (role == 1)
    {
        // 关联双击信号
        connect(m_pTableWidget, &QTableWidget::itemDoubleClicked, this, [=](QTableWidgetItem *item) {
            // 获取行号
            int row = item->row();
            // 获取内容
            QString text = item->text();
            qDebug() << QString("你双击了第 %1 行，内容是: %2").arg(row + 1).arg(text);

            QTableWidgetItem *nameItem = m_pTableWidget->item(row, 0);
            ProjectMgr::Instance().m_curProjectName = nameItem->text();
            qDebug() << "该行对应的名字是：" << nameItem->text();

            accept();

        });
    }
}

void ProjectListDialog::on_btnAdd_clicked()
{
    QString name = ui->lineEdit->text();
    QString memo = ui->lineEdit_2->text();
    if (name.isEmpty())
    {
        QMessageBox::information(this, "error", "名称不能为空");
        return;
    }

    QList<QTableWidgetItem *> items = m_pTableWidget->findItems(name, Qt::MatchExactly);

    for (QTableWidgetItem *item : items) {
        // 关键点：findItems 会搜索整个表格，我们需要过滤出第一列 (Column 0)
        if (item->column() == 0) {
            int row = item->row();
            qDebug() << "在第" << row << "行找到了相同项";

            QMessageBox::information(this, "error", "工区已经存在");
            return;
        }
    }

    bool bRet = ProjectDB::Instance().addProject(name, memo);
    if (bRet)
    {
        QMessageBox::information(this, "success", "新增工区成功");
        refreshList();
    }
    else
    {
        QMessageBox::information(this, "error", "新增工区失败，请检查数据库文件");
    }
}


void ProjectListDialog::on_btnDelete_clicked()
{
    QList<QTableWidgetItem*> selected = m_pTableWidget->selectedItems();
    if (selected.size() < 1)
    {
        QMessageBox::information(this, "error", "请选中要删除的工区");
        return;
    }

    int row = selected.first()->row();

    // 2. 通过行号，强制获取第 0 列（名字列）的 Item
    QTableWidgetItem *nameItem = m_pTableWidget->item(row, 0);
    bool bRet = ProjectDB::Instance().deleteProject(nameItem->text());
    if (!bRet)
    {
        qDebug() << "该行对应的名字是：" << nameItem->text();
        QMessageBox::information(this, "error", "删除工区失败，请检查数据库文件");
    }
    else
    {
        QMessageBox::information(this, "success", "删除工区成功");
        refreshList();
    }
}


void ProjectListDialog::on_btnOk_clicked()
{
    QList<QTableWidgetItem*> selected = m_pTableWidget->selectedItems();
    if (selected.size() < 1)
    {
        QMessageBox::information(this, "error", "请选中要打开的工区");
        return;
    }

    int row = selected.first()->row();

    // 2. 通过行号，强制获取第 0 列（名字列）的 Item
    QTableWidgetItem *nameItem = m_pTableWidget->item(row, 0);
    ProjectData prjData;
    prjData.name = nameItem->text();

    QTableWidgetItem* memoItem = m_pTableWidget->item(row, 1);
    prjData.memo = memoItem->text();
    ProjectMgr::Instance().projectOpen(prjData);
    qDebug() << "该行对应的名字是：" << nameItem->text();

    accept();
}


void ProjectListDialog::on_btnCancel_clicked()
{
    reject();
}
