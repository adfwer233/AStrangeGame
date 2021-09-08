#include "roundstatuspanel.h"

#include <QLayout>
#include <QLabel>
#include <QDebug>
roundStatusPanel::roundStatusPanel(QWidget *parent) : QWidget(parent) {

    m_roundNumberLabel = new QLabel();
    m_teamLabel = new QLabel();
    m_maxActionPointLabel = new QLabel();
    m_actionPointLabel = new QLabel();

    m_endRoundButton = new QPushButton();
    
    QGridLayout* layout = new QGridLayout();
    
    m_endRoundButton->setText("结束回合");
    layout->addWidget(new QLabel(tr("已经进行的回合数")),0 ,0 ,1, 1);
    layout->addWidget(new QLabel(tr("当前行动队伍")), 1, 0, 1 ,1);
    layout->addWidget(new QLabel(tr("最大行动点数")), 2 ,0 ,1, 1);
    layout->addWidget(new QLabel(tr("已用行动点数")), 3, 0 ,1, 1);
    layout->addWidget(m_roundNumberLabel, 0 , 1, 1 ,2);
    layout->addWidget(m_teamLabel, 1 , 1, 1, 2);
    layout->addWidget(m_maxActionPointLabel, 2, 1, 1, 2);
    layout->addWidget(m_actionPointLabel, 3, 1, 1, 2);
    layout->addWidget(m_endRoundButton, 0 ,3, 2, 4);

    auto sizePolicy = m_endRoundButton->sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    m_endRoundButton->setSizePolicy(sizePolicy);

    this->setLayout(layout);
    connect(m_endRoundButton, &QPushButton::clicked, this, &roundStatusPanel::endTheRound);
}

void roundStatusPanel::updateRoundStatus(roundStatus t_status) {
    m_status = t_status;
    m_roundNumberLabel->setText(QString::number(m_status.roundNumber));
    m_teamLabel->setText(m_status.teamInRound == teamOne ? tr("队伍一") : tr("队伍二"));
    m_maxActionPointLabel->setText(QString::number(m_status.maxActionPoint));
    m_actionPointLabel->setText(QString::number(m_status.actionPoint));
}

void roundStatusPanel::showNextRound() {
    m_endRoundButton->show();
}

void roundStatusPanel::hideNextRound() {
    m_endRoundButton->hide();
}
