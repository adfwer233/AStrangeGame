#include "rolebuff.h"

RoleBuff::RoleBuff()
{
    m_description = QObject::tr("抽象Buff效果");
}

QString RoleBuff::description() {
    return m_description;
}

void redBuff::lifeValueBuff(int& value) const {
    value += 30;
}

void blueBuff::magicValueBuff(int& value) const {
    value += 30;
}

void fireBuff::damageBuff(int& value) const {
    value += 10;
}

void waterBuff::defenseBuff(int& value) const {
    value += 5;
}