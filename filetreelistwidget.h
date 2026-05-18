#ifndef FILETREELISTWIDGET_H
#define FILETREELISTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QTableWidget>

namespace Ui {
class FileTreeListWidget;
}

class FileTreeListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTreeListWidget(QWidget *parent = nullptr);
    ~FileTreeListWidget();

private:
    void setupCentralLayout();
    void removeFolder(const QString& szPath);
    void removeTab(const QString &title, const QString &path);
    void createNewTab(const QString &title, const QString &path);
    void setupContextMenu(QTableWidget* table);

    void setupStylesheetText();
    void setupStylesheetText_vscode();
private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);
    void showTreeContextMenu(const QPoint &pos);
    void showGeneralContextMenu(QTableWidget* table, const QPoint &pos);
    void importFolder();
private:
    Ui::FileTreeListWidget *ui;
    QTabWidget* tabWidget;
    QSplitter *mainSplitter;


    //QTreeWidget *leftTree;
    QTreeWidgetItem *wellItem;//井节点
    QTreeWidgetItem *dasItem; //DAS节点
    QTreeWidgetItem *dtsItem; //DTS节点
    QWidget *rightContainer;

    QStringList m_dasFileList;
};

#endif // FILETREELISTWIDGET_H
