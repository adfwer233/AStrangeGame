#ifndef GAMELEVELTWO_H
#define GAMELEVELTWO_H

#include "battlefieldview.h"

class GameLevelTwo : public BattlefieldView {
    Q_OBJECT
public:
    explicit GameLevelTwo(QWidget* parent = nullptr) : BattlefieldView(parent) {}

    virtual void drawBattlefield() override;
};

#endif  // GAMELEVELTWO_H
