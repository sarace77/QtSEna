#ifndef QTSENAMAINWINDOW_H
#define QTSENAMAINWINDOW_H

#include <QHBoxLayout>
#include <QMainWindow>

#include "numbersgrid.h"

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

private slots:
    void update();
    void on_actionIntegrale_triggered();

public:
    explicit QtSEnaMainWindow(QWidget *parent = 0);
    ~QtSEnaMainWindow();


};

#endif // QTSENAMAINWINDOW_H
