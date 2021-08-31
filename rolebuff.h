#ifndef ROLEBUFF_H
#define ROLEBUFF_H

#include <QObject>
#include <QString>

class roleBuff {
public:
    roleBuff();

    virtual void lifeValueBuff(int&) const {};
    virtual void magicValueBuff(int&) const {};
    virtual void damageBuff(int&) const {};
    virtual void defenseBuff(int&) const {};

    QString description();

private:
    QString m_description;
};

class redBuff : public roleBuff {
    virtual void lifeValueBuff(int& value) const override;
};

class blueBuff : public roleBuff {
    virtual void magicValueBuff(int& value) const override;
};

class fireBuff : public roleBuff {
    virtual void damageBuff(int& value) const override;
};

class waterBuff : public roleBuff {
    virtual void defenseBuff(int& value) const override;
};

#endif