#ifndef GRAPHLAND_H
#define GRAPHLAND_H

#include "graphunit.h"
#include "role.h"
class GraphLand : public GraphUnit {

    Q_OBJECT
public:
    GraphLand(int t_x, int t_y);
    bool         isShowingReachability() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    
    void setShowingRechability(bool);

protected:
    bool  m_isShowingReachability;

signals:

};

#endif  // GRAPHLAND_H
