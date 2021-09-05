#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "graphland.h"

class Obstacle : public GraphLand {
    Q_OBJECT
public:
    Obstacle(int t_x, int t_y);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    virtual bool isReachable() const override;
    
    QImage* m_backImage;
    QImage* m_image;
};

#endif  // OBSTACLE_H
