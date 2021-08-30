#ifndef ROLE_H
#define ROLE_H

#include "graphunit.h"
#include "roleskill.h"
#include "statusConstant.h"
#include <QGraphicsItem>
#include <QString>
#include <QVector>

class Role : public GraphUnit {
    Q_OBJECT
public:
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

    virtual void settleLifeLoss(int t_damage);

    void updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate);

private:
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

signals:
    void roleStatueChanged(Role*);
};

#endif  // ROLE_H
