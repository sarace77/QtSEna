#include <QLibraryInfo>
#include <QtGui/QApplication>
#include <QTranslator>
#include "qtsenamainwindow.h"

#include <QDebug>
int main(int argc, char *argv[])
{
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication a(argc, argv);
    a.installTranslator(&qtTranslator);
    QtSEnaMainWindow w;
    w.show();
    return a.exec();
}
