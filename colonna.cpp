#include "colonna.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

Colonna::Colonna(quint8 valori[_NUMERO_ELEMENTI_COLONNA]) {
    for(quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        if (valori == NULL) {
            _colonna[i] = 0;
    #ifdef _DEBUG_FLAG_ENABLED
            qWarning() << "[COLONNA] - Colonna() - Puntatore non valido";
    #endif //_DEBUG_FLAG_ENABLED
        } else {
            _colonna[i] = valori[i];
        }
    }
}

quint8 Colonna::consecutivi() {
    quint8 contatore = 0;
    for (quint8 i = 1; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        if (_colonna[i] - _colonna[i-1] == 1)
            ++contatore;
    }
    return contatore;
}

quint8 Colonna::dispari() {
    return _NUMERO_ELEMENTI_COLONNA - pari();
}

QList<quint8> Colonna::elementi() {
    QList<quint8> valori;
    for(quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        valori.append(_colonna[i]);
    }
    return valori;
}

quint8 Colonna::gemelli() {
    quint8 contatore = 0;
    for(quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        if((_colonna[i] / 11) * 11 == _colonna[i])
            ++contatore;
    }
    return contatore;
}

quint8 Colonna::pari() {
    quint8 contatore = 0;
    for (quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        if ((_colonna[i] / 2) * 2 == _colonna[i])
            ++contatore;
    }
    return contatore;
}

quint16 Colonna::somma() {
    quint16 risultato = 0;
    for (int i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        risultato += _colonna[i];
    }
    return risultato;
}

QStringList Colonna::stampa() {
    QStringList risultato;
    for (quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++)
        risultato << QString("%1").arg(_colonna[i]);
    return risultato;
}
