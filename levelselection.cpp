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

    connect(ui->levelOneButton, &QPushButton::clicked, [=]{ emit levelSelected(1); });
    connect(ui->levelTwoButton, &QPushButton::clicked, [=]{ emit levelSelected(2); });
}

LevelSelection::~LevelSelection()
{
    delete ui;
}
