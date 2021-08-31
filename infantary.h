#ifndef INFANTARY_H
#define INFANTARY_H

#include "role.h"
#include "statusConstant.h"
#include <QMovie>
#include <QTimer>

class Infantary : public Role {
    Q_OBJECT
public:
    Infantary(int t_x, int t_y, int t_team);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    virtual void updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) override;
    
private:
    QMovie* m_movie;
};

#endif  // INFANTARY_H
