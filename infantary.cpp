#include "infantary.h"
#include "algorithm.h"
Infantary::Infantary(int t_x, int t_y, int t_team) : Role(t_x, t_y, t_team) {

    m_lifeValue = m_fullLifeValue = 100;
    m_damage = rand() % 3;
    m_defense = 5;

    if (t_team == 0) {
        m_movie = new QMovie(":/new/roles/src/infantary.gif");
    }
    else if (t_team == 1){
        m_movie = new QMovie(":/new/roles/src/infantary2.gif");
    }
    m_movie->start();
}

void Infantary::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
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

void Infantary::updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) {
    Algorithm::findPathBFS(t_coordinate, t_actionStatus, 5, 3, std::make_pair(coordinateX(), coordinateY()));
}
