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
#include <QtCore/QDebug>


XQEMessageHandler::XQEMessageHandler(QObject *parent) :
        QAbstractMessageHandler(parent)
{
}

XQEMessageHandler::~XQEMessageHandler()
{
}

void XQEMessageHandler::handleMessage(QtMsgType type, const QString &description,
                                      const QUrl &identifier, const QSourceLocation &sourceLocation)
{
    Q_UNUSED(identifier);

    switch (type)
    {
    case QtDebugMsg:
        _errLog += tr("<p>XQuery info at line %1 column %2:<br/>%3</p><hr/>")
                                 .arg(sourceLocation.line())
                                 .arg(sourceLocation.column())
                                 .arg(QTextDocumentFragment::fromHtml(description).toPlainText());
        break;

    case QtWarningMsg:
        _errLog += tr("<p style=\"background-color:#FFFF44;\">XQuery warning at line %1 column %2:<br/>%3</p><hr/>")
                                 .arg(sourceLocation.line())
                                 .arg(sourceLocation.column())
                                 .arg(QTextDocumentFragment::fromHtml(description).toPlainText());
        break;

    case QtCriticalMsg:
        _errLog += tr("<p style=\"background-color:#FF4444;\">XQuery error at line %1 column %2:<br/>%3</p><hr/>")
                                 .arg(sourceLocation.line())
                                 .arg(sourceLocation.column())
                                 .arg(QTextDocumentFragment::fromHtml(description).toPlainText());
        break;

    case QtFatalMsg:
        _errLog += tr("<p style=\"background-color:#FF4444;\">XQuery error at line %1 column %2:<br/>%3</p><hr/>")
                                 .arg(sourceLocation.line())
                                 .arg(sourceLocation.column())
                                 .arg(QTextDocumentFragment::fromHtml(description).toPlainText());
        break;
    }

    qDebug() << _errLog;
}
