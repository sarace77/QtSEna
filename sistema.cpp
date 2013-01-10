#include "sistema.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

Sistema::Sistema(ValuesList values) {
    if (values.count() < _NUMERO_ELEMENTI_COLONNA) {
#ifdef _DEBUG_FLAG_ENABLED
        qDebug() << "[SISTEMA] - Sistema() - I Valori passati non sono validi";
#endif //_DEBUG_FLAG_ENABLED
        for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++) {
            _sistemaNonCondizionato.append(i+1);
        }
    } else {
        _sistemaNonCondizionato = values;
    }
    for (quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        _indici[i] = i;
    }
    _ciSonoColonne= true;
}

Colonna Sistema::prossimaColonna() {
    quint8 risultato[_NUMERO_ELEMENTI_COLONNA];
    for (quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        risultato[i] = _sistemaNonCondizionato.at(_indici[i]);
    }
    if(!aggiornaIndici()) {
#ifdef _DEBUG_FLAG_ENABLED
        qDebug() << "[SISTEMA] - >>() - Non ci sono altre colonne valide";
#endif //_DEBUG_FLAG_ENABLED
        memset(risultato, 0, _NUMERO_ELEMENTI_COLONNA * sizeof(quint8));
        _ciSonoColonne = false;
    }
    return Colonna(risultato);
}

bool Sistema::ciSonoColonne() {
    return _ciSonoColonne;
}

bool Sistema::aggiornaIndici() {
    if (++_indici[5] < _sistemaNonCondizionato.count()) {
        return true;
    }
    if (++_indici[4] < _sistemaNonCondizionato.count() - 1) {
        _indici[5] = _indici[4] + 1;
        return true;
    }
    if (++_indici[3] < _sistemaNonCondizionato.count() - 2) {
        _indici[4] = _indici[3] + 1;
        _indici[5] = _indici[4] + 1;
        return true;
    }
    if (++_indici[2] < _sistemaNonCondizionato.count() - 3) {
        _indici[3] = _indici[2] + 1;
        _indici[4] = _indici[3] + 1;
        _indici[5] = _indici[4] + 1;
        return true;
    }
    if (++_indici[1] < _sistemaNonCondizionato.count() - 4) {
        _indici[2] = _indici[1] + 1;
        _indici[3] = _indici[2] + 1;
        _indici[4] = _indici[3] + 1;
        _indici[5] = _indici[4] + 1;
        return true;
    }
    if (++_indici[0] <= _sistemaNonCondizionato.count() - 5) {
        _indici[1] = _indici[0] + 1;
        _indici[2] = _indici[1] + 1;
        _indici[3] = _indici[2] + 1;
        _indici[4] = _indici[3] + 1;
        _indici[5] = _indici[4] + 1;
        return true;
    }
    return false;
}
