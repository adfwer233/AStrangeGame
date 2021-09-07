#ifndef BATTLEFIELDWIDGET_H
#define BATTLEFIELDWIDGET_H

#include "battlefieldview.h"
#include "musicpanel.h"
#include "rolestatuspanel.h"
#include "roundstatuspanel.h"
#include "statusConstant.h"
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QWidget>

class BattlefieldWidget : public QWidget {
    Q_OBJECT;

public:
    BattlefieldWidget(QWidget* parent = 0, int levelNumber = 1, levelOfAI AIlevel = basic);

public slots:
    void updateMessage(GraphicUnitInfo);

signals:
    void gameover(coordinateStatus);
    void exitgame();

private:
    RoleStatusPanel*  m_roleStatusPanel;
    roundStatusPanel* m_roundStatusPanel;
    MusicPanel*       m_musicPanel;
    BattlefieldView*  m_battlefieldView;
    QLabel*           m_recommendLabel;
    QLabel*           m_coordinateLabel;
    QLabel*           m_descriptionLabel;
    QPushButton*      m_beginRoundButton;
    QPushButton*      m_cancelSelection;
    QMediaPlayer*     musicPlayer;
};

#endif

// roundStage : 0
