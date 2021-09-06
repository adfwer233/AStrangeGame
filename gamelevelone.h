#ifndef GAMELEVELONE_H
#define GAMELEVELONE_H

#include "battlefieldview.h"

class GameLevelOne : public BattlefieldView
{
public:
    explicit GameLevelOne(QWidget* parent = nullptr) : BattlefieldView(parent) {

    }

    virtual void drawBattlefield() override;

private:
};

#endif // GAMELEVELONE_H
