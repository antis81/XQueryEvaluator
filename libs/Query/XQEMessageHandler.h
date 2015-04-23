/*
 * XQueryEvaluator - Edit and run XQuery / XSLT scripts.
 * Copyright (C) 2011-2015 by Nils Fenner <nils@macgitver.org>
 *
 * This file is part of XQueryEvaluator.
 *
 * XQueryEvaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XQueryEvaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XQEMESSAGEHANDLER_H
#define XQEMESSAGEHANDLER_H

#include <QtXmlPatterns/QAbstractMessageHandler>
#include <QtCore/QStringList>

/**
Provides a message handler to handle Error messages.
*/
class XQEMessageHandler : public QAbstractMessageHandler
{
public:
    explicit XQEMessageHandler(QObject *parent=0);
    virtual ~XQEMessageHandler();

    void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

    const QString& errLog() const
    {
        return _errLog;
    }

private:
    QString     _errLog;
};

#endif // XQEMESSAGEHANDLER_H
