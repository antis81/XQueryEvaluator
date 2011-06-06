/*
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

#ifndef XQEVALUATOR_H
#define XQEVALUATOR_H

#include <QtXmlPatterns/QXmlQuery>

#include "XQEMessageHandler.h"

/**
Evaluates XQuery oder XSLT queries.

At present transforms the output to XML. In some cases, plain text also works depending on the query.
*/
class XQEvaluator
{
public:
    XQEvaluator();
    ~XQEvaluator();

    QXmlQuery::QueryLanguage queryLanguage() const;
    void setQueryLanguage(QXmlQuery::QueryLanguage newVal);

    bool formattedOutput() const;
    void setFormattedOutput(bool enabled);

    bool legacyMode() const;
    void setLegacyMode(bool enabled);

    QString transform(const QString &source, const QString &query, QString &err);

private:
    QXmlQuery::QueryLanguage    _queryLanguage;
    bool                        _formattedOutput;
    bool                        _legacyMode; //! Use QXmlQuery::bindVariable() instead of QXmlQuery::setFocus()

};

#endif // XQEVALUATOR_H
