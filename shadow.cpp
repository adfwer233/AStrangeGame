#include "shadow.h"

ShadowLand::ShadowLand(int t_x, int t_y) : GraphLand(t_x, t_y) {}

void ShadowLand::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setBrush(QColor(192, 192, 192, 255));
    painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
}
