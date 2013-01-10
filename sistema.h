#ifndef SISTEMA_H
#define SISTEMA_H

#include "colonna.h"
#include "Defs.h"

class Sistema
{
private:
    ValuesList _sistemaNonCondizionato;
    quint8 _indici[_NUMERO_ELEMENTI_COLONNA];
    bool _ciSonoColonne;

private:
    bool aggiornaIndici();

public:
    Sistema(ValuesList values = ValuesList());
    Colonna prossimaColonna();
    bool ciSonoColonne();
};

#endif // SISTEMA_H
