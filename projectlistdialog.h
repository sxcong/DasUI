#ifndef PROJECTLISTDIALOG_H
#define PROJECTLISTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>

namespace Ui {
class ProjectListDialog;
}

class ProjectListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectListDialog(QWidget *parent = nullptr);
    ~ProjectListDialog();
    void refreshList();
    void setRole(int role);//role:0新建 1打开

private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();
private:
    Ui::ProjectListDialog *ui;
    QTableWidget* m_pTableWidget;
    int m_nRole = 0;
};

#endif // PROJECTLISTDIALOG_H
