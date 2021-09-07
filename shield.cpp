#include "shield.h"
#include "algorithm.h"
Shield::Shield(int t_x, int t_y, int t_team) : Role(t_x, t_y, t_team) {

    m_Description = tr("盾牌兵");
    m_lifeValue = m_fullLifeValue = 120;
    m_damage = 20 + rand() % 3;
    m_defense = 15;

    if (t_team == 0) {
        m_movie = new QMovie(":/new/roles/src/shield.gif");
    }
    else if (t_team == 1){
        m_movie = new QMovie(":/new/roles/src/shield2.gif");
    }
    m_movie->start();

    m_skills.push_back(new Explosion(this));
}

void Shield::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
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
    
    if (this->m_roundFinished == false) {
        painter->setBrush(QColor(255, 215, 0, 100));
        painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
    }

    if (m_movie != nullptr && m_movie->state() == QMovie::Running) {
        painter->drawImage(boundingRect(), m_movie->currentImage());
    }
}

void Shield::updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) {
    Algorithm::findPathBFS(t_coordinate, t_actionStatus, 5, 3, std::make_pair(coordinateX(), coordinateY()));
}
