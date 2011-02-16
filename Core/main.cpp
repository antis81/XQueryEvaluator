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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>

#include "MainApplication.h"

#include "XQEMainWindow.h"

#ifdef Q_OS_MAC
#  define SHARE_PATH "/../Resources"
#else
#  define SHARE_PATH "/../share/" APP_NAME
#endif

//void setupTranslators(QApplication &a)
//{
//    const QString locale = QLocale::system().name();
//    QTranslator qtTranslator;
//    qtTranslator.load( QLatin1String("qt_") + locale,
//                      QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
//    a.installTranslator( &qtTranslator );

//    QTranslator translator;
//    bool ok = translator.load( QLatin1String("xqeval_") + locale
//                    , QDir::cleanPath( QCoreApplication::applicationDirPath() + QLatin1String( SHARE_PATH "/translations" ) ) );
//    a.installTranslator( &translator );
//}

int main(int argc, char *argv[])
{
    MainApplication a(argc, argv);
    a.setApplicationName( QLatin1String(APP_NAME) );
    a.setApplicationVersion( APP_VERSION );

    QStringList args = a.arguments();

    // setup style
    QFile f(":/DarkBlue.css");
    if (f.open(QIODevice::ReadOnly))
        a.setStyleSheet(f.readAll());

//    setupTranslators(a);
    const QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    qtTranslator.load( QLatin1String("qt_") + locale,
                      QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    a.installTranslator( &qtTranslator );

    QTranslator translator;
    translator.load( QLatin1String("xqeval_") + locale
                    , QDir::cleanPath( QCoreApplication::applicationDirPath() + QLatin1String( SHARE_PATH "/translations" ) ) );
    a.installTranslator( &translator );

    XQEMainWindow w;
    QObject::connect( &a, SIGNAL(fileOpened(QString)), &w, SLOT(loadQuery(QString)), Qt::DirectConnection );

    //!@todo Need to do it right! (file associations)
    if ( args.count() > 1 )
    {
        const QString &queryFile = args[1].trimmed();
        if (!queryFile.startsWith("-"))
            w.loadQuery(queryFile);
    }

    w.show();

    return a.exec();
}
