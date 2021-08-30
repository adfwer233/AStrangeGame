#ifndef BATTLEFIELDWIDGET_H
#define BATTLEFIELDWIDGET_H

#include "battlefieldview.h"
#include "rolestatuspanel.h"
#include "roundstatuspanel.h"
#include "statusConstant.h"
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class BattlefieldWidget : public QWidget {
    Q_OBJECT;

public:
    BattlefieldWidget(QWidget* parent = 0);

public slots:
    void updateMessage(GraphicUnitInfo);
    void gameover(coordinateStatus);

private:
    RoleStatusPanel*  m_roleStatusPanel;
    roundStatusPanel* m_roundStatusPanel;
    BattlefieldView*  m_battlefieldView;
    QLabel*           m_recommendLabel;
    QLabel*           m_coordinateLabel;
    QLabel*           m_descriptionLabel;
    QPushButton*      m_beginRoundButton;
    QPushButton*      m_cancelSelection;
};

#endif

// roundStage : 0
