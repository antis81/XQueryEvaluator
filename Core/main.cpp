/**
**    Copyright (c) 2011 by Nils Fenner
**
**    This file is part of XQueryEvaluator.
**
**    XQueryEvaluator is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    XQueryEvaluator is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>
#include "XQEMainWindow.h"


void setupTranslators(QApplication &a)
{
    const QString locale = QLocale::system().name();
    QTranslator qtTranslator(0);
    qtTranslator.load( QString("qt_") + locale,
                      QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    a.installTranslator( &qtTranslator );
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName( APP_NAME );
    a.setApplicationVersion( APP_VERSION );

    QFile f(":/DarkBlue.css");
    if (f.open(QIODevice::ReadOnly))
        a.setStyleSheet(f.readAll());

    setupTranslators(a);

    XQEMainWindow w;
    w.show();

    return a.exec();
}
