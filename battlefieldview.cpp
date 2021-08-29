#include "battlefieldview.h"

#include "graphland.h"
#include "graphunit.h"
#include "normalland.h"
#include "obstacle.h"

#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

BattlefieldView::BattlefieldView(QWidget* parent) : QGraphicsView(parent) {}

void BattlefieldView::mousePressEvent(QMouseEvent* event) {
    QPoint         viewPos  = event->pos();
    QPointF        scenePos = mapToScene(viewPos);
    QGraphicsItem* item     = scene()->itemAt(scenePos, QTransform());

    GraphUnit* graphUnit = dynamic_cast<GraphUnit*>(item);

    // there is a role observing
    if (graphUnit != nullptr && m_observingRole != nullptr) {
        if (graphUnit->inherits("GraphLand")) {
            GraphLand* graphland = static_cast<GraphLand*>(graphUnit);
            handleMoving(m_observingRole, graphland);
        }

        if (graphUnit->inherits("Role")) {
            Role* role = static_cast<Role*>(graphUnit);
            handleAttack(m_observingRole, role);
        }

        m_observingRole->clearFocus();
        scene()->update(m_observingRole->boundingRect());
        scene()->update(graphUnit->boundingRect());

        m_observingRole = nullptr;
        closeAttackableRoles();
        closeReachableLands();

        return;
    }

    // if a unit is clicked
    if (graphUnit != nullptr) {
        graphUnit->setFocus();
        emit focusChanged(graphUnit->unitInfo());

        if (graphUnit->inherits("GraphLand")) {
            GraphLand* graphland = static_cast<GraphLand*>(graphUnit);
            emit       landChosen(graphland);
        }

        if (graphUnit->inherits("Role")) {
            Role* t_role = static_cast<Role*>(graphUnit);
            emit  roleChosen(t_role);

            showReachableLands(t_role);
            showAttackableRoles(t_role);
        }
    }

    // QGraphicsView::mousePressEvent(event);
}

void BattlefieldView::resizeEvent(QResizeEvent* event) {
    QRectF boundary = scene()->itemsBoundingRect();
    this->fitInView(boundary, Qt::KeepAspectRatio);
}

void BattlefieldView::addLandItem(GraphLand* t_land, int t_x, int t_y) {
    t_land->setPos(t_x * GraphUnit::INIT_SIZE, t_y * GraphUnit::INIT_SIZE);
    scene()->addItem(t_land);
}

void BattlefieldView::addRoleItem(Role* t_role, int t_x, int t_y) {
    t_role->setPos(t_x * GraphUnit::INIT_SIZE, t_y * GraphUnit::INIT_SIZE);
    t_role->setZValue(1);
    scene()->addItem(t_role);

    connect(t_role, &Role::roleStatueChanged, this, [=] { emit roleChosen(t_role); });
    connect(t_role, &Role::roleStatueChanged, this, &BattlefieldView::updateBattlefield);
}

void BattlefieldView::drawBattlefield() {

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            GraphLand* newItem;
            if ((i * j) % 5 == 1)
                newItem = new Obstacle(i, j);
            else {
                newItem = new normalLand(i, j);
            }

            addLandItem(newItem, i, j);

            if (i == 0) {
                Role* roleItem = new Role(i, j, j % 2);
                addRoleItem(roleItem, i, j);
            }
        }
    }

    // ininialize the related status
    m_mapheight = 15;
    m_mapwidth  = 15;
    m_mapStatus.resize(m_mapwidth);
    for (auto& x : m_mapStatus)
        x.resize(m_mapheight);

    m_actionStatus.resize(m_mapwidth);
    for (auto& x : m_actionStatus)
        x.resize(m_mapheight);
}

void BattlefieldView::updateMapStatus() {
    auto allitem = scene()->items();
    qDebug() << allitem.size();
    for (auto item : allitem) {
        auto unit = dynamic_cast<GraphLand*>(item);
        if (unit != nullptr) {
            if (unit->inherits("normalLand")) {
                m_mapStatus[unit->coordinateX()][unit->coordinateY()] = reachable;
            }
            if (unit->inherits("Obstacle")) {
                m_mapStatus[unit->coordinateX()][unit->coordinateY()] = unreachable;
            }
        }
    }

    for (auto item : allitem) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr) {
            if (unit->inherits("Role")) {
                auto role = static_cast<Role*>(unit);
                m_mapStatus[role->coordinateX()][role->coordinateY()] = role->teamID() == teamOne ? teamOne : teamTwo;
            }
        }
    }
}

void BattlefieldView::showReachableLands(Role* t_role) {

    updateMapStatus();
    t_role->updateActionStatus(m_actionStatus, m_mapStatus);

    auto items = this->scene()->items();
    for (auto t_item : items) {
        GraphLand* unit = dynamic_cast<GraphLand*>(t_item);
        if (unit != nullptr) {
            if (m_actionStatus[unit->coordinateX()][unit->coordinateY()] == moveable) {
                unit->setShowingRechability(true);
            }
        }
    }
    t_role->setFocus();
    this->m_observingRole = t_role;
    this->scene()->update(scene()->itemsBoundingRect());
}

void BattlefieldView::closeReachableLands() {
    auto items = this->scene()->items();
    for (auto t_item : items) {
        GraphLand* unit = dynamic_cast<GraphLand*>(t_item);
        if (unit != nullptr) {
            unit->setShowingRechability(false);
        }
    }
    this->scene()->update(scene()->itemsBoundingRect());
}

void BattlefieldView::showAttackableRoles(Role* t_role) {

    updateMapStatus();
    t_role->updateActionStatus(m_actionStatus, m_mapStatus);

    auto items = this->scene()->items();
    for (auto t_item : items) {
        Role* unit = dynamic_cast<Role*>(t_item);
        if (unit != nullptr) {
            if (m_actionStatus[unit->coordinateX()][unit->coordinateY()] == attackable) {
                unit->setShowingAttackable(true);
            }
        }
    }
    this->m_observingRole = t_role;
    this->scene()->update(scene()->itemsBoundingRect());
}

void BattlefieldView::closeAttackableRoles() {
    auto items = this->scene()->items();
    for (auto t_item : items) {
        Role* unit = dynamic_cast<Role*>(t_item);
        if (unit != nullptr) {
            unit->setShowingAttackable(false);
        }
    }
    this->scene()->update(scene()->itemsBoundingRect());
}

/**
 * BattlefieldView
 * clear the died roles and make them unfocusable
 */
void BattlefieldView::updateBattlefield() {
    auto items = this->scene()->items();

    // clean the died roles
    for (auto t_item : items) {
        Role* unit = dynamic_cast<Role*>(t_item);
        if (unit != nullptr) {
            if (unit->lifeValue() <= 0) {
                unit->setFlag(QGraphicsItem::ItemIsFocusable, false);
                // QMessageBox::information(this, "help", "gg", QMessageBox::Ok);
            }
        }
    }
    this->scene()->update(scene()->itemsBoundingRect());
}

/**
 * 人物移动的动画与结算
 *
 * @param  {Role*} t_role      : 想要移动的角色指针
 * @param  {GraphLand*} t_land : 目标地点
 */
void BattlefieldView::handleMoving(Role* t_role, GraphLand* t_land) {
    if (t_land->isShowingReachability() == true && t_role != nullptr) {
        QPropertyAnimation* moveAnimation = new QPropertyAnimation(t_role, "pos");
        moveAnimation->setDuration(1000);
        moveAnimation->setStartValue(t_role->pos());
        moveAnimation->setEndValue(t_land->pos());
        moveAnimation->start();

        t_role->setPos(this->pos());
        t_role->setCoordinate(t_land->coordinateX(), t_land->coordinateY());
        t_role = nullptr;
    }
}

/**
 * 普通攻击的动画与结算
 *
 * @param  {Role*} t_sender : 攻击发出者
 * @param  {Role*} t_target : 被攻击者
 */
void BattlefieldView::handleAttack(Role* t_sender, Role* t_target) {

    if (t_sender->teamID() == t_target->teamID()) {
        return;
    }

    if (t_target->isShowingAttackable() == true && t_sender != nullptr) {

        QPropertyAnimation* animationGo = new QPropertyAnimation(t_sender, "pos");
        animationGo->setStartValue(t_sender->pos());
        animationGo->setEndValue(t_target->pos());
        animationGo->setDuration(1000);

        QPropertyAnimation* animationBack = new QPropertyAnimation(t_sender, "pos");
        animationBack->setStartValue(t_target->pos());
        animationBack->setEndValue(t_sender->pos());
        animationGo->setDuration(1000);

        QSequentialAnimationGroup* animationGroup = new QSequentialAnimationGroup();
        animationGroup->addAnimation(animationGo);
        animationGroup->addAnimation(animationBack);
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
        // settle the damage caused by the attacker
        t_target->settleLifeLoss(t_sender->damage());
        return;
    }
}