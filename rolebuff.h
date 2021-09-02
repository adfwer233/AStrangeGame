#ifndef ROLEBUFF_H
#define ROLEBUFF_H

#include <QObject>
#include <QString>

class RoleBuff {
public:
    RoleBuff();

    virtual void lifeValueBuff(int&) const {};
    virtual void magicValueBuff(int&) const {};
    virtual void damageBuff(int&) const {};
    virtual void defenseBuff(int&) const {};

    QString description();

private:
    QString m_description;
};

class redBuff : public RoleBuff {
public:
    virtual void lifeValueBuff(int& value) const override;
};

class blueBuff : public RoleBuff {
public:
    virtual void magicValueBuff(int& value) const override;
};

class fireBuff : public RoleBuff {
public:   
    virtual void damageBuff(int& value) const override;
};

class waterBuff : public RoleBuff {
public:
    virtual void defenseBuff(int& value) const override;
};

#endif