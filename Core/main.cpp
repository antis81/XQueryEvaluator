#include <QtGui/QApplication>
#include "XQEMainWindow.h"

#include <QtCore/QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("XQueryEvaluator");

    QFile f(":/DarkBlue.css");
    if (f.open(QIODevice::ReadOnly))
        a.setStyleSheet(f.readAll());

    XQEMainWindow w;
    w.show();

    return a.exec();
}
