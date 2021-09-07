#include "roleskill.h"
#include "normalland.h"
#include "role.h"
#include "flightequipment.h"
#include <QGraphicsScene>
#include <QDebug>

RoleSkill::RoleSkill(QObject *parent) : QObject(parent)
{
    m_skillName = "抽象技能";
    m_description = "无效果";
}

int RoleSkill::actionPointCost() const {
    return 4;
}

int RoleSkill::magicPointCost() const {
    return 50;
}

QString RoleSkill::skillName() const {
    return m_skillName;
}

QString RoleSkill::skillDescription() const {
    return m_description;
}

void ArrowRain::releaseSkill(Role* t_sender, QGraphicsScene* t_scene) {
    auto map = Algorithm::getTopItem(t_scene);

    int posx = t_sender->coordinateX();
    int posy = t_sender->coordinateY();

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (std::max(abs(i - posx), abs(j - posy)) <= 3) {

                if(map[i][j]->inherits("Role")) {
                    auto role = static_cast<Role*>(map[i][j]);
                    if (role->teamID() != t_sender->teamID()) {
                        role->settleLifeLoss(t_sender->damage());
                        FlightEquipmentArrow* arrow = new FlightEquipmentArrow();
                        arrow->animationReact(t_sender->pos(), map[i][j]->pos(), t_scene);
                    }
                }
            }
        }
    }
}

void Explosion::releaseSkill(Role* t_sender, QGraphicsScene* t_scene) {
    auto map = Algorithm::getTopItem(t_scene);

    int posx = t_sender->coordinateX();
    int posy = t_sender->coordinateY();

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (std::max(abs(i - posx), abs(j - posy)) <= 1){
                if(map[i][j]->inherits("Obstacle")) {
                    normalLand* newLand = new normalLand(i, j);
                    newLand->setPos(i * GraphUnit::INIT_SIZE, j * GraphUnit::INIT_SIZE);
                    t_scene->addItem(newLand);
                    t_scene->removeItem(map[i][j]);
                }
            }
        }
    }
}

void SwordDance::releaseSkill(Role* t_sender, QGraphicsScene* t_scene) {
    auto map = Algorithm::getTopItem(t_scene);

    int posx = t_sender->coordinateX();
    int posy = t_sender->coordinateY();

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (std::max(abs(i - posx), abs(j - posy)) <= 1){
                if(map[i][j]->inherits("Role")) {
                    auto role = static_cast<Role*>(map[i][j]);
                    if (role->teamID() != t_sender->teamID()) {
                        role->settleLifeLoss(t_sender->damage() + 15);
                        qDebug() << "sword dance running";
                    }
                }
            }
        }
    }
}