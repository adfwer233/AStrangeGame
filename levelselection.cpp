#include "levelselection.h"
#include "ui_levelselection.h"

LevelSelection::LevelSelection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelSelection)
{
    ui->setupUi(this);

    auto pale = this->palette();
    pale.setBrush(backgroundRole(), QPixmap(":/new/roles/src/GameBackground.png"));
    this->setPalette(pale);
    this->setAutoFillBackground(true);

    connect(ui->levelOneButton, &QPushButton::clicked, [=]{ emit levelSelected(1, basic); });
    connect(ui->levelOneButton_ad, &QPushButton::clicked, [=]{ emit levelSelected(1, advanced); });
    connect(ui->levelTwoButton, &QPushButton::clicked, [=]{ emit levelSelected(2, basic); });
    connect(ui->levelTwoButton_ad, &QPushButton::clicked, [=]{ emit levelSelected(2, advanced); });
}

LevelSelection::~LevelSelection()
{
    delete ui;
}

void LevelSelection::closeEvent(QCloseEvent *event) {
    emit levelClosed();
    QWidget::closeEvent(event);
}