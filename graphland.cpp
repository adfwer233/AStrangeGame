#include "graphland.h"
#include <QCursor>
#include <QPropertyAnimation>

GraphLand::GraphLand(int t_x, int t_y) : GraphUnit(t_x, t_y) {
    m_isShowingReachability = false;
}

void GraphLand::setShowingRechability(bool t_value) {
    m_isShowingReachability = t_value;
}

bool GraphLand::isShowingReachability() const {
    return m_isShowingReachability;
}

void GraphLand::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {

    if (isShowingReachability()) {
        painter->setBrush(Qt::green);
        painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
        return;
    }

    if (hasFocus()) {
        painter->setBrush(Qt::blue);
    }
    else {
        painter->setBrush(Qt::red);
    }
    painter->drawRect(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
}
