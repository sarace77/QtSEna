#ifndef QTSENAMAINWINDOW_H
#define QTSENAMAINWINDOW_H

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>

#include <QHBoxLayout>
#include <QMainWindow>
#include <QProgressDialog>

#include "numbersgrid.h"
#include "sistema.h"

namespace Ui {
class QtSEnaMainWindow;
}

class QtSEnaMainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    Ui::QtSEnaMainWindow *ui;
    NumbersGrid *numbersGridWidget;
    QHBoxLayout *numbersGridLayout;

    QFutureWatcher<bool> watcher;
    QFuture<bool> future;
    QProgressDialog *progressDialog;

    Sistema *_sistema;
    QList<Colonna > *_colList;

private slots:
    void update();
    void on_actionIntegrale_triggered();

public:
    explicit QtSEnaMainWindow(QWidget *parent = 0);
    ~QtSEnaMainWindow();


};

#endif // QTSENAMAINWINDOW_H
