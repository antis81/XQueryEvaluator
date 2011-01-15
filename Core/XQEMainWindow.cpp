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

#include "XQEMainWindow.h"

#include "ui_XQEMainWindow.h"

#include "XQEOutput.h"
#include "Query/XQEMessageHandler.h"
#include "TextEditing/XQEditor.h"

#include <QtXmlPatterns/QXmlSerializer>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>

#include <QtCore/QBuffer>
#include <QtCore/QTime>

XQEMainWindow::XQEMainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::XQEMainWindow )
    , _textQuery( new XQEditor )
    , _queryLanguage( QXmlQuery::XQuery10 )
{
    ui->setupUi(this);

	QLayout *l = ui->myPlace->layout();
	if (l == 0)
		l = new QGridLayout(ui->myPlace);
	l->setContentsMargins(0,0,0,0);

	l->addWidget( _textQuery );

	_combo = new QComboBox();
	_combo->addItem( "XQuery 1.0", QXmlQuery::XQuery10 );
	_combo->addItem( "XSLT 2.0", QXmlQuery::XSLT20 );
	connect( _combo, SIGNAL(activated(int)), this, SLOT(queryLanguageSelected(int)) );

	ui->toolBar->addWidget(_combo);

	QAction *a = new QAction( "Run", this );
	a->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_R ) );
	connect( a, SIGNAL( triggered(bool) ), this, SLOT( on_btnQuery_clicked() ) );
	ui->toolBar->addAction(a);

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
    QXmlQuery query( _queryLanguage );
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
    QString filter;
    switch (_queryLanguage)
    {
    default: // XQuery 1.0
        filter = "*.xq *.xquery";
        break;
    case QXmlQuery::XSLT20:
        filter = "*.xsl *.xslt";
        break;
    }

    QString startPath = _queryFileName;
    if ( startPath.isEmpty() )
        startPath = QDir::homePath();

    const QString &destFile = QFileDialog::getOpenFileName(0, tr("Open query file ..."), startPath, filter);
    if (!destFile.isEmpty())
    {
        _queryFileName = destFile; // remember the file name

        QFile dest(_queryFileName);

        if ( dest.open(QIODevice::ReadOnly) )
        {
            _textQuery->setXQText( QString(dest.readAll()) );
        }
    }
}

void XQEMainWindow::on_actionSave_triggered()
{
    QString ext;
    switch (_queryLanguage)
    {
    default: // XQuery 1.0
        ext = "xq";
        break;
    case QXmlQuery::XSLT20:
        ext = "xsl";
        break;
    }

    QString startPath;
    if ( _queryFileName.isEmpty() )
        startPath = QDir::homePath();
    else
        startPath = _queryFileName;

    QFileDialog fd( 0, tr("Save query file ..."), startPath );
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setDefaultSuffix(ext);
    fd.setFileMode(QFileDialog::AnyFile);
    connect( &fd, SIGNAL( fileSelected(const QString &) ), SLOT(queryFileNameChanged(const QString &)),
            Qt::DirectConnection );

    if ( !fd.exec() )
        return;

    QFile dest(_queryFileName);

    if ( dest.open(QIODevice::WriteOnly) )
        dest.write( _textQuery->xqText().toUtf8() );
    else
        QMessageBox::critical(0, tr("Error"), tr("Unable to save XQuery file at: %1").arg(_queryFileName));
}

QString XQEMainWindow::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), QString(), "*.xml") );
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

void XQEMainWindow::queryLanguageSelected(int comboIndex)
{
    QXmlQuery::QueryLanguage ql = static_cast<QXmlQuery::QueryLanguage>( _combo->itemData(comboIndex).toInt() );

    _queryLanguage = ql;
}

void XQEMainWindow::queryFileNameChanged(const QString &newFileName)
{
    _queryFileName = newFileName;
}