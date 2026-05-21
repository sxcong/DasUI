#ifndef ADVANCEDANALYSISWIDGET_H
#define ADVANCEDANALYSISWIDGET_H

#include <QWidget>

namespace Ui {
class AdvancedAnalysisWidget;
}

class AdvancedAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedAnalysisWidget(QWidget *parent = nullptr);
    ~AdvancedAnalysisWidget();

private:
    Ui::AdvancedAnalysisWidget *ui;
};

#endif // ADVANCEDANALYSISWIDGET_H