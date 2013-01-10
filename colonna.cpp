#include "colonna.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

Colonna::Colonna(quint8 values[_COL_SIZE]) {
    for(quint8 i = 0; i < _COL_SIZE; i++) {
        if (values == NULL) {
            _colonna[i] = 0;
    #ifdef _DEBUG_FLAG_ENABLED
            qWarning() << "[COLONNA] - Colonna() - Invalid Pointer";
    #endif //_DEBUG_FLAG_ENABLED
        } else {
            _colonna[i] = values[i];
        }
    }
}

quint16 Colonna::getSum() {
    quint16 res = 0;
    for (int i = 0; i < _COL_SIZE; i++) {
        res += _colonna[i];
    }
    return res;
}

QStringList Colonna::view() {
    QStringList result;
    for (quint8 i = 0; i < _COL_SIZE; i++)
        result << QString("%1").arg(_colonna[i]);
    return result;
}
