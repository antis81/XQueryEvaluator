#ifndef XQEMESSAGEHANDLER_H
#define XQEMESSAGEHANDLER_H

#include <QtXmlPatterns/QAbstractMessageHandler>
#include <QtCore/QStringList>

/**
Provides a message handler to handle Error messages
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
