#include "roundstatuspanel.h"

#include <QLayout>
#include <QLabel>
roundStatusPanel::roundStatusPanel(QWidget *parent) : QWidget(parent) {

    m_roundNumberLabel = new QLabel();
    m_teamLabel = new QLabel();
    m_endRoundButton = new QPushButton();
    
    QGridLayout* layout = new QGridLayout();
    
    m_endRoundButton->setText("结束回合");
    layout->addWidget(new QLabel(tr("已经进行的回合数")),0 ,0 ,1, 1);
    layout->addWidget(new QLabel(tr("当前行动队伍")), 1, 0, 1 ,1);
    layout->addWidget(m_roundNumberLabel, 0 , 1, 1 ,2);
    layout->addWidget(m_teamLabel, 1 , 1, 1, 2);

    layout->addWidget(m_endRoundButton, 0 ,3, 2, 2);

    this->setLayout(layout);
    connect(m_endRoundButton, &QPushButton::clicked, this, &roundStatusPanel::endTheRound);
}

void roundStatusPanel::updateRoundStatus(roundStatus t_status) {
    m_roundNumber = t_status.roundNumber;
    m_teamInRound = t_status.teamInRound;
    m_roundNumberLabel->setText(QString::number(m_roundNumber));

    if (m_teamInRound == teamOne) {
        m_teamLabel->setText(tr("队伍一"));
    }
    else if (m_teamInRound == teamTwo) {
        m_teamLabel->setText(tr("队伍二"));
    }
}
