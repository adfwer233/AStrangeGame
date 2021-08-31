#include "archer.h"
#include "algorithm.h"
#include "flightequipment.h"

#include <QGraphicsItem>

Archer::Archer(int t_x, int t_y, int t_team) : Role(t_x, t_y, t_team) {
    m_Description = tr("弓箭手");
    m_lifeValue   = 75;
    m_damage      = 35 + rand() % 5;

    if (t_team == 0) {
        m_movie = new QMovie(":/new/roles/src/archer1_up.gif");
    }
    else if (t_team == 1) {
        m_movie = new QMovie(":/new/roles/src/archer2_right.gif");
    }
    m_movie->start();
}

void Archer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
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

void Archer::updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) {
    
    Algorithm::findPathBFS(t_coordinate, t_actionStatus, 3, 7, std::make_pair(coordinateX(), coordinateY()));

    const int x = this->coordinateX();
    const int y = this->coordinateY();

    for (int i = 0; i < t_coordinate.size(); i++)
        for (int j = 0; j < t_coordinate[i].size(); j++)
            if ((t_coordinate[i][j] == teamOne || t_coordinate[i][j] == teamTwo) && (t_coordinate[i][j] != this->teamID()))
                if ((i - x)^2 + (j - y)^2 <= 49)
                    t_actionStatus[i][j] = attackable;
}

void Archer::handleAttack(Role* t_target, QList<GraphUnit*>) {

    if (this->teamID() == t_target->teamID()) {
        return;
    }

    if (t_target->isShowingAttackable() == true && this != nullptr) {
        FlightEquipmentArrow* arrow = new FlightEquipmentArrow();
        arrow->animationReact(this->pos(), t_target->pos(), scene());
        t_target->settleLifeLoss(this->damage());
        this->setroundFinished(true);
        return;
    }
}
