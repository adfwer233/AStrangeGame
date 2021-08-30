#ifndef BATTLEFIELDVIEW_H
#define BATTLEFIELDVIEW_H

#include "graphland.h"
#include "role.h"
#include "statusConstant.h"
#include <QGraphicsView>
#include <QVector>

class BattlefieldView : public QGraphicsView {
    Q_OBJECT
public:
    explicit BattlefieldView(QWidget* parent = 0);

    virtual void drawBattlefield();
    virtual void resizeEvent(QResizeEvent* event) override;

signals:
    void focusChanged(GraphicUnitInfo);

    void roleChosen(Role*);
    void landChosen(GraphLand*);

public slots:
    void showReachableLands(Role*);
    void closeReachableLands();

    void showAttackableRoles(Role*);
    void closeAttackableRoles();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void addLandItem(GraphLand* t_land, int t_x, int t_y);
    void addRoleItem(Role* t_role, int t_x, int t_y);
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
    void  handleAttack(Role* t_sender, Role* t_target);

    QList<GraphUnit*> getPath(GraphUnit* t_start, GraphUnit* t_end);
};

#endif  // BATTLEFIELDVIEW_H
