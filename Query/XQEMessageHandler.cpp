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

#include "XQEMessageHandler.h"

#include <QtGui/QMessageBox>
#include <QtGui/QTextDocumentFragment>
#include <QtCore/QtGlobal>
#include <QtNetwork/QUrlInfo>


XQEMessageHandler::XQEMessageHandler(QObject *parent)
    : QAbstractMessageHandler(parent)
{
}

XQEMessageHandler::~XQEMessageHandler()
{
}

void XQEMessageHandler::handleMessage(QtMsgType type, const QString &description,
                                      const QUrl &identifier, const QSourceLocation &sourceLocation)
{
    Q_UNUSED(identifier);

    QUrl sourceUrl = sourceLocation.uri();
    QString source;

    if ( sourceUrl.scheme().toLower() == "tag" )
        source = tr("query");
    else
        source = "xml source";

    QString css;
    QString msgType;

    switch (type)
    {
    case QtDebugMsg:
        msgType = tr("Info");
        break;

    case QtWarningMsg:
        msgType = tr("Warning");
        css = "background-color: #FFFF44; margin: 0px;";
        break;

    case QtCriticalMsg:
        msgType = tr("Error");
        css = "background-color: #FF4444; margin: 0px;";
        break;

    case QtFatalMsg:
        msgType = tr("Error");
        css = "background-color: #FF4444; margin: 0px;";
        break;

    default: ;
    }

    _errLog += tr("<p style=\"%1\">%2 in %3 at line %4 column %5:&nbsp;%6</p><hr style=\"margin: 0px;\"/>")
            .arg(css)
            .arg(msgType)
            .arg(source)
            .arg(sourceLocation.line())
            .arg(sourceLocation.column())
            .arg(QTextDocumentFragment::fromHtml(description).toPlainText());
}
