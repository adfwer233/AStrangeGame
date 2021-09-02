#ifndef BATTLEFIELDVIEW_H
#define BATTLEFIELDVIEW_H

#include "graphland.h"
#include "role.h"
#include "statusConstant.h"
#include "roundstatuspanel.h"
#include "fallingobject.h"
#include "infantary.h"
#include "archer.h"
#include <QGraphicsView>
#include <QVector>
#include <QTimer>
class BattlefieldView : public QGraphicsView {
    Q_OBJECT
public:
    explicit BattlefieldView(QWidget* parent = 0);

    virtual void drawBattlefield();
    virtual void resizeEvent(QResizeEvent* event) override;

    QList<GraphUnit*> getPath(GraphUnit* t_start, GraphUnit* t_end);

signals:
    void focusChanged(GraphicUnitInfo);

    void roleChosen(Role*);
    void landChosen(GraphLand*);

    void gameover(coordinateStatus);

    void roundStatudChanged(roundStatus);

public slots:
    void showReachableLands(Role*);
    void closeReachableLands();

    void showAttackableRoles(Role*);
    void closeAttackableRoles();

    void nextRound();
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void addLandItem(GraphLand* t_land, int t_x, int t_y);
    void addRoleItem(Role* t_role, int t_x, int t_y);
    void addBuffItem(FallingObject* t_buff, int t_x, int t_y);
    void updateBattlefield();

    // information about the map
    int  m_mapwidth;
    int  m_mapheight;
    void updateMapStatus();

    QVector<QVector<coordinateStatus>> m_mapStatus;
    QVector<QVector<GraphUnit*>>       m_topUnit;
    QVector<QVector<actionStatus>>     m_actionStatus;

    Role* m_observingRole;
    void  handleMoving(Role* t_role, GraphLand* t_land);


    // information about the round system
    int              m_roundNumber;
    coordinateStatus m_activeTeam;

    void initalizeRound(coordinateStatus t_team);
    bool checkOperability();

    QTimer* m_roleFlashTimer;
};

#endif  // BATTLEFIELDVIEW_H
