#ifndef COLONNA_H
#define COLONNA_H

#include "Defs.h"
#include <QStringList>

class Colonna
{
private:
    quint8 _colonna[_COL_SIZE];

public:
    Colonna(quint8 values[_COL_SIZE]);

    quint16 getSum();
    QStringList view();
};

#endif // COLONNA_H
