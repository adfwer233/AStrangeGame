#ifndef ROLESKILL_H
#define ROLESKILL_H

#include "role.h"
#include <QList>
#include <QObject>
#include <QString>

class RoleSkill : public QObject {
    Q_OBJECT
public:
    explicit RoleSkill(QObject* parent = nullptr);

signals:

private:
    QString m_skillName;
    QString m_skillInformation;

};

#endif  // ROLESKILL_H
