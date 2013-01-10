#include "sistema.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

Sistema::Sistema(ValuesList values) {
    if (values.count() < _COL_SIZE) {
#ifdef _DEBUG_FLAG_ENABLED
        qDebug() << "[SISTEMA] - Sistema() - Invalid Values List";
#endif //_DEBUG_FLAG_ENABLED
        for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
            _sistemaNonCondizionato.append(i+1);
        }
    } else {
        _sistemaNonCondizionato = values;
    }
    for (quint8 i = 0; i < _COL_SIZE; i++) {
        index[i] = i;
    }
    _hasColonna = true;
}

Colonna Sistema::getColonna() {
    quint8 res[_COL_SIZE];
    for (quint8 i = 0; i < _COL_SIZE; i++) {
        res[i] = _sistemaNonCondizionato.at(index[i]);
    }
    if(!updateIndex()) {
#ifdef _DEBUG_FLAG_ENABLED
        qDebug() << "[SISTEMA] - >>() - Non ci sono altre colonne valide";
#endif //_DEBUG_FLAG_ENABLED
        memset(res, 0, _COL_SIZE * sizeof(quint8));
        _hasColonna = false;
    }
    return Colonna(res);
}

bool Sistema::hasColonna() {
    return _hasColonna;
}

bool Sistema::updateIndex() {
    if (++index[5] < _sistemaNonCondizionato.count()) {
        return true;
    }
    if (++index[4] < _sistemaNonCondizionato.count() - 1) {
        index[5] = index[4] + 1;
        return true;
    }
    if (++index[3] < _sistemaNonCondizionato.count() - 2) {
        index[4] = index[3] + 1;
        index[5] = index[4] + 1;
        return true;
    }
    if (++index[2] < _sistemaNonCondizionato.count() - 3) {
        index[3] = index[2] + 1;
        index[4] = index[3] + 1;
        index[5] = index[4] + 1;
        return true;
    }
    if (++index[1] < _sistemaNonCondizionato.count() - 4) {
        index[2] = index[1] + 1;
        index[3] = index[2] + 1;
        index[4] = index[3] + 1;
        index[5] = index[4] + 1;
        return true;
    }
    if (++index[0] <= _sistemaNonCondizionato.count() - 5) {
        index[1] = index[0] + 1;
        index[2] = index[1] + 1;
        index[3] = index[2] + 1;
        index[4] = index[3] + 1;
        index[5] = index[4] + 1;
        return true;
    }
    return false;
}
