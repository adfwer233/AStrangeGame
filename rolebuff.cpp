#include "rolebuff.h"

roleBuff::roleBuff()
{
    m_description = QObject::tr("抽象Buff效果");
}

QString roleBuff::description() {
    return m_description;
}

