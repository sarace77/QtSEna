#include "qtsenamainwindow.h"
#include "sistema.h"
#include "ui_qtsenamainwindow.h"

#include <QDialog>
#include <QMessageBox>
#include <QTime>

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

typedef struct _Condizionamento {
    quint16 min, max;
    QList<bool> listaConsecutivi, listaGemelli, listaPari;
}Condizionamento;

bool columnEngine(QList<Colonna> *list, Sistema *sistema, Condizionamento condizionamento) {//, QList<bool> listaPari) {
    quint32 count = 0, valid = 0;
#ifdef _DEBUG_FLAG_ENABLED
    qDebug() << condizionamento.min << condizionamento.max;
    for(quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
        if(condizionamento.listaConsecutivi.at(i))
            qDebug() << "c:" << i;
        if(condizionamento.listaPari.at(i))
            qDebug() << "p:" << i;
    }
#endif //_DEBUG_FLAG_ENABLED
    bool outOfMemory = false;
    while (sistema->ciSonoColonne() && !outOfMemory) {
        Colonna colonna = sistema->prossimaColonna();
        ++count;
        if ( colonna.somma() >= condizionamento.min && colonna.somma() <= condizionamento.max &&
             condizionamento.listaConsecutivi.at(colonna.consecutivi()) &&
             condizionamento.listaGemelli.at(colonna.gemelli()) &&
             condizionamento.listaPari.at(colonna.pari())) {
            ++valid;
            list->append(colonna);
        }
        outOfMemory = (list->size() >= _MAX_COLONNE);
    }
    return outOfMemory;
}

QtSEnaMainWindow::QtSEnaMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::QtSEnaMainWindow) {
    _colList = NULL;
    ui->setupUi(this);
    _layoutConsecutivi = new QHBoxLayout(ui->consecutiviBox);
    _layoutGemelli = new QHBoxLayout(ui->gemelliBox);
    _layoutGrigliaNumeri= new QHBoxLayout(ui->numbersGridBox);
    _layoutPari = new QHBoxLayout(ui->pariBox);
    for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
        QCheckBox *elementoPari = new QCheckBox();
        QCheckBox *elementoConsecutivi = new QCheckBox();
        QCheckBox *elementoGemelli = new QCheckBox();
        elementoConsecutivi->setChecked(true);
        elementoConsecutivi->setText(QString("%1").arg(i));
        elementoGemelli->setChecked(true);
        elementoGemelli->setText(QString("%1").arg(i));
        elementoPari->setChecked(true);
        elementoPari->setText(QString("%1").arg(i));
        _consecutivi.append(elementoConsecutivi);
        _gemelli.append(elementoGemelli);
        _pari.append(elementoPari);
        _layoutConsecutivi->addWidget(_consecutivi.at(i));
        _layoutGemelli->addWidget(_gemelli.at(i));
        _layoutPari->addWidget(_pari.at(i));
    }
    _grigliaNumeri= new GrigliaNumeri();
    _layoutGrigliaNumeri->addWidget(_grigliaNumeri);
    progressDialog = new QProgressDialog();
    ui->actionApri->setEnabled(false);
    ui->actionBiridotto->setEnabled(false);
    ui->actionPreferenze->setEnabled(false);
    ui->actionRidotto->setEnabled(false);
    ui->actionSalva->setEnabled(false);
    ui->risultatoLabel->setVisible(false);
    connect(_grigliaNumeri, SIGNAL(aggiornato(int)), this, SLOT(aggiorna()));
    connect(&watcher, SIGNAL(finished()), progressDialog, SLOT(cancel()));
    aggiorna();
}

QtSEnaMainWindow::~QtSEnaMainWindow()
{
    delete ui;
    for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
        delete _consecutivi.at(i);
        delete _gemelli.at(i);
        delete _pari.at(i);
    }
    delete _layoutConsecutivi;
    delete _layoutGemelli;
    delete _layoutGrigliaNumeri;
    delete _layoutPari;
    delete progressDialog;
}

void QtSEnaMainWindow::aggiorna() {
    ValuesList temp = _grigliaNumeri->listaElementi();
    int min = 0, max = 0;

    for (int i = 0; i < 6; i ++) {
        min += temp.at(i);
        max += temp.at(temp.size() - i -1);
    }
    ui->minSum->setMinimum(min);
    ui->minSum->setMaximum(max);
    ui->maxSum->setMinimum(min);
    ui->maxSum->setMaximum(max);
    ui->elementiLabel->setText(QString("Elementi: %1").arg(_grigliaNumeri->numeroElementi()));
    QString sColonneSistema = "";
    int MM, mm, uu;
    MM = _grigliaNumeri->numeroColonneSistema() / 1000000;
    mm = (_grigliaNumeri->numeroColonneSistema() - MM * 1000000) / 1000;
    uu = _grigliaNumeri->numeroColonneSistema()  - MM * 1000000 - mm * 1000;
    if (MM > 0)        sColonneSistema += QString("%1,").arg(MM);
    if (mm > 0)        sColonneSistema += QString("%1,").arg(mm, 3, 10, QChar('0'));

    sColonneSistema += QString("%1").arg(uu);
    ui->colonneLabel->setText(QString("Colonne: ") + sColonneSistema);
}

void QtSEnaMainWindow::on_actionIntegrale_triggered() {
    _sistema = new Sistema(_grigliaNumeri->listaElementi());
    QTime timer;
    timer.start();

    if (_colList != NULL)
        delete _colList;
    _colList = new QList<Colonna>();
    Condizionamento condizionamento;
    condizionamento.max = ui->maxSum->value();
    condizionamento.min = ui->minSum->value();
    for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
        condizionamento.listaConsecutivi.append(_consecutivi.at(i)->isChecked());
        condizionamento.listaGemelli.append(_gemelli.at(i)->isChecked());
        condizionamento.listaPari.append(_pari.at(i)->isChecked());
    }

    future = QtConcurrent::run(columnEngine, _colList, _sistema, condizionamento);//, listaPari);
    watcher.setFuture(future);

    progressDialog->setWindowTitle("Computazione combinazioni valide...");
    progressDialog->setLabelText(QString("Elaborazione %1 colonne in corso...").arg(_grigliaNumeri->numeroColonneSistema()));
    progressDialog->setMinimum(0);
    progressDialog->setMaximum(0);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->exec();

    if (future.result()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Errore condizionamento!");
        msgBox.setText("Computate solo le prime 50 milioni di combinazioni valide");
        msgBox.exec();
    }

    quint32 hh, mm, ss, ee;
    hh = timer.elapsed()/3600000;
    mm = (timer.elapsed() - hh *3600000) /60000;
    ss = (timer.elapsed() - hh * 3600000 - mm * 60000) /1000;
    ee = timer.elapsed() - hh * 3600000 - mm * 60000 - ss * 1000;

    QString sMessage = QString("%1 Colonne elaborate in %2h %3m %4s.%5").arg(_grigliaNumeri->numeroColonneSistema()).arg(hh).arg(mm).arg(ss).arg(ee);
    //sMessage += QString(", %1 selezionate (%2\%)").arg(_colList->count()).arg(((float)10000 * _colList->count())/_grigliaNumeri->numeroColonneSistema()/100);
    ui->statusbar->showMessage(sMessage);
    QString sLabel = QString("<B>Colonne Sistema:</B><BR> %1<BR><B>Costo Sistema:</B><BR>%2 euro").arg(_colList->count()).arg(_colList->count()/2);
    ui->risultatoLabel->setVisible(true);
    ui->risultatoLabel->setText(sLabel);
}

