#include "role.h"
#include "lifebar.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
Role::Role(int t_x, int t_y, int t_team) : GraphUnit(t_x, t_y) {
    m_Description   = tr("抽象人物单元");
    m_lifeValue     = 50;
    m_fullLifeValue = 50;
    m_damage        = 20 + rand() % 5;
    m_defense       = 5;
    m_teamID        = t_team == 0 ? teamOne : teamTwo;
    m_fullmagicValue = 100;
    m_magicValue = 20;

    m_isShowingAttackable = 0;
}

QRectF Role::boundingRect() const {
    if (this->lifeValue() <= 0) {
        qreal penWidth = 1;
        return QRectF(-INIT_SIZE / 8 - penWidth / 2, -INIT_SIZE / 8 - penWidth / 2, INIT_SIZE / 4 + penWidth, INIT_SIZE / 4 + penWidth);
    }
    return GraphUnit::boundingRect();
}

void Role::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {

    // if (lifeValue() <= 0) {
    //     painter->setBrush(Qt::magenta);
    //     painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
    //     painter->drawText(0, 0, tr("die"));
    //     return;
    // }

    if (isShowingAttackable()) {
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
        return;
    }

    if (hasFocus()) {
        painter->setBrush(Qt::blue);
    }

    if (this->m_roundFinished == false) {
        painter->setBrush(QColor(255, 215, 0));
    }
    painter->drawEllipse(-INIT_SIZE / 2, -INIT_SIZE / 2, INIT_SIZE, INIT_SIZE);
}

void Role::settleLifeLoss(int t_damage) {

    int loss = t_damage * (500)/ (500 + this->defense());

    if (loss < 0)
        return;
    else {
        Algorithm::LifeChangeAnimation(this, -loss);
        this->m_lifeValue -= loss;
        emit roleStatueChanged(this);

        if (m_lifeValue <= 0) {
            emit deathProcess(this);
        }
    }
}

/**
 * Role
 * find the moveable and attackable targets (a virtual function)
 * @param  {QVector<QVector<actionStatus>>} t_actionStatus   :
 * @param  {QVector<QVector<coordinateStatus>>} t_coordinate :
 */
void Role::updateActionStatus(QVector<QVector<actionStatus>>& t_actionStatus, const QVector<QVector<coordinateStatus>>& t_coordinate) {
    for (int i = 0; i < t_coordinate.size(); i++) {
        for (int j = 0; j < t_coordinate[i].size(); j++) {

            t_actionStatus[i][j] = none;
            if (t_coordinate[i][j] == teamOne && this->teamID() == teamTwo && abs(i - this->coordinateX()) + abs(j - this->coordinateY()) < 5) {
                t_actionStatus[i][j] = attackable;
            }
            else if (t_coordinate[i][j] == teamTwo && this->teamID() == teamOne && abs(i - this->coordinateX()) + abs(j - this->coordinateY()) < 5) {
                t_actionStatus[i][j] = attackable;
            }

            if (t_coordinate[i][j] == reachable && abs(i - this->coordinateX()) + abs(j - this->coordinateY()) < 10) {
                t_actionStatus[i][j] = moveable;
            }
        }
    }
}

bool Role::handleAttack(Role* t_target, QList<GraphUnit*> t_list) {
    if (this->teamID() == t_target->teamID()) {
        return false;
    }

	if (t_target->isShowingAttackable() == true) {

        QSequentialAnimationGroup* animationGroup = new QSequentialAnimationGroup();
        for (int i = 1; i < t_list.size(); i++) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(t_list[i - 1]->pos());
            moveAnimation->setEndValue(t_list[i]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        for (int i = t_list.size() - 1; i >= 1; i--) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(t_list[i]->pos());
            moveAnimation->setEndValue(t_list[i - 1]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
        connect(animationGroup, &QSequentialAnimationGroup::finished, this, &Role::actionFinished);
        this->setroundFinished(true);
        t_target->settleLifeLoss(this->damage());
        return true;
    }

    return false;
}

void Role::settleBuff() {
    for (auto item : m_buffs) {
        int tmp_life = m_lifeValue;
        item->lifeValueBuff(m_lifeValue);
        if (tmp_life != m_lifeValue) {
            Algorithm::LifeChangeAnimation(this, tmp_life, m_lifeValue);
        }
        m_lifeValue = std::min(m_lifeValue, m_fullLifeValue);
        item->magicValueBuff(m_magicValue);
        m_magicValue = std::min(m_magicValue, m_fullmagicValue);
        item->damageBuff(m_damage);
        item->defenseBuff(m_defense);
    }
    m_buffs.clear();
}

void Role::AIaction(BattlefieldView* t_view) {
    Algorithm::basicAI(this, t_view);
}

bool Role::releaseSkill(RoleSkill* t_skill, bool byAI) {

    if (t_skill->magicPointCost() > m_magicValue) {
        if (byAI == false)
            QMessageBox::information(nullptr, tr("技能施放"), tr("魔法值不足，技能施放失败"));
        return false;
    }

    m_magicValue -= t_skill->magicPointCost();
    t_skill->releaseSkill(this, scene());
    return true;
}

void Role::setShowingAttackable(bool t_value) {
    m_isShowingAttackable = t_value;
}

bool Role::isShowingAttackable() const {
    return m_isShowingAttackable;
}

coordinateStatus Role::teamID() const {
    return m_teamID;
}

int Role::lifeValue() const {
    return m_lifeValue;
}

int Role::damage() const {
    return m_damage;
}

int Role::defense() const {
    return m_defense;
}

int Role::fullLifeValue() const {
    return m_fullLifeValue;
}

int Role::magicValue() const {
    return m_magicValue;
}

int Role::fullMagicValue() const {
    return m_fullmagicValue;
}

void Role::setCoordinate(int x, int y) {
    m_coordinateX = x;
    m_coordinateY = y;
}

bool Role::isroundFinished() const {
    return m_roundFinished;
}

void Role::setroundFinished(bool value) {
    m_roundFinished = value;
}

void Role::repaint() {
    scene()->update(boundingRect());
}

void Role::addBuff(RoleBuff* t_buff) {
    m_buffs.push_back(t_buff);
}

void Role::addSkill(RoleSkill* t_skill) {
    m_skills.push_back(t_skill);
}

QList<RoleSkill*> Role::skillList() const {
    return m_skills;
}

void Role::selfHeat() {
    m_magicValue = std::min(m_fullmagicValue, m_magicValue += 2);
}

QMovie* Role::roleMovie() const {
    return m_movie;
}