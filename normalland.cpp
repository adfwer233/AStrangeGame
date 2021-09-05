#include "normalland.h"

normalLand::normalLand(int t_x, int t_y) : GraphLand(t_x, t_y) {
    m_Description = "普通的地形";
    m_image       = new QImage(":/new/roles/src/normalLand.png");
}

void normalLand::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {

    if (isShowingReachability()) {
        painter->setBrush(Qt::green);
        painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
        return;
    }

    if (hasFocus()) {
        painter->setBrush(Qt::blue);
        painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
    }
    else {
        painter->setBrush(Qt::red);
        painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
    }

    if (m_image != nullptr) {
        painter->drawImage(boundingRect(), *m_image);
    }
}
