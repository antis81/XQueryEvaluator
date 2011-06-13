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

#include "XQEvaluator.h"

#include <QtXmlPatterns/QXmlSerializer>
#include <QtXmlPatterns/QXmlFormatter>

#include <QtCore/QBuffer>

/**
Constructs an evaluator object for parsing queries. By default XQuery is set as query language.
*/
XQEvaluator::XQEvaluator()
    : _queryLanguage( QXmlQuery::XQuery10 )
    , _formattedOutput(true)
    , _legacyMode(false)
{
}

XQEvaluator::~XQEvaluator()
{
}

/**
@return The set language for parsing query scripts.
*/
QXmlQuery::QueryLanguage XQEvaluator::queryLanguage() const
{
    return _queryLanguage;
}

/**
Set a language for parsing a query script.
*/
void XQEvaluator::setQueryLanguage(QXmlQuery::QueryLanguage newVal)
{
    _queryLanguage = newVal;
}

/**
When formatted output is true, the output is indented.

@return Generate formatted output or not.
*/
bool XQEvaluator::formattedOutput() const
{
    return _formattedOutput;
}

/**
Setting formatted output to false compresses the result string to a single line.

By default, formatted output is enabled.
*/
void XQEvaluator::setFormattedOutput(bool enabled)
{
    _formattedOutput = enabled;
}

/**
Transforms the source XML string to a resulting format using the given query script.
The resulting string is usually XML, but can vary in other plain text formats depending on the query.
The parameter err can be used to output error messages during the parsing process.

When formattedOutput is set, the result will be indented.
*/
QString XQEvaluator::transform(const QString &source, const QString &query, QString &err)
{
    QXmlQuery theQuery( _queryLanguage );

    XQEMessageHandler msgHandler;
    theQuery.setMessageHandler(&msgHandler);

    QBuffer input;
    if ( _legacyMode )
    {
        input.setData(source.toUtf8());
        input.open(QIODevice::ReadOnly);
        theQuery.bindVariable("inputDocument", &input);
    } else {
        theQuery.setFocus( source );
    }

    theQuery.setQuery( query );


    QBuffer outBuffer;
    outBuffer.open(QIODevice::WriteOnly);

    if ( _formattedOutput )
    {
        QXmlFormatter formatter(theQuery, &outBuffer);
        theQuery.evaluateTo(&formatter);
    } else {
        QXmlSerializer serializer(theQuery, &outBuffer);
        theQuery.evaluateTo(&serializer);
    }

    err = msgHandler.errLog();
    if ( err.isEmpty() )
        err = QObject::tr("<p style=\"background-color:#44FF44;\">Query parsed. Everything Ok.</p>");

    return QString::fromUtf8( outBuffer.data() );
}

bool XQEvaluator::legacyMode() const
{
    return _legacyMode;
}

/**
Sets the legacy mode to develop queries for Qt versions 4.4.x.
This declares the source document as an external variable $inputDocument.

Example query:
@code
declare variable $inputDocument external;

for $root in doc($inputDocument)
return <MyXml>{ $root }</MyXml>
@endcode
*/
void XQEvaluator::setLegacyMode(bool enabled)
{
    _legacyMode = enabled;
}
