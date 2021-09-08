#include "battlefieldWidget.h"
#include "gamelevelone.h"
#include "gameleveltwo.h"
#include <QLayout>
#include <QMessageBox>
#include <QSpacerItem>
#include <QMediaPlaylist>

BattlefieldWidget::BattlefieldWidget(QWidget* parent, int levelNumber, levelOfAI t_AIlevel) : QWidget(parent) {

    // initialize the game level
    if (levelNumber == 1) {
        m_battlefieldView = new GameLevelOne(parent);
        this->setWindowTitle("第一关");
    }
    else if (levelNumber == 2) {
        m_battlefieldView = new GameLevelTwo(parent);
        this->setWindowTitle("第二关");
    }
    else {
        throw "no this level";
    }

    this->setWindowIcon(QIcon(":/new/roles/src/GameBackground.png"));

    m_battlefieldView->setAIlevel(t_AIlevel);

    QGraphicsScene* scene = new QGraphicsScene();

    m_battlefieldView->setScene(scene);

    QVBoxLayout* messageLayout = new QVBoxLayout();
    m_coordinateLabel          = new QLabel(QObject::tr("这里将会显示坐标"));
    m_descriptionLabel         = new QLabel(QObject::tr("Description"));
    m_beginRoundButton         = new QPushButton(tr("退出游戏"));
    m_cancelSelection          = new QPushButton(tr("取消选择"));
    m_roleStatusPanel          = new RoleStatusPanel();
    m_roundStatusPanel         = new roundStatusPanel();
    m_musicPanel               = new MusicPanel();

    messageLayout->addWidget(m_roleStatusPanel);
    // messageLayout->addWidget(m_coordinateLabel);
    // messageLayout->addWidget(m_descriptionLabel);
    messageLayout->addWidget(m_musicPanel);
    messageLayout->addWidget(m_cancelSelection);
    messageLayout->addWidget(m_beginRoundButton);

    messageLayout->setStretchFactor(m_cancelSelection, 1);
    messageLayout->setStretchFactor(m_beginRoundButton, 1);
    messageLayout->setStretchFactor(m_musicPanel, 3);
    messageLayout->setStretchFactor(m_roleStatusPanel, 7);

    QHBoxLayout* layout = new QHBoxLayout();

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_battlefieldView);
    leftLayout->addWidget(m_roundStatusPanel);

    layout->addLayout(leftLayout);
    layout->addStretch(0);
    layout->addLayout(messageLayout);
    layout->setStretchFactor(leftLayout, 6);
    layout->setStretchFactor(messageLayout, 1);

    this->setLayout(layout);

    // Show the panel when a role is chosen
    connect(m_battlefieldView, &BattlefieldView::roleChosen, this, [=] { this->m_roleStatusPanel->show(); });
    connect(m_battlefieldView, &BattlefieldView::roleChosen, m_roleStatusPanel, &RoleStatusPanel::updateByRole);

    // hide the panel when the land is chosen
    connect(m_battlefieldView, &BattlefieldView::landChosen, this, [=] { this->m_roleStatusPanel->hide(); });

    // connect the attack and move button in the panel with the battlefieldview
    connect(m_roleStatusPanel, &RoleStatusPanel::attackAction, m_battlefieldView, &BattlefieldView::showAttackableRoles);
    connect(m_roleStatusPanel, &RoleStatusPanel::moveAction, m_battlefieldView, &BattlefieldView::showReachableLands);
    connect(m_roleStatusPanel, &RoleStatusPanel::skillAction, m_battlefieldView, &BattlefieldView::roleReleaseSkill);
    // update other message
    connect(m_battlefieldView, &BattlefieldView::focusChanged, this, &BattlefieldWidget::updateMessage);

    // the button used to cancel the Selection
    connect(m_cancelSelection, &QPushButton::clicked, m_battlefieldView, &BattlefieldView::closeAttackableRoles);
    connect(m_cancelSelection, &QPushButton::clicked, m_battlefieldView, &BattlefieldView::closeReachableLands);

    // connect the gameover signal from view to the slot
    connect(m_battlefieldView, &BattlefieldView::gameover, this, &BattlefieldWidget::gameover);

    // connect the end round button clicked to the BattlefieldView
    connect(m_roundStatusPanel, &roundStatusPanel::endTheRound, m_battlefieldView, &BattlefieldView::nextRound);
    connect(m_battlefieldView, &BattlefieldView::roundStatudChanged, m_roundStatusPanel, &roundStatusPanel::updateRoundStatus);

    // connect the AI start and end with the next round button show or hide
    connect(m_battlefieldView, &BattlefieldView::AIstart, m_roundStatusPanel, &roundStatusPanel::hideNextRound);
    connect(m_battlefieldView, &BattlefieldView::AIend, m_roundStatusPanel, &roundStatusPanel::showNextRound);

    connect(m_beginRoundButton, &QPushButton::clicked, this, &BattlefieldWidget::exitgame);
    m_battlefieldView->drawBattlefield();

    // the background music

    auto playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/new/bgm/src/BGM.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setPlaylist(playlist);
    musicPlayer->setVolume(30);
    musicPlayer->play();

    // connect the music panel with the music player
    connect(m_musicPanel, &MusicPanel::musicPlay, [=] { musicPlayer->play(); });
    connect(m_musicPanel, &MusicPanel::musicPause, [=] { musicPlayer->pause(); });
    connect(m_musicPanel, &MusicPanel::changeVolume, [=](int x) { musicPlayer->setVolume(x); });
}

void BattlefieldWidget::updateMessage(GraphicUnitInfo t_info) {
    std::string temp = "X: " + std::to_string(t_info.coordinateX) + "  Y: " + std::to_string(t_info.coordinateY);
    m_coordinateLabel->setText(QObject::tr(temp.c_str()));
    m_descriptionLabel->setText(t_info.Description);
}
