#include "startscreen.h"
#include "ui_startscreen.h"

#include <QPainter>
#include <QPalette>
#include <QMessageBox>
StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    auto pale = this->palette();
    pale.setBrush(backgroundRole(), QPixmap(":/new/roles/src/GameBackground.png"));
    this->setPalette(pale);
    this->setAutoFillBackground(true);

    connect(ui->StartButton, &QPushButton::clicked, this, startButtonClicked);
    connect(ui->TextButton, &QPushButton::clicked, this, courseButtonClicked);
}

StartScreen::~StartScreen()
{
    delete ui;
}
