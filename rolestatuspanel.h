#ifndef ROLESTATUSPANEL_H
#define ROLESTATUSPANEL_H

#include "role.h"
#include <QWidget>
#include <QLabel>
#include <QProgressBar>

class RoleStatusPanel : public QWidget
{
    Q_OBJECT
public:
    explicit RoleStatusPanel(QWidget *parent = nullptr);
    explicit RoleStatusPanel(Role* t_role, QWidget *parent = nullptr);

    void lifeLossAnimation(int);
    void magicLossAnimation(int);
    void updateByRole(Role* const);

signals:

private:
    QProgressBar* m_lifeValueBar;
    QProgressBar* m_magicValueBar;
    QLabel* m_damageLabel;
    QLabel* m_defenseLabel;

    void initalizer();
};

#endif // ROLESTATUSPANEL_H
