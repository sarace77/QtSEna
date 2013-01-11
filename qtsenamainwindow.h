#ifndef QTSENAMAINWINDOW_H
#define QTSENAMAINWINDOW_H

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QProgressDialog>

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
    QHBoxLayout *_layoutConsecutivi, *_layoutGemelli, *_layoutGrigliaNumeri, *_layoutPari;

    QList<QCheckBox *> _consecutivi, _gemelli, _pari;


    QFutureWatcher<bool> watcher;
    QFuture<bool> future;
    QProgressDialog *progressDialog;

    Sistema *_sistema;
    QList<Colonna > *_colList;

private slots:
    void aggiorna();
    void on_actionIntegrale_triggered();

public:
    explicit QtSEnaMainWindow(QWidget *parent = 0);
    ~QtSEnaMainWindow();


};

#endif // QTSENAMAINWINDOW_H
