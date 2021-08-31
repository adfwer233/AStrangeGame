#include "flightequipment.h"
#include "graphunit.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPropertyAnimation>
#include <QtMath>
#include <QDebug>

void FlightEquipment::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->drawImage(boundingRect(), *m_image);
}

QRectF FlightEquipment::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(-GraphUnit::INIT_SIZE / 2 - penWidth / 2, -GraphUnit::INIT_SIZE / 2 - penWidth / 2, GraphUnit::INIT_SIZE + penWidth,
                  GraphUnit::INIT_SIZE + penWidth);
}

void FlightEquipment::animationReact(QPointF t_origin, QPointF t_destination, QGraphicsScene* t_scene) {

    this->setPos(t_origin);
    this->setRotation(180 + 180 / 3.1415 * (qAtan2(t_destination.y() - t_origin.y(), t_destination.x() - t_origin.x())));

    t_scene->addItem(this);

    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(200);
    animation->setStartValue(t_origin);
    animation->setEndValue(t_destination);

    // remove the flight equipment when finished the animation
    connect(animation, &QPropertyAnimation::finished, [=] { t_scene->removeItem(this); });

    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

FlightEquipmentArrow::FlightEquipmentArrow() : FlightEquipment() {
    m_image = new QImage(":/new/roles/src/arrow.png");
}