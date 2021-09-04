#include "algorithm.h"

#include <QtAlgorithms>
#include <limits>
#include <numeric>
#include <queue>
#include <QDebug>
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
        qFill(distance[i].begin(), distance[i].end(), std::numeric_limits<int>::max());
        qFill(visited[i].begin(), visited[i].end(), 0);
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
        qFill(distance[i].begin(), distance[i].end(), std::numeric_limits<int>::max());
        qFill(visited[i].begin(), visited[i].end(), 0);
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

    int width = 0;
    int height = 0;
    for (auto item : t_scene->items()) {
        auto unit = dynamic_cast<GraphUnit*>(item);
        if (unit != nullptr) {
            width = std::max(width, unit->coordinateX());
            height = std::max(height, unit->coordinateY());
        }
    }

    QVector<QVector<GraphUnit*>> answer;
    answer.resize(width);
    for (auto &x : answer) {
        x.resize(height);
        qFill(x.begin(), x.end(), nullptr);
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