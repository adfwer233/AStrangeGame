#ifndef SHADOW_H
#define SHADOW_H

#include "graphland.h"
#include <QObject>

class ShadowLand : public GraphLand
{
public:
    ShadowLand(int t_x, int t_y);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
};

#endif // SHADOW_H
