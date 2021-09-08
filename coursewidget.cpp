#include "coursewidget.h"
#include "ui_coursewidget.h"

CourseWidget::CourseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseWidget)
{
    ui->setupUi(this);
}

CourseWidget::~CourseWidget()
{
    delete ui;
}

void CourseWidget::closeEvent(QCloseEvent *event) {
    emit courseClosed();
    QWidget::closeEvent(event);
}
