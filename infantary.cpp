#include "infantary.h"

Infantary::Infantary(int t_x, int t_y, int t_team) : Role(t_x, t_y, t_team) {
    if (t_team == teamOne) {
        m_movie = new QMovie(":/new/roles/src/infantary.gif");
    }
    else {
        m_movie = new QMovie(":/new/roles/src/infantary.gif");
    }

    m_movie->start();
}

void Infantary::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    if (lifeValue() <= 0) {
        painter->setBrush(Qt::magenta);
        painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
        painter->drawText(0, 0, tr("die"));
        return;
    }

    if (isShowingAttackable()) {
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
        if (m_movie != nullptr && m_movie->state() == QMovie::Running) {
            painter->drawImage(boundingRect(), m_movie->currentImage());
        }
        return;
    }

    if (hasFocus()) {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
    }

    if (m_movie != nullptr && m_movie->state() == QMovie::Running) {
        painter->drawImage(boundingRect(), m_movie->currentImage());
    }
}