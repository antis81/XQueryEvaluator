#ifndef XQEVALUATOR_H
#define XQEVALUATOR_H

#include <QtXmlPatterns/QXmlQuery>

#include "XQEMessageHandler.h"

class XQEvaluator
{
public:
    XQEvaluator();
    ~XQEvaluator();

    QXmlQuery::QueryLanguage queryLanguage() const;
    void setQueryLanguage(QXmlQuery::QueryLanguage newVal);

    bool formattedOutput() const;

    QString transform(const QString &source, const QString &query, QString &err);

    void setFormattedOutput(bool enabled);

private:
    QXmlQuery::QueryLanguage    _queryLanguage;
    bool                        _formattedOutput;
};

#endif // XQEVALUATOR_H
