#ifndef STATUSCONSTANT_H
#define STATUSCONSTANT_H

#include <QString>

enum actionStatus { attackable, moveable, none };
enum coordinateStatus { reachable, unreachable, teamOne, teamTwo };
enum levelOfAI {basic, advanced};

class Constant {
public:
    static QString panelQSS;
};

#endif // STATUSCONSTANT_H
