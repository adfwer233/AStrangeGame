#ifndef FALLINGOBJECT_H
#define FALLINGOBJECT_H

#include "graphland.h"
#include "rolebuff.h"
#include <QImage>
#include <QDebug>
class FallingObject : public GraphLand {
    Q_OBJECT
public:
    FallingObject(int t_x, int t_y);
    RoleBuff*    getBuff() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

protected:
    QImage*   m_image;
    RoleBuff* m_buff;
};

class FallingRedBuff : public FallingObject {
public:
    FallingRedBuff(int t_x, int t_y) : FallingObject(t_x, t_y) {
        m_buff  = new redBuff();
        m_image = new QImage(":/new/roles/src/redbuff.png");
    }
};

class FallingBuleBuff : public FallingObject {
public:
    FallingBuleBuff(int t_x, int t_y) : FallingObject(t_x, t_y) {
        m_buff  = new blueBuff();
        m_image = new QImage(":/new/roles/src/bluebuff.png");
    }
};

class FallingFireBuff : public FallingObject {
public:
    FallingFireBuff(int t_x, int t_y) : FallingObject(t_x, t_y) {
        m_buff  = new fireBuff();
        m_image = new QImage(":/new/roles/src/firebuff.png");
    }
};

class FallingWaterBuff : public FallingObject {
public:
    FallingWaterBuff(int t_x, int t_y) : FallingObject(t_x, t_y) {
        m_buff  = new waterBuff();
        m_image = new QImage(":/new/roles/src/waterbuff.png");
    }
};

#endif  // FALLINGOBJECT_H
