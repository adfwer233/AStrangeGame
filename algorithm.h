#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "statusConstant.h"
#include <QVector>
#include <QList>
#include <QPair>

class Algorithm {
public:
    Algorithm();

    static void findPathBFS(const QVector<QVector<coordinateStatus>>& t_map, QVector<QVector<actionStatus>>& actionResult,
                            int t_maxMoveLength, int t_maxAttackLength, const std::pair<int, int> t_start);

    static void findPathBFS(const QVector<QVector<coordinateStatus>>& t_map, QVector<QVector<actionStatus>>& actionResult,
                            QVector<QVector<std::pair<int, int>>>& t_prev, int t_maxMoveLength, int t_maxAttackLength,
                            const std::pair<int, int> t_start);

    static QList<QPair<int, int>> getPathList(const QVector<QVector<QPair<int, int>>> t_prev, QPair<int, int> t_start, QPair<int, int> t_end);
};

#endif  // ALGORITHM_H
