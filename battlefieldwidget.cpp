#include "battlefieldWidget.h"
#include <QLayout>
#include <QMessageBox>

BattlefieldWidget::BattlefieldWidget(QWidget* parent) : QWidget(parent) {
    m_battlefieldView     = new BattlefieldView();
    QGraphicsScene* scene = new QGraphicsScene();

    m_battlefieldView->setScene(scene);

    QVBoxLayout* messageLayout = new QVBoxLayout();
    m_coordinateLabel          = new QLabel(QObject::tr("这里将会显示坐标"));
    m_descriptionLabel         = new QLabel(QObject::tr("Description"));
    m_beginRoundButton         = new QPushButton(tr("开始回合"));
    m_cancelSelection          = new QPushButton(tr("取消选择"));
    m_roleStatusPanel          = new RoleStatusPanel();
    m_roundStatusPanel         = new roundStatusPanel();

    messageLayout->addWidget(m_roleStatusPanel);
    messageLayout->addWidget(m_coordinateLabel);
    messageLayout->addWidget(m_descriptionLabel);
    messageLayout->addWidget(m_cancelSelection);
    messageLayout->addWidget(m_beginRoundButton);

    QHBoxLayout* layout = new QHBoxLayout();

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_battlefieldView);
    leftLayout->addWidget(m_roundStatusPanel);

    layout->addLayout(leftLayout);
    layout->addLayout(messageLayout);

    this->setLayout(layout);

    // Show the panel when a role is chosen
    connect(m_battlefieldView, &BattlefieldView::roleChosen, this, [=] { this->m_roleStatusPanel->show(); });
    connect(m_battlefieldView, &BattlefieldView::roleChosen, m_roleStatusPanel, &RoleStatusPanel::updateByRole);

    // hide the panel when the land is chosen
    connect(m_battlefieldView, &BattlefieldView::landChosen, this, [=] { this->m_roleStatusPanel->hide(); });

    // connect the attack and move button in the panel with the battlefieldview
    connect(m_roleStatusPanel, &RoleStatusPanel::attackAction, m_battlefieldView, &BattlefieldView::showAttackableRoles);
    connect(m_roleStatusPanel, &RoleStatusPanel::moveAction, m_battlefieldView, &BattlefieldView::showReachableLands);

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

    m_battlefieldView->drawBattlefield();
}

void BattlefieldWidget::updateMessage(GraphicUnitInfo t_info) {
    std::string temp = "X: " + std::to_string(t_info.coordinateX) + "  Y: " + std::to_string(t_info.coordinateY);
    m_coordinateLabel->setText(QObject::tr(temp.c_str()));
    m_descriptionLabel->setText(t_info.Description);
}

void BattlefieldWidget::gameover(coordinateStatus t_winner) {
    QMessageBox::information(this, "!!!", "Good Game", QMessageBox::Ok);
}