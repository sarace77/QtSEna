#ifndef COLONNA_H
#define COLONNA_H

#include "Defs.h"
#include <QStringList>

class Colonna
{
private:
    quint8 _colonna[_NUMERO_ELEMENTI_COLONNA];

public:
    Colonna(quint8 valori[_NUMERO_ELEMENTI_COLONNA]);

    quint8 consecutivi();
    quint8 dispari();
    QList<quint8> elementi();
    quint8 gemelli();
    quint8 pari();
    quint16 somma();
    QStringList stampa();
};

#endif // COLONNA_H
