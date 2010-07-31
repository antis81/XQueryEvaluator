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
//        _errLog << tr("XQuery info at line %1 column %2: \n%3")
//                                 .arg(sourceLocation.line())
//                                 .arg(sourceLocation.column())
//                                 .arg(description);
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
