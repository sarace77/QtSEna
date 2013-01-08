#include <QtGui/QApplication>
#include "qtsenamainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSEnaMainWindow w;
    w.show();
    return a.exec();
}
