#include "qtsenamainwindow.h"
#include "ui_qtsenamainwindow.h"

#include "sistema.h"
#include <QTime>

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

QtSEnaMainWindow::QtSEnaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtSEnaMainWindow)
{
    ui->setupUi(this);
    numbersGridLayout = new QHBoxLayout(ui->numbersGridBox);
    numbersGridWidget = new NumbersGrid();
    numbersGridLayout->addWidget(numbersGridWidget);
    ui->progressBar->setVisible(false);

    connect(numbersGridWidget, SIGNAL(clicked(int)), this, SLOT(update()));
}

QtSEnaMainWindow::~QtSEnaMainWindow()
{
    delete ui;
}

void QtSEnaMainWindow::on_actionIntegrale_triggered() {
    Sistema integrale(numbersGridWidget->getValuesList());
    quint32 count = 0, part_count = 0, valid = 0;
    QTime timer;
    timer.start();
    ui->progressBar->setMaximum(numbersGridWidget->getSystemSize()/100);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    while (integrale.hasColonna()) {
        Colonna colonna = integrale.getColonna();
            if (colonna.isValid() && colonna.getSum() >= ui->minSum->value() && colonna.getSum() <= ui->maxSum->value()) {
                ++valid;
#ifdef _DEBUG_FLAG_ENABLED
                qDebug() << colonna.view();
#endif //_DEBUG_FLAG_ENABLED
            }
            if (++count / 100 > part_count) {
                ui->progressBar->setValue(++part_count);
            }
    }
    QString sMessage = QString("Elaborate %1 colonne su %2 in").arg(--count).arg(numbersGridWidget->getSystemSize());
    quint32 hh, mm, ss, ee;
    hh = timer.elapsed()/3600000;
    mm = (timer.elapsed() - hh *3600000) /60000;
    ss = (timer.elapsed() - hh * 3600000 - mm * 60000) /1000;
    ee = timer.elapsed() - hh * 3600000 - mm * 60000 - ss * 1000;
    sMessage += QString(" %1h %2m %3s.%4").arg(hh).arg(mm).arg(ss).arg(ee);
    if (valid != count)
        sMessage += QString(", %1 Colonne valide (%2\%)").arg(valid).arg((float)(10000 * valid/count)/100);
    ui->statusbar->showMessage(sMessage);
    ui->progressBar->setVisible(false);
}

void QtSEnaMainWindow::update() {
    ValuesList temp = numbersGridWidget->getValuesList();

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
    if(numbersGridWidget->getSystemSize() > 0) {
        sNumCol = QString("Numero Colonne Sistema non condizionato: %1").arg(numbersGridWidget->getSystemSize());
    } else {
        sNumCol = QString("Numero di valori selezionati insufficiente");
    }
    ui->statusbar->showMessage(sNumCol);

}
