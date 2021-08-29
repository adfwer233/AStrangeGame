#ifndef BATTLEFIELDWIDGET_H
#define BATTLEFIELDWIDGET_H

#include "battlefieldview.h"
#include "rolestatuspanel.h"
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class BattlefieldWidget : public QWidget {

public:
    BattlefieldWidget(QWidget* parent = 0);

public slots:
    void updateMessage(GraphicUnitInfo);

private:
    int m_roundStage;

    RoleStatusPanel* m_roleStatusPanel;
    BattlefieldView* m_battlefieldView;
    QLabel*          m_recommendLabel;
    QLabel*          m_coordinateLabel;
    QLabel*          m_descriptionLabel;
    QPushButton*     m_beginRoundButton;
    QPushButton*     m_cancelSelection;
};

#endif

// roundStage : 0
