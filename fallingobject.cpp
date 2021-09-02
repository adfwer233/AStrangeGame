#include "fallingobject.h"

FallingObject::FallingObject(int t_x, int t_y) : GraphUnit(t_x, t_y) {
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
    
    if (m_image != nullptr)
        painter->drawImage(this->boundingRect(), *m_image);
}