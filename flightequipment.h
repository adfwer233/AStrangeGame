#ifndef FLIGHTEQUIPMENT_H
#define FLIGHTEQUIPMENT_H

#include <QGraphicsObject>
#include <QObject>

class FlightEquipment : public QGraphicsObject {
    Q_OBJECT
public:
	FlightEquipment(QGraphicsItem* parent = nullptr) : QGraphicsObject(parent) {
        setFlag(QGraphicsItem::ItemIsFocusable, false);
    }

    ~FlightEquipment() {
        delete m_image;
    }

    QRectF boundingRect() const override;
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void   animationReact(QPointF t_origin, QPointF t_destination, QGraphicsScene* t_scene);

protected:
    QImage* m_image;
signals:
    void animationFinished();
};

class FlightEquipmentArrow : public FlightEquipment {
    Q_OBJECT
public:
    FlightEquipmentArrow();
};

#endif  // FLIGHTEQUIPMENT_H
