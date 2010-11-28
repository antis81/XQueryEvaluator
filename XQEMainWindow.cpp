#include "XQEMainWindow.h"

#include "ui_XQEMainWindow.h"

#include "XQEOutput.h"
#include "XQEMessageHandler.h"
#include "XQEditor.h"

#include <QtXmlPatterns/QtXmlPatterns>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>


XQEMainWindow::XQEMainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::XQEMainWindow)
  , _textQuery(new XQEditor)
{
    ui->setupUi(this);

	QLayout *l = ui->myPlace->layout();
	if (l == 0)
		l = new QGridLayout(ui->myPlace);
	l->setContentsMargins(0,0,0,0);

	l->addWidget( _textQuery );
}

XQEMainWindow::~XQEMainWindow()
{
	delete ui;
}

void XQEMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void XQEMainWindow::on_btnQuery_clicked()
{
    const QString source = loadSourceFile( ui->textSourceFile->text() );
    QXmlQuery query( qQXmlQuery::XQuery10 );
    XQEMessageHandler msgHandler;
    query.setMessageHandler(&msgHandler);

    query.setFocus(source);

    query.setQuery( _textQuery->xqText() );
    //    QXmlFormatter serializer(query, );
    QTime stopWatch;
    stopWatch.start();

    //QString         out;
    QBuffer outBuffer;
    outBuffer.open(QIODevice::WriteOnly);

    QXmlSerializer serializer(query, &outBuffer);
    query.evaluateTo(&serializer);

    QString out = QString::fromUtf8(outBuffer.data().constData());

    int duration = stopWatch.elapsed(); // time measurement

    XQEOutput dlg;
    dlg.setDuration(duration); // the duration of the query
    dlg.setXml(out); // show resulting XML
    if ( msgHandler.errLog().isEmpty() )
    {
        QString noErrors = tr("<p style=\"background-color:#44FF44;\">XQuery parsed. Everything Ok.</p>");
        dlg.setErrors(noErrors);
    }
    else
    {
        dlg.setErrors(msgHandler.errLog()); // show parsing errors and warnings
    }

    dlg.exec();
}

void XQEMainWindow::on_btnOpenSource_clicked()
{
    ui->textSourceFile->setText(selectSourceFile());
}

void XQEMainWindow::on_actionOpen_triggered()
{
    QString destFile = QFileDialog::getOpenFileName(0, tr("Open query file ..."), "", "*.xq");
    if (!destFile.isEmpty())
    {
        QFile dest(destFile);

        if ( dest.open(QIODevice::ReadOnly) )
        {
            _textQuery->setXQText( QString(dest.readAll()) );
        }
    }
}

void XQEMainWindow::on_actionSave_triggered()
{
    //    selectedFileExt = "*.xq";
    QString destFile = QFileDialog::getSaveFileName(0, tr("Save query file ..."), QDir::homePath(), tr("XQuery file (%1)").arg("*.xq"));
    if (!destFile.isEmpty())
    {
        QFile dest(destFile);

        if ( dest.open(QIODevice::WriteOnly) )
            dest.write( _textQuery->xqText().toUtf8() );
        else
            QMessageBox::critical(0, tr("Error"), tr("Unable to save XQuery file at: %1").arg(destFile));
    }
}

QString XQEMainWindow::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), "", "*.xml") );
}

QString XQEMainWindow::loadSourceFile(const QString &path) const
{
    if (path.isEmpty())
        return QString();

    QFile f(path);
    if (f.open(QIODevice::ReadOnly))
        return f.readAll();

    return QString();
}
