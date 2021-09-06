#ifndef ROLESTATUSPANEL_H
#define ROLESTATUSPANEL_H

#include "role.h"
#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

class RoleStatusPanel : public QWidget
{
    Q_OBJECT
public:
    explicit RoleStatusPanel(QWidget *parent = nullptr);
    explicit RoleStatusPanel(Role* t_role, QWidget *parent = nullptr);

    void lifeLossAnimation(int);
    void magicLossAnimation(int);
    void updateByRole(Role*);

signals:
    void moveAction(Role*);
    void attackAction(Role*);
    void skillAction(Role*, RoleSkill*);

private:
    QProgressBar* m_lifeValueBar;
    QProgressBar* m_magicValueBar;
    QLabel* m_damageLabel;
    QLabel* m_defenseLabel;
    QLabel* m_roundLabel;
    QLabel* m_teamLabel;
    QLabel* m_descriptionLabel;
    
    QGridLayout* m_mainLayout;
    QGroupBox* m_actionMenu;
    QPushButton* m_attackButton;
    QPushButton* m_moveButton;
    void initalizer();
};

#endif // ROLESTATUSPANEL_H
