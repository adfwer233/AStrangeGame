#ifndef NORMALLAND_H
#define NORMALLAND_H

#include "graphland.h"

class normalLand : public GraphLand
{
    Q_OBJECT
public:
    normalLand(int t_x, int t_y);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
protected:
    QImage *m_image;
};

#endif // NORMALLAND_H
