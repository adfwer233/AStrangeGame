#include "gameleveltwo.h"
#include "archer.h"
#include "infantary.h"
#include "normalland.h"
#include "shield.h"
#include "obstacle.h"
#include "statusConstant.h"
void GameLevelTwo::drawBattlefield() {
    /**
     * @brief the beginning map for Game level one
     * 0 means normalland
     * 1 means Obstacle
     * 2 means random buff state
     *
     * 9 means archer of team one
     * 8 means shield of team one
     * 7 means infantary of team one
     *
     * 6 means archer of team two
     * 5 means shield of team two
     * 4 means infantary of team two
     */
    QVector<QVector<int>> pre = {
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4, 0, 6, 4, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4, 0, 6, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4, 0, 6},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 4, 0},
        {0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 5, 0, 4},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 5},
        {0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0},
        {8, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 8, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {7, 0, 8, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0},
        {0, 7, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {9, 0, 7, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 9, 0, 7, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 9, 0, 7, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    };

    m_mapwidth  = 20;
    m_mapheight = 15;

    // add the land to the battlefield
    for (int i = 0; i < m_mapwidth; i++) {
        for (int j = 0; j < m_mapheight; j++) {
            if (pre[j][i] == 0) {
                normalLand* normal = new normalLand(i, j);
                addLandItem(normal, i, j);
            }
            else if (pre[j][i] == 1) {
                Obstacle* obstacle = new Obstacle(i, j);
                addLandItem(obstacle, i, j);
            }
            else {
                normalLand* normal = new normalLand(i, j);
                addLandItem(normal, i, j);
            }

            if (pre[j][i] == 2) {
                m_RandomBuffPos.push_back(qMakePair(i, j));
            }
        }
    }

    // add the roles to the battlefield
    for (int i = 0; i < m_mapwidth; i++) {
        for (int j = 0; j < m_mapheight; j++) {
            Role* role = nullptr;
            switch (pre[j][i]) {
            case 9:
                role = new Archer(i, j, 0);
                break;
            case 8:
                role = new Shield(i, j, 0);
                break;
            case 7:
                role = new Infantary(i, j, 0);
                break;
            case 6:
                role = new Archer(i, j, 1);
                break;
            case 5:
                role = new Shield(i, j, 1);
                break;
            case 4:
                role = new Infantary(i, j, 1);
                break;
            default:
                break;
            }
            if (role != nullptr)
                addRoleItem(role, i, j);
        }
    }
    m_mapStatus.resize(m_mapwidth);
    m_actionStatus.resize(m_mapwidth);
    for (int i = 0; i < m_mapwidth; i++) {
        m_mapStatus[i].resize(m_mapheight);
        m_actionStatus[i].resize(m_mapheight);
    }
    m_topUnit = Algorithm::getTopItem(this->scene());
    connect(m_roleFlashTimer, &QTimer::timeout, this, [&] { this->scene()->update(scene()->itemsBoundingRect()); });
    m_roleFlashTimer->start(100);
    initalizeRound(teamOne);

    updateShadow();
}
