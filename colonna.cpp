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
