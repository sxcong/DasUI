#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class LeftNavBar;
class QStackedWidget;
class WellWidget;
class FileTreeListWidget;
class AdvancedAnalysisWidget;
class UserCenterWidget;
class SystemSettingsWidget;
class ReportWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void setIndex(int index);

public slots:
    void openLfDasFiles();
    void openFbeEnergyFiles();
    void openDownsampleFiles();
    void openSpectrumDbFiles();
private:
    Ui::MainWidget *ui;
    LeftNavBar *leftNavBar;
    QStackedWidget *rightStack;

    WellWidget* m_pWellWidget = nullptr;
    FileTreeListWidget* m_pFileTreeListWidget = nullptr;
    AdvancedAnalysisWidget* m_pAnalysisWidget = nullptr;
    UserCenterWidget* m_pUserCenterWidget = nullptr;
    SystemSettingsWidget* m_pSystemSettingsWidget = nullptr;
    ReportWidget* m_pReportWidget = nullptr;

};

#endif // MAINWIDGET_H
