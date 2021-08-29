#include "obstacle.h"

Obstacle::Obstacle(int t_x, int t_y) : GraphLand(t_x, t_y) {
    m_Description = tr("阻挡地形");
}

void Obstacle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setBrush(Qt::black);
    painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
}

bool Obstacle::isReachable() const {
    return false;
}
