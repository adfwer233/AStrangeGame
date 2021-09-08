#ifndef COURSEWIDGET_H
#define COURSEWIDGET_H

#include <QWidget>

namespace Ui {
class CourseWidget;
}

class CourseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CourseWidget(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~CourseWidget();

signals:
    void courseClosed();

private:
    Ui::CourseWidget *ui;
};

#endif // COURSEWIDGET_H
