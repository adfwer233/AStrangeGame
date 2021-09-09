#include "gameoverwidget.h"
#include "ui_gameoverwidget.h"

GameoverWidget::GameoverWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameoverWidget)
{
    ui->setupUi(this);

    auto pale = this->palette();
    pale.setBrush(backgroundRole(), QPixmap(":/new/roles/src/GameBackground.png"));
    this->setPalette(pale);
    this->setAutoFillBackground(true);

    connect(ui->pushButton, &QPushButton::clicked, this, &GameoverWidget::goBack);
}

GameoverWidget::~GameoverWidget()
{
    delete ui;
}

void GameoverWidget::setWinner(coordinateStatus t_team) {
    if (t_team == teamOne)
        ui->resultLabel->setText(tr("你赢了！！！"));
    else if (t_team == teamTwo)
        ui->resultLabel->setText(tr("失败"));
    else
        ui->resultLabel->setText(tr("作者无法想象的事情发生了"));
}