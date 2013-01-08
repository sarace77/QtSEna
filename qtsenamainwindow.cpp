#include "qtsenamainwindow.h"
#include "ui_qtsenamainwindow.h"


QtSEnaMainWindow::QtSEnaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtSEnaMainWindow)
{
    ui->setupUi(this);
    testWidget = new NumbersGrid(ui->centralwidget);

}

QtSEnaMainWindow::~QtSEnaMainWindow()
{
    delete ui;
}
