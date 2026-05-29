#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QStyledItemDelegate>

namespace Ui {
class FileListWidget;
}

// 自定义 delegate：只绘制 hover 行背景，不改变 selection
class HoverRowDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    int hoveredRow = -1;

    explicit HoverRowDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        if (index.row() == hoveredRow && !(opt.state & QStyle::State_Selected)) {
            opt.state |= QStyle::State_Selected;
        }
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

class FileListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget *parent = nullptr);
    ~FileListWidget();
    void setPath(const QString &title, const QString &path);
private slots:
    void showGeneralContextMenu(QTableWidget* table, const QPoint &pos);
private:
    void initTable(QTableWidget* table);
    void setupContextMenu(QTableWidget* table);
    QWidget* createCustomRowWidget(const QString &fileName, const QString &fileSize, const QString &fileTime);
    void openLfDasFiles(const QString& initialDir = QString());
    void openFbeEnergyFiles(const QString& initialDir = QString());
    void openDownsampleFiles(const QString& initialDir = QString());
    void openSpectrumDbFiles(const QString& initialDir = QString());
private:
    bool eventFilter(QObject *obj, QEvent *event) override;
    Ui::FileListWidget *ui;
    QString m_title;
    QString m_path;
    QStringList m_dasFileList;
};

#endif // FILELISTWIDGET_H
