#ifndef LIFEBAR_H
#define LIFEBAR_H

#include <QGraphicsObject>

class LifeBar : public QGraphicsObject {
    Q_OBJECT
public:
    LifeBar(QGraphicsItem* parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    Q_PROPERTY(int value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged);

    void setMaxValue(int);
    void setValue(int);
    int maxValue() const;
    int value() const;

signals:
    void valueChanged();

private:
    int m_maxValue;
    int m_value;
};

#endif  // LIFEBAR_H
