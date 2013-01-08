#include "qtsenamainwindow.h"
#include "ui_qtsenamainwindow.h"


QtSEnaMainWindow::QtSEnaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtSEnaMainWindow)
{
    ui->setupUi(this);
    numbersGridLayout = new QHBoxLayout(ui->numbersGridBox);
    numbersGridWidget = new NumbersGrid();
    numbersGridLayout->addWidget(numbersGridWidget);

    connect(numbersGridWidget, SIGNAL(clicked(int)), this, SLOT(update()));
}

QtSEnaMainWindow::~QtSEnaMainWindow()
{
    delete ui;
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
}
