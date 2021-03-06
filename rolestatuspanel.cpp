#include "rolestatuspanel.h"
#include "battlefieldview.h"
#include <QDebug>
#include <QPushButton>

void RoleStatusPanel::initalizer() {
    m_lifeValueBar     = new QProgressBar();
    m_magicValueBar    = new QProgressBar();
    m_damageLabel      = new QLabel();
    m_defenseLabel     = new QLabel();
    m_teamLabel        = new QLabel();
    m_roundLabel       = new QLabel();
    m_attackButton     = new QPushButton();
    m_moveButton       = new QPushButton();
    m_mainLayout       = new QGridLayout();
    m_descriptionLabel = new QLabel();
    m_imageLabel       = new QLabel();
    m_actionMenu       = nullptr;

    m_mainLayout->addWidget(m_imageLabel, 0, 0, 2, 2);
    m_mainLayout->addWidget(new QLabel(tr("队伍")), 2, 0, 1, 1);
    m_mainLayout->addWidget(m_teamLabel, 2, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel("角色"), 3, 0, 1, 1);
    m_mainLayout->addWidget(m_descriptionLabel, 3, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel("本轮状态"), 4, 0, 1, 1);
    m_mainLayout->addWidget(m_roundLabel, 4, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel(tr("生命值")), 5, 0, 1, 1);
    m_mainLayout->addWidget(m_lifeValueBar, 5, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel(tr("魔法值")), 6, 0, 1, 1);
    m_mainLayout->addWidget(m_magicValueBar, 6, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel(tr("攻击力")), 7, 0, 1, 1);
    m_mainLayout->addWidget(m_damageLabel, 7, 1, 1, 2);
    m_mainLayout->addWidget(new QLabel(tr("防御力")), 8, 0, 1, 1);
    m_mainLayout->addWidget(m_defenseLabel, 8, 1, 1, 2);

    m_lifeValueBar->setMaximum(100);
    m_lifeValueBar->setMinimum(0);
    m_lifeValueBar->setValue(0);

    m_magicValueBar->setMaximum(100);
    m_magicValueBar->setMinimum(0);
    m_magicValueBar->setValue(0);

    m_damageLabel->setText(tr(""));
    m_defenseLabel->setText(tr(""));

    m_moveButton->setText("移动");
    m_attackButton->setText("攻击");

    this->setLayout(m_mainLayout);

    this->setStyleSheet(Constant::panelQSS);
}

RoleStatusPanel::RoleStatusPanel(QWidget* parent) : QWidget(parent) {
    initalizer();
}

RoleStatusPanel::RoleStatusPanel(Role* t_role, QWidget* parent) : QWidget(parent) {
    initalizer();

    m_lifeValueBar->setMaximum(t_role->fullLifeValue());
    m_lifeValueBar->setValue(t_role->lifeValue());
    m_magicValueBar->setValue(t_role->fullMagicValue());
    m_magicValueBar->setValue(t_role->magicValue());
    m_damageLabel->setText(tr(std::to_string(t_role->damage()).c_str()));
    m_defenseLabel->setText(tr(std::to_string(t_role->defense()).c_str()));
}

void RoleStatusPanel::updateByRole(Role* t_role) {

    if (m_actionMenu != nullptr) {
        m_actionMenu->hide();
    }

    m_imageLabel->setMovie(t_role->roleMovie());

    m_lifeValueBar->setMaximum(t_role->fullLifeValue());
    m_lifeValueBar->setValue(std::max(0, t_role->lifeValue()));
    m_magicValueBar->setMaximum(t_role->fullMagicValue());
    m_magicValueBar->setValue(std::max(0, t_role->magicValue()));
    m_damageLabel->setText(tr(std::to_string(t_role->damage()).c_str()));
    m_defenseLabel->setText(tr(std::to_string(t_role->defense()).c_str()));
    m_descriptionLabel->setText(t_role->description());
    
    if (t_role->teamID() == teamOne) {
        m_teamLabel->setText(tr("队伍一"));
    }
    else {
        m_teamLabel->setText(tr("队伍二"));
    }

    if (t_role->isroundFinished() == false) {
        m_roundLabel->setText(tr("可移动"));
    }
    else {
        m_roundLabel->setText(tr("不可移动"));
    }

    if (true) {
        m_actionMenu           = new QGroupBox(this);
        QVBoxLayout* tmpLayout = new QVBoxLayout(m_actionMenu);

        QLabel* tmpLabel  = new QLabel(tr("技能栏"));
        auto    tmpPolicy = tmpLabel->sizePolicy();
        tmpPolicy.setVerticalPolicy(QSizePolicy::Minimum);
        tmpLabel->setSizePolicy(tmpPolicy);

        tmpLayout->addWidget(tmpLabel);

        for (auto item : t_role->skillList()) {
            QPushButton* button = new QPushButton(this);
            button->setText(item->skillName());
            tmpLayout->addWidget(button);
            
            if (t_role->teamID() == teamOne)
                connect(button, &QPushButton::clicked, [=] { emit skillAction(t_role, item, false); });

            auto policy = button->sizePolicy();
            // policy.setVerticalPolicy(QSizePolicy::Expanding);
            policy.setHorizontalPolicy(QSizePolicy::Expanding);
            button->setSizePolicy(policy);
        }

        m_actionMenu->setLayout(tmpLayout);

        auto policy = m_actionMenu->sizePolicy();
        policy.setHorizontalPolicy(QSizePolicy::Expanding);
        m_actionMenu->setSizePolicy(policy);

        m_mainLayout->addWidget(m_actionMenu, 9, 0);
    }
}
