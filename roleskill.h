#ifndef ROLESKILL_H
#define ROLESKILL_H


#include "graphunit.h"
#include "algorithm.h"
#include <QList>
#include <QObject>
#include <QString>
#include <QGraphicsScene>


class Role;

struct SkillInfo {
    QString m_skillname, m_description;
};

class RoleSkill : public QObject {
    Q_OBJECT
public:
    explicit RoleSkill(QObject* parent = nullptr);

    QString skillName() const;
    QString skillDescription() const;

    virtual int actionPointCost() const;
    virtual int magicPointCost() const;

    // the settlement of skills and animations
    virtual void releaseSkill(Role* t_sender, QGraphicsScene* t_scene) = 0; 

signals:

private:
    QString m_skillName;
    QString m_description;

};

class ArrowRain : public RoleSkill {
    Q_OBJECT
public:
    explicit ArrowRain(QObject* parent = nullptr) : RoleSkill(parent) {}

    virtual int actionPointCost() const override {
        return 3;
    }

    virtual int magicPointCost() const override {
        return 35;
    }
    
    virtual void releaseSkill(Role* t_sender, QGraphicsScene* t_scene) override;
};

class Explosion : public RoleSkill {
    Q_OBJECT
public:
    explicit Explosion(QObject* parent = nullptr) : RoleSkill(parent) {}

    virtual int actionPointCost() const override {
        return 5;
    }

    virtual int magicPointCost() const override {
        return 40;
    }

    virtual void releaseSkill(Role* t_sender, QGraphicsScene* t_scene) override;
};

class SwordDance : public RoleSkill {
    Q_OBJECT
public:
    explicit SwordDance(QObject* parent = nullptr) : RoleSkill(parent) {}

    virtual int actionPointCost() const override {
        return 2;
    }

    virtual int magicPointCost() const override {
        return 30;
    }

    virtual void releaseSkill(Role* t_sender, QGraphicsScene* t_scene) override;
};



#endif  // ROLESKILL_H
