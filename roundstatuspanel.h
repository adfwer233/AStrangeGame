#ifndef ROUNDSTATUSPANEL_H
#define ROUNDSTATUSPANEL_H

#include "statusConstant.h"
#include <QLabel>
#include <QPushButton>
#include <QWidget>

struct roundStatus {
    int              roundNumber;
    coordinateStatus teamInRound;
    int              maxActionPoint;
    int              actionPoint;
};

class roundStatusPanel : public QWidget {
    Q_OBJECT
public:
    explicit roundStatusPanel(QWidget* parent = nullptr);
    void updateRoundStatus(roundStatus);

signals:
    void endTheRound();

private:
    roundStatus      m_status;
    QLabel*          m_roundNumberLabel;
    QLabel*          m_teamLabel;
    QLabel*          m_maxActionPointLabel;
    QLabel*          m_actionPointLabel;
    QPushButton*     m_endRoundButton;
};

#endif  // ROUNDSTATUSPANEL_H
