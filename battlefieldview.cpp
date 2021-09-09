#include "battlefieldview.h"

#include "graphland.h"
#include "graphunit.h"
#include "normalland.h"
#include "obstacle.h"
#include "shadow.h"
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QQueue>
#include <QSequentialAnimationGroup>

BattlefieldView::BattlefieldView(QWidget* parent) : QGraphicsView(parent) {
    m_roundNumber    = 0;
    m_observingRole  = nullptr;
    m_isGameover     = false;
    m_roleFlashTimer = new QTimer(this);
}

void BattlefieldView::mousePressEvent(QMouseEvent* event) {
    QPoint         viewPos  = event->pos();
    QPointF        scenePos = mapToScene(viewPos);
    QGraphicsItem* item     = scene()->itemAt(scenePos, QTransform());

    GraphUnit* graphUnit = dynamic_cast<GraphUnit*>(item);

    // there is a role observing
    if (graphUnit != nullptr && m_observingRole != nullptr) {
        if (graphUnit->inherits("normalLand")) {
            GraphLand* graphland = static_cast<GraphLand*>(graphUnit);

            bool tmp = m_actionPoint >= Role::MOVE_ACTION_POINT;
            if (tmp && handleMoving(m_observingRole, graphland)) {
                m_actionPoint -= Role::MOVE_ACTION_POINT;
                actionFinished();
                m_observingRole->setroundFinished(true);
            }
        }

        if (graphUnit->inherits("Role")) {
            Role* role = static_cast<Role*>(graphUnit);

            bool tmp = m_actionPoint >= role->ATTACK_ACTION_POINT;
            if (tmp && m_observingRole->handleAttack(role, getPath(m_observingRole, role))) {
                m_actionPoint -= Role::ATTACK_ACTION_POINT;
                actionFinished();
                m_observingRole->setroundFinished(true);
            }
        }

        if (graphUnit->inherits("FallingObject")) {
            FallingObject* fallingBuff = static_cast<FallingObject*>(graphUnit);

            bool tmp = m_actionPoint >= Role::MOVE_ACTION_POINT;
            if (tmp && handleMoving(m_observingRole, fallingBuff)) {
                // m_observingRole->addBuff(fallingBuff->getBuff());
                m_actionPoint -= Role::MOVE_ACTION_POINT;
                actionFinished();
                m_observingRole->setroundFinished(true);
            }
        }

        m_observingRole->clearFocus();
        scene()->update(m_observingRole->boundingRect());
        scene()->update(graphUnit->boundingRect());

        m_observingRole = nullptr;
        closeAttackableRoles();
        closeReachableLands();

        if (checkOperability() == false) {
            nextRound();
        }

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
            if (t_role->lifeValue() > 0) {
                emit roleChosen(t_role);
                if (t_role->isroundFinished() == false) {
                    showReachableLands(t_role);
                    showAttackableRoles(t_role);
                }
            }
        }
    }
    // QGraphicsView::mousePressEvent(event);
}

void BattlefieldView::resizeEvent(QResizeEvent*) {
    QRectF boundary = scene()->itemsBoundingRect();
    this->fitInView(boundary, Qt::KeepAspectRatio);
}

void BattlefieldView::addLandItem(GraphLand* t_land, int t_x, int t_y) {
    t_land->setPos(t_x * GraphUnit::INIT_SIZE, t_y * GraphUnit::INIT_SIZE);
    scene()->addItem(t_land);
}

void BattlefieldView::addBuffItem(FallingObject* t_buff, int t_x, int t_y) {
    t_buff->setPos(t_x * GraphUnit::INIT_SIZE, t_y * GraphUnit::INIT_SIZE);
    t_buff->setZValue(0.5);
    scene()->addItem(t_buff);
}

void BattlefieldView::addRoleItem(Role* t_role, int t_x, int t_y) {
    t_role->setPos(t_x * GraphUnit::INIT_SIZE, t_y * GraphUnit::INIT_SIZE);
    t_role->setZValue(1);
    scene()->addItem(t_role);

    connect(t_role, &Role::roleStatueChanged, this, [=] { emit roleChosen(t_role); });
    connect(t_role, &Role::roleStatueChanged, this, &BattlefieldView::updateBattlefield);
    connect(t_role, &Role::deathProcess, [=] { Algorithm::removeDeath(t_role); });
}

void BattlefieldView::drawBattlefield() {

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 15; j++) {
            GraphLand* newItem;
            if ((i * j) % 5 == 1)
                newItem = new Obstacle(i, j);
            else {
                newItem = new normalLand(i, j);
            }

            addLandItem(newItem, i, j);

            if (i == 0) {
                Role* roleItem = new Archer(i, j, j % 2);
                addRoleItem(roleItem, i, j);
            }

            if ((i * j) % 5 == 2) {
                FallingObject* buff = new FallingRedBuff(i, j);
                addBuffItem(buff, i, j);
            }
        }
    }

    // ininialize the related status
    m_mapwidth  = 20;
    m_mapheight = 15;
    m_mapStatus.resize(m_mapwidth);
    for (auto& x : m_mapStatus)
        x.resize(m_mapheight);

    m_actionStatus.resize(m_mapwidth);
    for (auto& x : m_actionStatus)
        x.resize(m_mapheight);

    m_topUnit.resize(m_mapwidth);
    for (auto& x : m_topUnit)
        x.resize(m_mapheight);

    connect(m_roleFlashTimer, &QTimer::timeout, this, [&] { this->scene()->update(scene()->itemsBoundingRect()); });
    m_roleFlashTimer->start(100);
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
 * when one team has no role living, emit the game over slot
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

    if (teamoneNum == 0 || teamtwoNum == 0)
        m_isGameover = true;
}

/**
 * 人物移动的动画与结算
 *
 * @param  {Role*} t_role      : 想要移动的角色指针
 * @param  {GraphLand*} t_land : 目标地点
 */
bool BattlefieldView::handleMoving(Role* t_role, GraphLand* t_land) {
    if (t_land->isShowingReachability() == true && t_role != nullptr) {

        auto items = getPath(t_role, t_land);

        QSequentialAnimationGroup* animationGroup = new QSequentialAnimationGroup();
        for (int i = 1; i < items.size(); i++) {
            QPropertyAnimation* moveAnimation = new QPropertyAnimation(t_role, "pos");
            moveAnimation->setDuration(100);
            moveAnimation->setStartValue(items[i - 1]->pos());
            moveAnimation->setEndValue(items[i]->pos());
            animationGroup->addAnimation(moveAnimation);
        }

        QObject::connect(animationGroup, &QSequentialAnimationGroup::finished, t_role, &Role::actionFinished);
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

        if (t_land->inherits("FallingObject")) {
            auto buff = static_cast<FallingObject*>(t_land);
            t_role->addBuff(buff->getBuff());
            scene()->removeItem(t_land);
        }

        t_role->setroundFinished(true);
        t_role->setPos(this->pos());
        t_role->setCoordinate(t_land->coordinateX(), t_land->coordinateY());
        t_role = nullptr;

        updateShadow();
        this->scene()->update(scene()->itemsBoundingRect());
        return true;
    }

    return false;
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

    m_maxActionPoint = memberCount(t_team) + 5;
    m_actionPoint    = m_maxActionPoint;

    emit roundStatudChanged(roundStatus{ m_roundNumber, m_activeTeam, m_maxActionPoint, m_actionPoint });
}

void BattlefieldView::nextRound() {
    if (m_activeTeam == teamOne) {
        initalizeRound(teamTwo);
    }
    else if (m_activeTeam == teamTwo) {
        initalizeRound(teamOne);
    }

    auto         items = scene()->items();
    QList<Role*> roleList;
    for (auto unit : items) {
        auto role = dynamic_cast<Role*>(unit);
        if (role != nullptr)
            roleList.push_back(role);
    }

    for (auto role : roleList) {
        if (role->lifeValue() >= 0) {
            role->selfHeat();
            role->settleBuff();
        }
    }

    for (auto item : items) {
        auto unit = static_cast<GraphUnit*>(item);
        if (unit->inherits("FallingObject")) {
            auto buff = static_cast<FallingObject*>(unit);
            for (auto role : roleList) {
                if (role->coordinateX() == buff->coordinateX() && role->coordinateY() == buff->coordinateY()) {
                    role->addBuff(buff->getBuff());
                    scene()->removeItem(buff);
                }
            }
        }
    }

    if (m_activeTeam == teamTwo) {
        Algorithm::AIcontrol(this, AIlevel());
    }

    if (m_roundNumber % 6 == 0) {
        for (auto item : m_RandomBuffPos) {
            bool flag = 0;
            for (auto unit : scene()->items()) {
                auto x = static_cast<GraphUnit*>(unit);
                if (x->inherits("FallingObject") && x->coordinateX() == item.first && x->coordinateY() == item.second)
                    flag = 1;
            }
            if (flag)
                continue;

            int            x = rand() % 4;
            FallingObject* buff;
            if (x == 0)
                buff = new FallingRedBuff(item.first, item.second);
            else if (x == 1)
                buff = new FallingBuleBuff(item.first, item.second);
            else if (x == 2)
                buff = new FallingFireBuff(item.first, item.second);
            else if (x == 3)
                buff = new FallingWaterBuff(item.first, item.second);
            addBuffItem(buff, item.first, item.second);
        }
    }
}

bool BattlefieldView::checkOperability() {
    return m_actionPoint > 0;
}

int BattlefieldView::memberCount(coordinateStatus t_team) {
    if (t_team != teamOne && t_team != teamTwo)
        throw "invalid teamID";

    if (scene() == nullptr) {
        qDebug() << "running";
        throw "no scene";
    }

    int result = 0;
    for (auto item : scene()->items()) {
        auto role = dynamic_cast<Role*>(item);
        if (role != nullptr) {
            if (role->teamID() == t_team)
                result += 1;
        }
    }

    return result;
}

void BattlefieldView::actionFinished() {
    emit roundStatudChanged(roundStatus{ m_roundNumber, m_activeTeam, m_maxActionPoint, m_actionPoint });
}

void BattlefieldView::roleReleaseSkill(Role* t_role, RoleSkill* t_skill, bool isAI) {
    if (t_skill->actionPointCost() > this->m_actionPoint) {
        QMessageBox::information(this, "技能施放", "行动点数不足");
        return;
    }
    else {
        if (t_role->releaseSkill(t_skill, isAI)) {
            m_actionPoint -= t_skill->actionPointCost();
        }
        actionFinished();
        if (checkOperability() == false)
            nextRound();
    }
}

bool BattlefieldView::isGameover() const {
    return m_isGameover;
}

levelOfAI BattlefieldView::AIlevel() const {
    return m_AILevel;
}

void BattlefieldView::setAIlevel(levelOfAI t_value) {
    m_AILevel = t_value;
}

void BattlefieldView::updateShadow() {
    QList<Role*> roleList;

    // delete the old shadow
    for (auto item : this->scene()->items()) {
        auto unit = dynamic_cast<ShadowLand*>(item);
        if (unit != nullptr) {
            this->scene()->removeItem(unit);
            unit->hide();
            unit->deleteLater();
        }
    }


    for (auto item : this->scene()->items()) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr && unit->inherits("Role")) {
            auto role = static_cast<Role*>(unit);
            if (role->teamID() == teamOne) {
                roleList.push_back(role);
            }
        }
    }

    QVector<QVector<bool>> vis;
    QVector<QVector<QPointF>> m_pos;
    vis.resize(m_mapwidth);
    for (auto& x : vis) {
        x.resize(m_mapheight);
        std::fill(x.begin(), x.end(), 0);
    }
    
    for (auto role : roleList) {
        for (int i = 0; i < m_mapwidth; i++)
            for (int j = 0; j < m_mapheight; j++) {
                if (abs(i - role->coordinateX()) + abs(j - role->coordinateY()) <= 8)
                    vis[i][j] = 1;
            }
    }

    for (int i = 0; i < m_mapwidth; i++) {
        for (int j = 0; j < m_mapheight; j++) {
            if (vis[i][j] == 0) {
                ShadowLand* shadow = new ShadowLand(i, j);
                addLandItem(shadow, i, j);
                shadow->setZValue(2);
            }
        }
    }
}
