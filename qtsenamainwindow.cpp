#include "qtsenamainwindow.h"
#include "sistema.h"
#include "ui_qtsenamainwindow.h"

#include <QTime>
#include <QDialog>
#include <QMessageBox>

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

bool columnEngine(Sistema *sistema, quint16 min, quint16 max, QList<Colonna> *list) {
    quint32 count = 0, valid = 0;
    bool outOfMemory = false;
    while (sistema->ciSonoColonne() && !outOfMemory) {
        Colonna colonna = sistema->prossimaColonna();
        ++count;
        if (colonna.somma() >= min && colonna.somma() <= max) {
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
    numbersGridLayout = new QHBoxLayout(ui->numbersGridBox);
    numbersGridWidget = new GrigliaNumeri();
    numbersGridLayout->addWidget(numbersGridWidget);
    progressDialog = new QProgressDialog();
    ui->actionApri->setEnabled(false);
    ui->actionBiridotto->setEnabled(false);
    ui->actionRidotto->setEnabled(false);
    ui->actionSalva->setEnabled(false);
    connect(numbersGridWidget, SIGNAL(aggiornato(int)), this, SLOT(update()));
    connect(&watcher, SIGNAL(finished()), progressDialog, SLOT(cancel()));
}

QtSEnaMainWindow::~QtSEnaMainWindow()
{
    delete ui;
    delete numbersGridWidget;
    delete numbersGridLayout;
    delete progressDialog;
}

void QtSEnaMainWindow::on_actionIntegrale_triggered() {
    _sistema = new Sistema(numbersGridWidget->listaElementi());
    QTime timer;
    timer.start();

    if (_colList != NULL)
        delete _colList;
    _colList = new QList<Colonna>();
    future = QtConcurrent::run(columnEngine, _sistema, ui->minSum->value(), ui->maxSum->value(), _colList);
    watcher.setFuture(future);

    progressDialog->setWindowTitle("Computazione combinazioni valide...");
    progressDialog->setLabelText(QString("Elaborazione %1 colonne in corso...").arg(numbersGridWidget->numeroColonneSistema()));
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

    QString sMessage = QString("%1 Colonne elaborate in %2h %3m %4s.%5").arg(numbersGridWidget->numeroColonneSistema()).arg(hh).arg(mm).arg(ss).arg(ee);
    sMessage += QString(", %1 selezionate (%2\%)").arg(_colList->count()).arg(((float)10000 * _colList->count())/numbersGridWidget->numeroColonneSistema()/100);
    ui->statusbar->showMessage(sMessage);
}

void QtSEnaMainWindow::update() {
    ValuesList temp = numbersGridWidget->listaElementi();
    int min = 0, max = 0;

    for (int i = 0; i < 6; i ++) {
        min += temp.at(i);
        max += temp.at(temp.size() - i -1);
    }
    ui->minSum->setMinimum(min);
    ui->minSum->setMaximum(max);
    ui->maxSum->setMinimum(min);
    ui->maxSum->setMaximum(max);
    QString sNumCol;
    if(numbersGridWidget->numeroColonneSistema() > 0) {
        sNumCol = QString("Numero Colonne Sistema non condizionato: %1").arg(numbersGridWidget->numeroColonneSistema());
    } else {
        sNumCol = QString("Numero di valori selezionati insufficiente");
    }
    ui->statusbar->showMessage(sNumCol);
}
