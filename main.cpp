#include <QtGui/QApplication>
#include "XQEMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XQEMainWindow w;
    w.show();
    return a.exec();
}
