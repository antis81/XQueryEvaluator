#include "XQEvaluator.h"

#include <QtXmlPatterns/QXmlSerializer>
#include <QtXmlPatterns/QXmlFormatter>

#include <QtCore/QBuffer>


XQEvaluator::XQEvaluator()
    : _queryLanguage( QXmlQuery::XQuery10 )
    , _formattedOutput(true)
{
}

XQEvaluator::~XQEvaluator()
{
}

QXmlQuery::QueryLanguage XQEvaluator::queryLanguage() const
{
    return _queryLanguage;
}

void XQEvaluator::setQueryLanguage(QXmlQuery::QueryLanguage newVal)
{
    _queryLanguage = newVal;
}

bool XQEvaluator::formattedOutput() const
{
    return _formattedOutput;
}

void XQEvaluator::setFormattedOutput(bool enabled)
{
    _formattedOutput = enabled;
}

QString XQEvaluator::transform(const QString &source, const QString &query, QString &err)
{
    QXmlQuery theQuery( _queryLanguage );
    XQEMessageHandler msgHandler;
    theQuery.setMessageHandler(&msgHandler);

    theQuery.setFocus(source);

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
        err = QObject::tr("<p style=\"background-color:#44FF44;\">XQuery parsed. Everything Ok.</p>");

    return QString::fromUtf8(outBuffer.data());
}
