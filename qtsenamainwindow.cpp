#include "qtsenamainwindow.h"
#include "sistema.h"
#include "ui_qtsenamainwindow.h"

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QTime>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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
    _sistema = NULL;

    ui->setupUi(this);
    _apriFile = new QFileDialog(this);
    _g3 = new QRadioButton(this);
    _g4 = new QRadioButton(this);
    _g5 = new QRadioButton(this);
    _grigliaNumeri= new GrigliaNumeri(this);
    _integrale = new QRadioButton(this);
    _layoutConsecutivi = new QHBoxLayout(ui->consecutiviBox);
    _layoutGemelli = new QHBoxLayout(ui->gemelliBox);
    _layoutGrigliaNumeri= new QHBoxLayout(ui->numbersGridBox);
    _layoutPari = new QHBoxLayout(ui->pariBox);
    _progressDialog = new QProgressDialog();
    _salvaFile = new QFileDialog(this);
    _toolBar = new QToolBar(this);

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

    QStringList sFileFilters;
    sFileFilters << "Sistemi QtSEna (*.sen *.SEN)";
    _apriFile->setWindowTitle("Apri Sistema Salvato");
    _apriFile->setNameFilters(sFileFilters);
    _g3->setText("G3");
    _g3->setEnabled(false);
    _g4->setText("G4");
    _g4->setEnabled(false);
    _g5->setText("G5");
    _g5->setEnabled(false);
    _integrale->setChecked(true);
    _integrale->setText("Integrale");
    _layoutGrigliaNumeri->addWidget(_grigliaNumeri);
    _salvaFile->setWindowTitle("Salva Sistema Corrente");
    _salvaFile->setNameFilters(sFileFilters);
    _salvaFile->setAcceptMode(QFileDialog::AcceptSave);
    ui->actionG3->setEnabled(false);
    ui->actionG4->setEnabled(false);
    ui->actionG5->setEnabled(false);
    ui->actionIntegrale->setChecked(true);
    ui->actionPreferenze->setEnabled(false);

    addToolBar(_toolBar);
    _toolBar->addAction(ui->actionSistema);
    _toolBar->addSeparator();
    _toolBar->addWidget(_integrale);
    _toolBar->addWidget(_g5);
    _toolBar->addWidget(_g4);
    _toolBar->addWidget(_g3);

    connect(_grigliaNumeri, SIGNAL(aggiornato(int)), this, SLOT(aggiorna()));
    connect(&watcher, SIGNAL(finished()), _progressDialog, SLOT(cancel()));
    connect(ui->actionApri, SIGNAL(triggered()), _apriFile, SLOT(exec()));
    connect(ui->actionSalva, SIGNAL(triggered()), _salvaFile, SLOT(exec()));
    connect(_apriFile, SIGNAL(accepted()), this, SLOT(apri()));
    connect(_salvaFile, SIGNAL(accepted()), this, SLOT(salva()));
    connect(_g3, SIGNAL(clicked()), this, SLOT(on_actionG3_activated()));
    connect(_g4, SIGNAL(clicked()), this, SLOT(on_actionG4_activated()));
    connect(_g5, SIGNAL(clicked()), this, SLOT(on_actionG5_activated()));
    connect(_integrale, SIGNAL(clicked()), this, SLOT(on_actionIntegrale_activated()));
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
    delete _apriFile;
    delete _grigliaNumeri;
    delete _layoutConsecutivi;
    delete _layoutGemelli;
    delete _layoutGrigliaNumeri;
    delete _layoutPari;
    delete _progressDialog;
    delete _salvaFile;
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

void QtSEnaMainWindow::apri() {
    QFile xmlFile(_apriFile->selectedFiles().at(0));
    if(xmlFile.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xmlStream(&xmlFile);
        while(!xmlStream.atEnd()) {
            xmlStream.readNext();
            if(xmlStream.isStartElement() && xmlStream.name().toAscii() == "Elementi") {
                QStringList sElementi = QString(xmlStream.readElementText().toAscii()).split(QChar(','));
                _grigliaNumeri->deselezionaTutti();
                for (quint8 i = 0; i < sElementi.count(); i++) {
                    _grigliaNumeri->selezionaValore(sElementi.at(i).toUShort() - 1);
                }
            }
            if(xmlStream.isStartElement() && xmlStream.name().toAscii() == "Somma") {
                ui->maxSum->setValue(xmlStream.attributes().value("max").toAscii().toInt());
                ui->minSum->setValue(xmlStream.attributes().value("min").toAscii().toInt());
            }
            if(xmlStream.isStartElement() && xmlStream.name().toAscii() == "Consecutivi") {
                QStringList sConsecutivi = QString(xmlStream.readElementText().toAscii()).split(QChar(','));
                for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++)
                    _consecutivi.at(i)->setChecked(false);
                for (quint8 i = 0; i < sConsecutivi.count(); i++) {
                    _consecutivi.at(sConsecutivi.at(i).toUShort())->setChecked(true);
                }
            }
            if(xmlStream.isStartElement() && xmlStream.name().toAscii() == "Gemelli") {
                QStringList sGemelli = QString(xmlStream.readElementText().toAscii()).split(QChar(','));
                for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++)
                    _gemelli.at(i)->setChecked(false);
                for (quint8 i = 0; i < sGemelli.count(); i++) {
                    _gemelli.at(sGemelli.at(i).toUShort())->setChecked(true);
                }
            }
            if(xmlStream.isStartElement() && xmlStream.name().toAscii() == "Pari") {
                QStringList sPari = QString(xmlStream.readElementText().toAscii()).split(QChar(','));
                for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++)
                    _pari.at(i)->setChecked(false);
                for (quint8 i = 0; i < sPari.count(); i++) {
                    _pari.at(sPari.at(i).toUShort())->setChecked(true);
                }
            }
        }
        xmlFile.close();
#ifdef _DEBUG_FLAG_ENABLED
        if (xmlStream.hasError()){
            qWarning() << "[QTSENAMAINWINDOW] - on_actionApri_triggered() - Errore: " << xmlStream.errorString();
        }
    } else {
        qWarning() << "[QTSENAMAINWINDOW] - on_actionApri_triggered() - Impossibile leggere dal file " << _nomeFile;
#endif //_DEBUG_FLAG_ENABLED
    }
}

void QtSEnaMainWindow::on_actionG3_activated() {
    if(!_g3->isChecked())
        _g3->setChecked(true);
    ui->actionG3->setChecked(true);
    ui->actionG4->setChecked(false);
    ui->actionG5->setChecked(false);
    ui->actionIntegrale->setChecked(false);
}

void QtSEnaMainWindow::on_actionG4_activated() {
    if(!_g4->isChecked())
        _g4->setChecked(true);
    ui->actionG3->setChecked(false);
    ui->actionG4->setChecked(true);
    ui->actionG5->setChecked(false);
    ui->actionIntegrale->setChecked(false);
}

void QtSEnaMainWindow::on_actionG5_activated() {
    if(!_g5->isChecked())
        _g5->setChecked(true);
    ui->actionG3->setChecked(false);
    ui->actionG4->setChecked(false);
    ui->actionG5->setChecked(true);
    ui->actionIntegrale->setChecked(false);
}

void QtSEnaMainWindow::on_actionIntegrale_activated() {
    if(!_integrale->isChecked())
        _integrale->setChecked(true);
    ui->actionG3->setChecked(false);
    ui->actionG4->setChecked(false);
    ui->actionG5->setChecked(false);
    ui->actionIntegrale->setChecked(true);
}



void QtSEnaMainWindow::on_actionSistema_triggered() {
    if(_sistema != NULL)
        delete _sistema;
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

    _progressDialog->setWindowTitle("Computazione combinazioni valide...");
    _progressDialog->setLabelText(QString("Elaborazione %1 colonne in corso...").arg(_grigliaNumeri->numeroColonneSistema()));
    _progressDialog->setMinimum(0);
    _progressDialog->setMaximum(0);
    _progressDialog->setWindowModality(Qt::WindowModal);
    _progressDialog->exec();

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
    ui->statusbar->showMessage(sMessage);
    QString sLabel = QString("<B>Colonne Sistema:</B><BR> %1<BR><B>Costo Sistema:</B><BR>%2 euro").arg(_colList->count()).arg(_colList->count()/2);
    ui->risultatoLabel->setText(sLabel);
}

void QtSEnaMainWindow::salva() {
    QFile xmlFile(_salvaFile->selectedFiles().at(0));
    if(xmlFile.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xmlStream(&xmlFile);
        xmlStream.setAutoFormatting(true);
        xmlStream.writeStartDocument();
        xmlStream.writeStartElement("Sistema");
        QString sElementi = "";
        for(quint8 i = 0; i < _grigliaNumeri->listaElementi().count(); i++) {
            sElementi.append(QString("%1,").arg(_grigliaNumeri->listaElementi().at(i)));
        }
        sElementi.chop(1);
        xmlStream.writeTextElement("Elementi", sElementi);
        xmlStream.writeStartElement("Condizionamento");
        xmlStream.writeStartElement("Somma");
        xmlStream.writeAttribute("max", QString("%1").arg(ui->maxSum->value()));
        xmlStream.writeAttribute("min", QString("%1").arg(ui->minSum->value()));
        xmlStream.writeEndElement(); // Somma
        QString sConsecutivi = "";
        for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
            if(_consecutivi.at(i)->isChecked())
                sConsecutivi += QString("%1,").arg(i);
        }
        sConsecutivi.chop(1);
        xmlStream.writeTextElement("Consecutivi", sConsecutivi);
        QString sGemelli = "";
        for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
            if(_gemelli.at(i)->isChecked())
                sGemelli += QString("%1,").arg(i);
        }
        sGemelli.chop(1);
        xmlStream.writeTextElement("Gemelli", sGemelli);
        QString sPari = "";
        for (quint8 i = 0; i <= _NUMERO_ELEMENTI_COLONNA; i++) {
            if(_pari.at(i)->isChecked())
                sPari += QString("%1,").arg(i);
        }
        sPari.chop(1);
        xmlStream.writeTextElement("Pari", sPari);
        xmlStream.writeEndElement(); // Condizionamento
        xmlStream.writeEndElement(); // Sistema
        xmlStream.writeEndDocument();
        xmlFile.close();
#ifdef _DEBUG_FLAG_ENABLED
    } else {
        qWarning() << "[QTSENAMAINWINDOW] - on_actionSave_triggered() - Impossibile scrivere sul file " << _nomeFile;
#endif //_DEBUG_FLAG_ENABLED
    }
}
