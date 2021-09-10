#include "algorithm.h"
#include "battlefieldview.h"
#include "lifebar.h"
#include "role.h"
#include "normalland.h"
#include <QDebug>
#include <QObject>
#include <QPropertyAnimation>
#include <QtAlgorithms>
#include <limits>
#include <numeric>
#include <queue>
Algorithm::Algorithm() {}

void Algorithm::findPathBFS(const QVector<QVector<coordinateStatus>>& t_map, QVector<QVector<actionStatus>>& t_actionResult,
                            QVector<QVector<std::pair<int, int>>>& t_prev, int t_maxMoveLength, int t_maxAttackLength,
                            const std::pair<int, int> t_start) {

    std::queue<std::pair<int, int>> que;
    QVector<QVector<int>>           distance(t_map.size());
    QVector<QVector<int>>           visited(t_map.size());

    for (int i = 0; i < t_map.size(); i++) {
        distance[i].resize(t_map[i].size());
        visited[i].resize(t_map[i].size());
        std::fill(distance[i].begin(), distance[i].end(), std::numeric_limits<int>::max());
        std::fill(visited[i].begin(), visited[i].end(), 0);
    }

    que.push(t_start);
    visited[t_start.first][t_start.second]  = 1;
    distance[t_start.first][t_start.second] = 0;

    int dx[] = { 1, -1, 0, 0 };
    int dy[] = { 0, 0, 1, -1 };

    while (!que.empty()) {
        auto p = que.front();
        que.pop();
        const int x = p.first;
        const int y = p.second;

        if (t_map[x][y] == teamOne || t_map[x][y] == teamTwo)
            continue;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if ((t_map[x][y] == teamOne || t_map[x][y] == teamTwo) && (x != t_start.first || y != t_start.second))
                continue;

            if (visited[nx][ny] == 0 && t_map[nx][ny] != unreachable) {
                visited[nx][ny]  = 1;
                distance[nx][ny] = distance[x][y] + 1;
                t_prev[nx][ny]   = std::make_pair(x, y);
                que.push(std::make_pair(nx, ny));
            }
        }
    }

    for (int i = 0; i < t_map.size(); i++) {
        for (int j = 0; j < t_map[i].size(); j++) {
            t_actionResult[i][j] = none;
            if (distance[i][j] <= t_maxMoveLength && t_map[i][j] == reachable) {
                t_actionResult[i][j] = moveable;
            }

            if (distance[i][j] <= t_maxAttackLength) {
                if ((t_map[i][j] == teamOne || t_map[i][j] == teamTwo) && t_map[i][j] != t_map[t_start.first][t_start.second])
                    t_actionResult[i][j] = attackable;
            }
        }
    }
}

void Algorithm::findPathBFS(const QVector<QVector<coordinateStatus>>& t_map, QVector<QVector<actionStatus>>& t_actionResult, int t_maxMoveLength,
                            int t_maxAttackLength, const std::pair<int, int> t_start) {
    std::queue<std::pair<int, int>> que;
    QVector<QVector<int>>           distance(t_map.size());
    QVector<QVector<int>>           visited(t_map.size());

    for (int i = 0; i < t_map.size(); i++) {
        distance[i].resize(t_map[i].size());
        visited[i].resize(t_map[i].size());
        std::fill(distance[i].begin(), distance[i].end(), std::numeric_limits<int>::max());
        std::fill(visited[i].begin(), visited[i].end(), 0);
    }

    que.push(t_start);
    visited[t_start.first][t_start.second]  = 1;
    distance[t_start.first][t_start.second] = 0;

    int dx[] = { 1, -1, 0, 0 };
    int dy[] = { 0, 0, 1, -1 };

    while (!que.empty()) {
        auto p = que.front();
        que.pop();
        const int x = p.first;
        const int y = p.second;

        if ((t_map[x][y] == teamOne || t_map[x][y] == teamTwo) && (x != t_start.first || y != t_start.second))
            continue;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || ny < 0 || nx >= t_map.size() || ny >= t_map[nx].size())
                continue;

            if (visited[nx][ny] == 0 && t_map[nx][ny] != unreachable) {
                visited[nx][ny]  = 1;
                distance[nx][ny] = distance[x][y] + 1;
                que.push(std::make_pair(nx, ny));
            }
        }
    }

    for (int i = 0; i < t_map.size(); i++) {
        for (int j = 0; j < t_map[i].size(); j++) {
            t_actionResult[i][j] = none;
            if (distance[i][j] <= t_maxMoveLength && t_map[i][j] == reachable) {
                t_actionResult[i][j] = moveable;
            }

            if (distance[i][j] <= t_maxAttackLength) {
                if ((t_map[i][j] == teamOne || t_map[i][j] == teamTwo) && t_map[i][j] != t_map[t_start.first][t_start.second])
                    t_actionResult[i][j] = attackable;
            }
        }
    }
}

QList<QPair<int, int>> getPathList(const QVector<QVector<QPair<int, int>>> t_prev, QPair<int, int> t_start, QPair<int, int> t_end) {

    QList<QPair<int, int>> result;

    const int ex = t_end.first;
    const int ey = t_end.second;
    const int sx = t_start.first;
    const int sy = t_start.second;

    int posx = ex;
    int posy = ey;

    while (posx != sx || posy != sy) {
        result.push_front(qMakePair(posx, posy));
        auto p = t_prev[posx][posy];
        posx   = p.first;
        posy   = p.second;
    }

    return result;
}

QVector<QVector<GraphUnit*>> Algorithm::getTopItem(QGraphicsScene* t_scene) {

    if (t_scene == nullptr) {
        throw "nullptr scene in the algorithm getTopItem";
    }

    int width  = 0;
    int height = 0;
    for (auto item : t_scene->items()) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr) {
            width  = std::max(width, unit->coordinateX());
            height = std::max(height, unit->coordinateY());
        }
    }

    QVector<QVector<GraphUnit*>> answer;
    answer.resize(width + 1);
    for (auto& x : answer) {
        x.resize(height + 1);
    }

    for (auto item : t_scene->items()) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr) {
            int posx = unit->coordinateX();
            int posy = unit->coordinateY();
            if (answer[posx][posy] == nullptr) {
                answer[posx][posy] = unit;
                continue;
            }
            else if (unit->zValue() > answer[posx][posy]->zValue()) {
                answer[posx][posy] = unit;
            }
        }
    }

    return answer;
}

void Algorithm::LifeChangeAnimation(Role* t_role, int t_delta) {

    int endValue = std::max(0, t_role->lifeValue() + t_delta);
    endValue     = std::min(endValue, t_role->fullLifeValue());

    LifeChangeAnimation(t_role, t_role->lifeValue(), endValue);
}

void Algorithm::LifeChangeAnimation(Role* t_role, int from, int to) {

    LifeBar* lifeBar = new LifeBar();
    lifeBar->setMaxValue(t_role->fullLifeValue());
    lifeBar->setValue(t_role->lifeValue());
    lifeBar->setPos(t_role->pos());
    lifeBar->setZValue(1.1);
    t_role->scene()->addItem(lifeBar);

    QPropertyAnimation* animaton = new QPropertyAnimation(lifeBar, "value");
    animaton->setStartValue(from);
    animaton->setEndValue(to);
    animaton->setDuration(500);
    animaton->start(QPropertyAnimation::DeleteWhenStopped);

    auto scene = t_role->scene();

    QObject::connect(animaton, &QPropertyAnimation::finished, [=] {
        scene->removeItem(lifeBar);
        scene->update();
    });
}

/**
 * Algorithm
 * Basic AI algorithm , no skill
 * @param  {Role*} t_role            : the role will be controled
 * @param  {BattlefieldView*} t_view : BattlefieldView
 */
void Algorithm::basicAI(Role* t_role, BattlefieldView* t_view) {

    if (t_view->isGameover())
        return;

    if (t_view->m_actionPoint <= 0)
        return;

    QList<Role*> enemyList;
    for (auto item : t_view->scene()->items()) {
        auto role = dynamic_cast<Role*>(item);
        if (role != nullptr && role->teamID() != t_role->teamID() && t_role->lifeValue() > 0) {
            enemyList.push_back(role);
        }
    }

    auto distance = [=](Role* other) {
        return abs(other->coordinateX() - t_role->coordinateX()) + abs(other->coordinateY() - t_role->coordinateY());
    };

    int   minDistance = 1000;
    Role* target      = nullptr;
    for (auto item : enemyList) {
        if (distance(item) < minDistance) {
            minDistance = distance(item);
            target      = item;
        }
    }

    t_view->updateMapStatus();
    t_role->updateActionStatus(t_view->m_actionStatus, t_view->m_mapStatus);

    /**
     * @brief a part only advanced AI have
     * pick the falling buff firstly
     */
    if (t_view->AIlevel() == advanced) {
        QList<FallingObject*> buffList;
        for (auto item : t_view->scene()->items()) {
            auto unit = dynamic_cast<GraphUnit*>(item);
            if (unit != nullptr && unit->inherits("FallingObject")) {
                buffList.push_back(static_cast<FallingObject*>(unit));
            }
        }

        for (auto buff : buffList) {
            if (t_view->m_actionStatus[buff->coordinateX()][buff->coordinateY()] == moveable){
                for (auto item : t_view->scene()->items()) {
                    auto unit = dynamic_cast<GraphUnit*>(item);
                    if (unit != nullptr && unit->inherits("normalLand"))
                        if (unit->coordinateX() == buff->coordinateX() && unit->coordinateY() == buff->coordinateY()) {
                            auto land = static_cast<normalLand*>(unit);
                            land->setShowingRechability(true);
                            t_view->handleMoving(t_role, land);
                            return;
                        }
                }
            }
        }

        int roleNumber = 0;
        for (auto role : enemyList) {
            if (distance(role) <= 2)
                roleNumber++;
        }

        if (t_role->inherits("Infantary") && roleNumber >= 3) {
            for (auto skill : t_role->skillList()) {
                qDebug() << "Infantary try to release skill";
                t_view->roleReleaseSkill(t_role, skill, true);
            }
        }

    }


    // attack is the first place choice
    if (target != nullptr && t_view->m_actionStatus[target->coordinateX()][target->coordinateY()] == attackable) {
        target->setShowingAttackable(true);
        t_role->handleAttack(target, t_view->getPath(t_role, target));
        t_view->m_actionPoint -= Role::ATTACK_ACTION_POINT;
        return;
    }

    GraphLand* landTarget = nullptr;

    if (target == nullptr) {
        return;
    }

    auto landDistance = [=](GraphLand* t_land) {
        return abs(t_land->coordinateX() - target->coordinateX()) + abs(t_land->coordinateY() - target->coordinateY());
    };

    minDistance = 1000;
    for (auto item : t_view->scene()->items()) {
        GraphLand* land = dynamic_cast<GraphLand*>(item);
        if (land != nullptr && t_view->m_actionStatus[land->coordinateX()][land->coordinateY()] == moveable) {
            if (landDistance(land) < minDistance) {
                minDistance = landDistance(land);
                landTarget  = land;
            }
        }
    }

    if (landTarget != nullptr) {
        landTarget->setShowingRechability(true);
        t_view->handleMoving(t_role, landTarget);
        t_view->m_actionPoint -= Role::MOVE_ACTION_POINT;
        return;
    }

    emit t_role->actionFinished();
}

void Algorithm::AIcontrol(BattlefieldView* t_view, levelOfAI t_level) {

    emit t_view->AIstart();

    const coordinateStatus ctrlTeam = teamTwo;

    QList<Role*> roleList;

    if (t_view->isGameover())
        return;

    for (auto item : t_view->scene()->items()) {
        auto role = dynamic_cast<Role*>(item);
        if (role != nullptr && role->teamID() == ctrlTeam) {

            /**
             * @brief reverse the list when advanced AI controling
             * more roles can be moved in larger area
             */
            if (t_level == basic)
                roleList.push_back(role);
            else
                roleList.push_front(role);
        }
    }

    if (roleList.size() == 0)
        return;

    for (int i = 0; i < roleList.size() - 1; i++) {
        QObject::connect(roleList[i], &Role::actionFinished, [=] { roleList[i + 1]->AIaction(t_view); });
    }

    roleList[0]->AIaction(t_view);
    QObject::connect(roleList[roleList.size() - 1], &Role::actionFinished, [=] { 
        t_view->nextRound(); 
        t_view->closeAttackableRoles();
        t_view->closeReachableLands();

        for (auto item : roleList) {
            QObject::disconnect(item, &Role::actionFinished, nullptr, nullptr);
        }

        emit t_view->AIend();
    });


}

void Algorithm::removeDeath(Role* t_role) {
    auto scene = t_role->scene();
    scene->removeItem(t_role);
    t_role->hide();
    scene->update();
}
