#ifndef GRIGLIANUMERI_H
#define GRIGLIANUMERI_H

#include "Defs.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QWidget>


class GrigliaNumeri : public QWidget
{
    Q_OBJECT

private:
    QGridLayout *_layoutGriglia;
    QSignalMapper *_mappaSegnali;
    QVector<QCheckBox *> _griglia;

#ifdef _DEBUG_FLAG_ENABLED
private slots:
    void statoValoreCambiato(int valore);
#endif //_DEBUG_FLAG_ENABLED

public:
    explicit GrigliaNumeri(QWidget *parent = 0);
    ~GrigliaNumeri();

    quint8 numeroElementi();
    quint32 numeroColonneSistema();
    ValuesList listaElementi();

    static QStringList stampa(ValuesList valori);

public slots:
    void deselezionaTutti();
    void selezionaTutti();
    void selezionaValore(quint8 valore);
    void deselezionaValore(quint8 valore);

signals:
    void aggiornato(int value);
};

#endif //GRIGLIANUMERI_H
