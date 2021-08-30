#ifndef ROUNDSTATUSPANEL_H
#define ROUNDSTATUSPANEL_H

#include "statusConstant.h"
#include <QLabel>
#include <QPushButton>
#include <QWidget>

struct roundStatus {
    int roundNumber;
    coordinateStatus teamInRound;
};

class roundStatusPanel : public QWidget {
    Q_OBJECT
public:
    explicit roundStatusPanel(QWidget* parent = nullptr);
    void updateRoundStatus(roundStatus);

signals:
    void endTheRound();


private:
    int              m_roundNumber;
    QLabel*          m_roundNumberLabel;
    coordinateStatus m_teamInRound;
    QLabel*          m_teamLabel;
    QPushButton*     m_endRoundButton;
};

#endif  // ROUNDSTATUSPANEL_H
