#ifndef QVABOUTDIALOG_H
#define QVABOUTDIALOG_H

#include <QDialog>
//#include <QtNetwork>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

    void updateText();

    double getLatestVersionNum() const;
    void setLatestVersionNum(double value);

private:
    Ui::AboutDialog *ui;

    double latestVersionNum;
};

#endif // QVABOUTDIALOG_H
