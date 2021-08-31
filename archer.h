#ifndef ARCHER_H
#define ARCHER_H

#include "role.h"
#include <QMovie>
class Archer : public Role {
    Q_OBJECT
public:
    Archer(int t_x, int t_y, int t_team);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    virtual void updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) override;
    virtual void handleAttack(Role* t_target, QList<GraphUnit*> t_list);
private:
    QMovie *m_movie;
};

#endif // ARCHER_H
