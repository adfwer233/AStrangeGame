#include "obstacle.h"

Obstacle::Obstacle(int t_x, int t_y) : GraphLand(t_x, t_y) {
    m_Description = tr("阻挡地形");
    m_backImage      = new QImage(":/new/roles/src/normalLand.png");
    m_image       = new QImage(":/new/roles/src/Obstacle.png");
}

void Obstacle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setBrush(Qt::black);
    painter->drawImage(boundingRect(), *m_backImage);
    painter->drawImage(boundingRect(), *m_image);
}

bool Obstacle::isReachable() const {
    return false;
}
