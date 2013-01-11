#ifndef QTSENAMAINWINDOW_H
#define QTSENAMAINWINDOW_H

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>

#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QProgressDialog>
#include <QRadioButton>
#include <QToolBar>

#include "griglianumeri.h"
#include "sistema.h"

namespace Ui {
class QtSEnaMainWindow;
}

class QtSEnaMainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    Ui::QtSEnaMainWindow *ui;
    GrigliaNumeri *_grigliaNumeri;
    QFileDialog *_apriFile, *_salvaFile;
    QHBoxLayout *_layoutConsecutivi, *_layoutGemelli, *_layoutGrigliaNumeri, *_layoutPari;

    QList<QCheckBox *> _consecutivi, _gemelli, _pari;

    QFutureWatcher<bool> watcher;
    QFuture<bool> future;
    QProgressDialog *_progressDialog;
    QRadioButton *_integrale, *_g3, *_g4, *_g5;
    QToolBar *_toolBar;

    Sistema *_sistema;
    QList<Colonna > *_colList;

private slots:
    void aggiorna();
    void apri();
    void on_actionG3_activated();
    void on_actionG4_activated();
    void on_actionG5_activated();
    void on_actionIntegrale_activated();
    void on_actionSistema_triggered();
    void salva();

public:
    explicit QtSEnaMainWindow(QWidget *parent = 0);
    ~QtSEnaMainWindow();


};

#endif // QTSENAMAINWINDOW_H
