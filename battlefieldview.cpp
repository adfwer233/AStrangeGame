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
#include <QQueue>
#include <QSequentialAnimationGroup>

BattlefieldView::BattlefieldView(QWidget* parent) : QGraphicsView(parent) {
    m_roundNumber = 0;
}

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
        
        m_observingRole->setroundFinished(true);

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

            if (t_role->isroundFinished() == false) {
                showReachableLands(t_role);
                showAttackableRoles(t_role);
            }
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

    m_topUnit.resize(m_mapwidth);
    for (auto& x : m_topUnit)
        x.resize(m_mapheight);

    initalizeRound(teamOne);
}

void BattlefieldView::updateMapStatus() {
    auto allitem = scene()->items();
    for (auto item : allitem) {
        auto unit = dynamic_cast<GraphLand*>(item);
        if (unit != nullptr) {
            if (unit->inherits("normalLand")) {
                m_mapStatus[unit->coordinateX()][unit->coordinateY()] = reachable;
            }
            if (unit->inherits("Obstacle")) {
                m_mapStatus[unit->coordinateX()][unit->coordinateY()] = unreachable;
            }

            m_topUnit[unit->coordinateX()][unit->coordinateY()] = unit;
        }
    }

    for (auto item : allitem) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr) {
            if (unit->inherits("Role")) {
                auto role                                             = static_cast<Role*>(unit);
                m_mapStatus[role->coordinateX()][role->coordinateY()] = role->teamID() == teamOne ? teamOne : teamTwo;
            }

            m_topUnit[unit->coordinateX()][unit->coordinateY()] = unit;
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
 * when one team has no role living, emit the game overslot
 */
void BattlefieldView::updateBattlefield() {
    auto items = this->scene()->items();

    // clean the died roles
    int teamoneNum = 0;
    int teamtwoNum = 0;

    for (auto t_item : items) {
        Role* unit = dynamic_cast<Role*>(t_item);
        if (unit != nullptr) {
            if (unit->lifeValue() <= 0) {
                unit->setFlag(QGraphicsItem::ItemIsFocusable, false);
                // QMessageBox::information(this, "help", "gg", QMessageBox::Ok);
            }
            else {
                if (unit->teamID() == teamOne)
                    teamoneNum++;
                else if (unit->teamID() == teamTwo)
                    teamtwoNum++;
            }
        }
    }
    this->scene()->update(scene()->itemsBoundingRect());

    if (teamoneNum == 0)
        emit gameover(teamTwo);
    else if (teamtwoNum == 0)
        emit gameover(teamOne);
}

/**
 * 人物移动的动画与结算
 *
 * @param  {Role*} t_role      : 想要移动的角色指针
 * @param  {GraphLand*} t_land : 目标地点
 */
void BattlefieldView::handleMoving(Role* t_role, GraphLand* t_land) {
    if (t_land->isShowingReachability() == true && t_role != nullptr) {

        auto items = getPath(t_role, t_land);
        t_role->setPos(this->pos());

        QSequentialAnimationGroup* animationGroup = new QSequentialAnimationGroup();
        for (int i = 1; i < items.size(); i++) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(t_role, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(items[i - 1]->pos());
            moveAnimation->setEndValue(items[i]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

        t_role->setCoordinate(t_land->coordinateX(), t_land->coordinateY());
        t_role = nullptr;
    }

    this->scene()->update(scene()->itemsBoundingRect());
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

        auto items = getPath(t_sender, t_target);

        QSequentialAnimationGroup* animationGroup = new QSequentialAnimationGroup();
        for (int i = 1; i < items.size(); i++) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(t_sender, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(items[i - 1]->pos());
            moveAnimation->setEndValue(items[i]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        for (int i = items.size() - 1; i >= 1; i--) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(t_sender, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(items[i]->pos());
            moveAnimation->setEndValue(items[i - 1]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

        t_target->settleLifeLoss(t_sender->damage());
        return;
    }
}

/**
 * a function used to find the path in the animation
 */
void getPathBFS(int x, int y, QVector<QVector<coordinateStatus>>& t_map, QVector<QVector<std::pair<int, int>>>& t_result,
                QVector<QVector<bool>>& vis) {

    int dx[] = { 1, 0, -1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    QQueue<std::pair<int, int>> que;
    que.push_front(std::make_pair(x, y));
    vis[x][y] = 1;

    while (!que.empty()) {
        auto p = que.front();
        que.pop_front();
        int posx = p.first;
        int posy = p.second;
        for (int i = 0; i < 4; i++) {
            int nx = posx + dx[i];
            int ny = posy + dy[i];

            if (nx < 0 || ny < 0 || nx >= t_map.size() || ny >= t_map[0].size())
                continue;

            if ((vis[nx][ny] == 0) && (t_map[nx][ny] == reachable)) {
                t_result[nx][ny] = std::make_pair(posx, posy);
                vis[nx][ny]      = 1;
                que.push_back(std::make_pair(nx, ny));
            }
        }
    }
}

/**
 * BattlefieldView
 * find the path from start to t_end
 * @param  {GraphUnit*} t_start :  origin
 * @param  {GraphUnit*} t_end   :  destination
 * @return {QList<GraphUnit>*}  : the path got by BFS
 */
QList<GraphUnit*> BattlefieldView::getPath(GraphUnit* t_start, GraphUnit* t_end) {
    updateMapStatus();
    QVector<QVector<std::pair<int, int>>> result;
    result.resize(m_mapwidth);
    for (auto& item : result)
        item.resize(m_mapheight);

    QVector<QVector<bool>> vis;
    vis.resize(m_mapwidth);
    for (auto& item : vis) {
        item.resize(m_mapheight);
        std::fill(item.begin(), item.end(), 0);
    }

    auto temp = m_mapStatus[t_end->coordinateX()][t_end->coordinateY()];

    m_mapStatus[t_end->coordinateX()][t_end->coordinateY()] = reachable;
    getPathBFS(t_start->coordinateX(), t_start->coordinateY(), m_mapStatus, result, vis);
    m_mapStatus[t_end->coordinateX()][t_end->coordinateY()] = temp;

    int nowx, nowy;
    nowx = t_end->coordinateX();
    nowy = t_end->coordinateY();

    QList<GraphUnit*> answer;
    answer.push_front(t_end);
    while (nowx != t_start->coordinateX() || nowy != t_start->coordinateY()) {
        if (vis[nowx][nowy] == 0)
            break;
        auto p = result[nowx][nowy];
        nowx   = p.first;
        nowy   = p.second;
        answer.push_front(m_topUnit[nowx][nowy]);
    }

    return answer;
}

void BattlefieldView::initalizeRound(coordinateStatus t_team) {
    m_roundNumber += 1;
    m_activeTeam = t_team;

    auto items = scene()->items();
    for (auto unit : items) {
        auto role = dynamic_cast<Role*>(unit);
        if (role != nullptr) {
            if (role->teamID() == t_team) {
                role->setroundFinished(false);
            }
            else {
                role->setroundFinished(true);
            }
        }
    }

    emit roundStatudChanged(roundStatus{ m_roundNumber, m_activeTeam });
}

void BattlefieldView::nextRound() {
    if (m_activeTeam == teamOne) {
        initalizeRound(teamTwo);
    } else if (m_activeTeam == teamTwo) {
        initalizeRound(teamOne);
    }
}

