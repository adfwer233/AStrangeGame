#include "graphunit.h"
#include <QCursor>
#include <QDebug>
#include <QMessageBox>

void GraphUnit::initalizer() {
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
}

GraphUnit::GraphUnit() {
    initalizer();
}

GraphUnit::GraphUnit(int t_x, int t_y) : m_coordinateX(t_x), m_coordinateY(t_y) {
    initalizer();
    m_Description = QObject::tr("抽象地图单元");
}

void GraphUnit::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (hasFocus()) {
        painter->setBrush(Qt::blue);
    }
    else {
        painter->setBrush(Qt::red);
    }
    painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
}

QRectF GraphUnit::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(-INIT_SIZE / 2 - penWidth / 2, -INIT_SIZE / 2 - penWidth / 2, INIT_SIZE + penWidth, INIT_SIZE + penWidth);
}


GraphicUnitInfo GraphUnit::unitInfo() const {
    return GraphicUnitInfo{ coordinateX(), coordinateY(), description() };
}

QString GraphUnit::description() const {
    return m_Description;
}

int GraphUnit::coordinateX() const {
    return m_coordinateX;
}

int GraphUnit::coordinateY() const {
    return m_coordinateY;
}

bool GraphUnit::isReachable() const {
    return true;
}