#include "rolestatuspanel.h"

#include <QLayout>

void RoleStatusPanel::initalizer() {
    m_lifeValueBar = new QProgressBar();
    m_magicValueBar = new QProgressBar();
    m_damageLabel = new QLabel();
    m_defenseLabel = new QLabel();

    QGridLayout* layout = new QGridLayout();
    
    layout->addWidget(new QLabel(tr("生命值")), 0, 0, 1, 1);
    layout->addWidget(m_lifeValueBar, 0, 1, 1, 2);
    layout->addWidget(new QLabel(tr("魔法值")), 1, 0, 1, 1);
    layout->addWidget(m_magicValueBar, 1, 1, 1, 2);
    layout->addWidget(new QLabel(tr("攻击力")), 2, 0, 1, 1);
    layout->addWidget(m_damageLabel, 2, 1, 1, 2);
    layout->addWidget(new QLabel(tr("防御力")), 3, 0, 1, 1);
    layout->addWidget(m_defenseLabel, 3, 1, 1, 2);

    m_lifeValueBar->setMaximum(100);
    m_lifeValueBar->setMinimum(0);
    m_lifeValueBar->setValue(50);

    m_magicValueBar->setMaximum(100);
    m_magicValueBar->setMinimum(0);
    m_magicValueBar->setValue(50);

    m_damageLabel->setText(tr("10"));
    m_defenseLabel->setText(tr("5"));
    this->setLayout(layout);    
}

RoleStatusPanel::RoleStatusPanel(QWidget *parent) : QWidget(parent) {
    initalizer();
}

RoleStatusPanel::RoleStatusPanel(Role* t_role, QWidget *parent) : QWidget(parent) {
    initalizer();

    m_lifeValueBar->setMaximum(t_role->fullLifeValue());
    m_lifeValueBar->setValue(t_role->lifeValue());
    m_magicValueBar->setValue(t_role->fullMagicValue());
    m_magicValueBar->setValue(t_role->magicValue());
    m_damageLabel->setText(tr(std::to_string(t_role->damage()).c_str()));
    m_defenseLabel->setText(tr(std::to_string(t_role->defense()).c_str()));
}

void RoleStatusPanel::updateByRole(Role* const t_role) {

    m_lifeValueBar->setMaximum(t_role->fullLifeValue());
    m_lifeValueBar->setValue(std::max(0, t_role->lifeValue()));
    m_magicValueBar->setValue(t_role->fullMagicValue());
    m_magicValueBar->setValue(t_role->magicValue());
    m_damageLabel->setText(tr(std::to_string(t_role->damage()).c_str()));
    m_defenseLabel->setText(tr(std::to_string(t_role->defense()).c_str()));
    
}