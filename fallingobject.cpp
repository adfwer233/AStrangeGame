#include "fallingobject.h"

FallingObject::FallingObject(int t_x, int t_y) : GraphLand(t_x, t_y) {
    m_buff = nullptr;
}

RoleBuff* FallingObject::getBuff() const {
    return m_buff;
}

void FallingObject::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (hasFocus()) {
        painter->setBrush(Qt::blue);
    }
    else {
        painter->setBrush(Qt::red);
    }

    if (m_image != nullptr) {
        qreal penWidth = 1;
        QRectF rect = QRectF(-INIT_SIZE / 4 - penWidth / 2, -INIT_SIZE / 4 - penWidth / 2, INIT_SIZE / 2 + penWidth, INIT_SIZE / 2+ penWidth);
        painter->drawImage(rect, *m_image);
    }
}