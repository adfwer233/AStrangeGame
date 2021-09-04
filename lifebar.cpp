#include "lifebar.h"
#include "graphunit.h"
#include <QPainter>

LifeBar::LifeBar(QGraphicsItem* parent) : QGraphicsObject(parent) {
    connect(this, &LifeBar::valueChanged, [=]{ this->update(); });
}

void LifeBar::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setBrush(Qt::white);
    painter->drawRect(-GraphUnit::INIT_SIZE / 2, -GraphUnit::INIT_SIZE / 10, GraphUnit::INIT_SIZE, GraphUnit::INIT_SIZE / 5);
    qreal ratio = qreal(m_value) / qreal(m_maxValue);
    painter->setBrush(Qt::red);
    painter->drawRect(-GraphUnit::INIT_SIZE / 2, -GraphUnit::INIT_SIZE / 10, int(ratio * GraphUnit::INIT_SIZE), GraphUnit::INIT_SIZE / 5);
}

QRectF LifeBar::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(-GraphUnit::INIT_SIZE / 2 - penWidth / 2, -GraphUnit::INIT_SIZE / 2 - penWidth / 2, GraphUnit::INIT_SIZE + penWidth, GraphUnit::INIT_SIZE + penWidth);
}

void LifeBar::setMaxValue(int t_v) {
    m_maxValue = t_v;
}

void LifeBar::setValue(int t_v) {
    if (t_v == m_value)
        return;
    
    m_value = std::min(t_v, m_maxValue);
    emit valueChanged();
}

int LifeBar::value() const {
    return m_value;
}

int LifeBar::maxValue() const {
    return m_maxValue;
}