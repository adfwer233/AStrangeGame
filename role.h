#ifndef ROLE_H
#define ROLE_H

#include "graphunit.h"
#include "roleskill.h"
#include "statusConstant.h"
#include "rolebuff.h"
#include "graphland.h"
#include <QGraphicsItem>
#include <QString>
#include <QVector>
#include <QList>

class Role : public GraphUnit {
    Q_OBJECT
public:

    static const int MOVE_ACTION_POINT = 1;
    static const int ATTACK_ACTION_POINT = 1;

    Role(int t_x, int t_y, int t_team);

    virtual QRectF boundingRect() const override;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    bool isShowingAttackable() const;
    void setShowingAttackable(bool);

    coordinateStatus teamID() const;

    int   lifeValue() const;
    int   magicValue() const;
    int   fullLifeValue() const;
    int   fullMagicValue() const;
    int   damage() const;
    int   defense() const;
    bool  isroundFinished() const;
    void  setCoordinate(int x, int y);
    void  setroundFinished(bool);
    Role* observingRole() const;

    // function of buff system
    void addBuff(RoleBuff* t_buff);
    void settleBuff();

    // member functions of skill system
    void addSkill(RoleSkill* t_skill);
    QList<RoleSkill*> skillList() const;

    // the virtual functions 
    virtual void settleLifeLoss(int t_damage);
    virtual void updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate);
    virtual bool handleAttack(Role* t_target, QList<GraphUnit*> t_list); 

protected:
    // basic properties for roles
    int              m_lifeValue;
    int              m_fullLifeValue;
    int              m_magicValue;
    int              m_fullmagicValue;
    int              m_damage;
    int              m_defense;
    coordinateStatus m_teamID;

    // properties to achieve the Round System
    bool m_roundFinished;

    bool m_isShowingAttackable;

    // buff system
    QList<RoleBuff*> m_buffs;

    //skill system
    QList<RoleSkill*> m_skills;

signals:
    void roleStatueChanged(Role*);
    void actionFinished();
    void deathProcess(Role*);

public slots:
    void repaint();
    void releaseSkill(RoleSkill*);
    void AIaction(BattlefieldView* t_view);
};

#endif  // ROLE_H
