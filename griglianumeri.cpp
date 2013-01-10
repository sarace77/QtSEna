#include "griglianumeri.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

GrigliaNumeri::GrigliaNumeri(QWidget *parent) :
    QWidget(parent)
{
    _layoutGriglia = new QGridLayout(this);
    _mappaSegnali = new QSignalMapper(this);
    setStyleSheet("QWidget {background-color:#fff;}");
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++) {
        QCheckBox *elemento = new QCheckBox();
        elemento->setText(QString("%1").arg(i+1));
        connect(elemento, SIGNAL(stateChanged(int)), _mappaSegnali, SLOT(map()));
        _mappaSegnali->setMapping(elemento, elemento->text().toInt());
        _griglia.append(elemento);
        _layoutGriglia->addWidget(_griglia.at(i), i/10, i - (i/10)*10, 1, 1);
    }
    selezionaTutti();
    connect(_mappaSegnali, SIGNAL(mapped(int)), this, SIGNAL(aggiornato(int)));
#ifdef _DEBUG_FLAG_ENABLED
    connect(this, SIGNAL(aggiornato(int)), this, SLOT(statoValoreCambiato(int)));
#endif //_DEBUG_FLAG_ENABLED
}

GrigliaNumeri::~GrigliaNumeri() {
#ifdef _DEBUG_FLAG_ENABLED
    qDebug() << "[GRIGLIANUMERI] - ~GrigliaNumeri() - Valori:" << stampa(listaElementi());
#endif //_DEBUG_FLAG_ENABLED
    delete _layoutGriglia;
    delete _mappaSegnali;
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++) {
        delete _griglia.at(i);
    }
}

quint8 GrigliaNumeri::numeroElementi() {
    quint8 contatore = 0;
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++) {
        if (_griglia.at(i)->isChecked())
            contatore++;
    }
    return contatore;
}

quint32 GrigliaNumeri::numeroColonneSistema() {
    quint32 risultato = 1;
    for (quint8 i = 0; i < _NUMERO_ELEMENTI_COLONNA; i++) {
        risultato *= numeroElementi() - i;
        risultato /= (i + 1);
    }
    return risultato;
}

ValuesList GrigliaNumeri::listaElementi() {
    ValuesList risultato;
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++) {
        if (_griglia.at(i)->isChecked())
            risultato << (i + 1);
    }
    return risultato;
}

#ifdef _DEBUG_FLAG_ENABLED
void GrigliaNumeri::statoValoreCambiato(int value) {
    qDebug() << "[GRIGLIANUMERI] - statoValoreCambiato() - Colonne Sistema: " << numeroColonneSistema() << value << ": " << _griglia.at(value - 1)->isChecked();
}
#endif //_DEBUG_FLAG_ENABLED

void GrigliaNumeri::deselezionaTutti() {
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++)
        deselezionaValore(i);
}

void GrigliaNumeri::selezionaTutti() {
    for (quint8 i = 0; i < _NUMERO_DI_ELEMENTI; i++)
        selezionaValore(i);
}

void GrigliaNumeri::selezionaValore(quint8 valore) {
    _griglia.at(valore)->setChecked(true);
}

void GrigliaNumeri::deselezionaValore(quint8 valore) {
    _griglia.at(valore)->setChecked(false);
}

QStringList GrigliaNumeri::stampa(ValuesList valori) {
    QStringList risultato;
    for (int i = 0; i < valori.count(); i++) {
        risultato << QString("%1").arg(valori.at(i));
    }
    return risultato;
}
