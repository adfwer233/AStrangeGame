#ifndef BATTLEFIELDVIEW_H
#define BATTLEFIELDVIEW_H

#include "graphland.h"
#include "statusConstant.h"
#include "roundstatuspanel.h"
#include "fallingobject.h"
#include "archer.h"
#include "algorithm.h"
#include <QGraphicsView>
#include <QVector>
#include <QMap>
#include <QTimer>

class Role;
class Infantary;

class BattlefieldView : public QGraphicsView {
    Q_OBJECT
public:
    explicit BattlefieldView(QWidget* parent = 0);

    virtual void drawBattlefield();
    virtual void resizeEvent(QResizeEvent* event) override;

    QList<GraphUnit*> getPath(GraphUnit* t_start, GraphUnit* t_end);

    friend class Algorithm;

    bool isGameover() const;
    
    levelOfAI AIlevel() const;
    void setAIlevel(levelOfAI);

signals:
    void focusChanged(GraphicUnitInfo);

    void roleChosen(Role*);
    void landChosen(GraphLand*);

    void gameover(coordinateStatus);

    void roundStatudChanged(roundStatus);

    // signals about the AI 
    void AIstart();
    void AIend();
    
public slots:
    void showReachableLands(Role*);
    void closeReachableLands();

    void showAttackableRoles(Role*);
    void closeAttackableRoles();

    void nextRound();

    void roleReleaseSkill(Role* t_role, RoleSkill* t_skill);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void addLandItem(GraphLand* t_land, int t_x, int t_y);
    void addRoleItem(Role* t_role, int t_x, int t_y);
    void addBuffItem(FallingObject* t_buff, int t_x, int t_y);
    void updateBattlefield();

    int memberCount(coordinateStatus t_team);
    void actionFinished();

    // information about the map
    int  m_mapwidth;
    int  m_mapheight;
    void updateMapStatus();

    // map status arrays
    QVector<QVector<coordinateStatus>> m_mapStatus;
    QVector<QVector<GraphUnit*>>       m_topUnit;
    QVector<QVector<actionStatus>>     m_actionStatus;

    // handle the moving system
    Role* m_observingRole;
    bool  handleMoving(Role* t_role, GraphLand* t_land);


    // information about the round system
    int              m_roundNumber;
    coordinateStatus m_activeTeam;

    void initalizeRound(coordinateStatus t_team);
    bool checkOperability();

    // actionPoint system
    int m_maxActionPoint;
    int m_actionPoint;
    
    QList<QPair<int, int>> m_RandomBuffPos;

    QTimer* m_roleFlashTimer;

    bool m_isGameover;

    // AI level system
    levelOfAI m_AILevel;

    void updateShadow();
};

#endif  // BATTLEFIELDVIEW_H
