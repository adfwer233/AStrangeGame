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

private:
    QMovie* m_movie;
    QTimer* m_flashTimer;
};

#endif  // INFANTARY_H
