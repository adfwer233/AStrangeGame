#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>

struct GraphicUnitInfo {
    int     coordinateX, coordinateY;
    QString Description;
};

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
public:
    static const int INIT_SIZE = 50;

    GraphUnit();
    GraphUnit(int t_x, int t_y);


    virtual QRectF boundingRect() const override;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    QString      description() const;
    int          coordinateX() const;
    int          coordinateY() const;
    virtual bool isReachable() const;

    GraphicUnitInfo unitInfo() const;
signals:

private:

protected:
    void    initalizer();
    int     m_coordinateX, m_coordinateY;
    QString m_Description;
};

#endif  // GRAPHUNIT_H
