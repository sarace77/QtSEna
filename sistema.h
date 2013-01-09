#ifndef SISTEMA_H
#define SISTEMA_H

#include "colonna.h"
#include "Defs.h"

class Sistema
{
private:
    ValuesList _sistemaNonCondizionato;
    quint8 index[_COL_SIZE];
    bool _hasColonna;

private:
    bool updateIndex();

public:
    Sistema(ValuesList values = ValuesList());
    Colonna getColonna();
    bool hasColonna();
};

#endif // SISTEMA_H
