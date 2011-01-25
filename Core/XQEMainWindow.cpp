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
#include "XmlEditDialog.h"

#include "Query/XQEMessageHandler.h"

#include "TextEditing/XQEditor.h"
#include "TextEditing/XMLEditor.h"

#include <QtXmlPatterns/QXmlSerializer>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>

#include <QtCore/QBuffer>
#include <QtCore/QTime>


XQEMainWindow::XQEMainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::XQEMainWindow )
    , _textQuery( new XQEditor )
    , _xmlEditor(0)
    , _queryLanguage( QXmlQuery::XQuery10 )
{
    ui->setupUi(this);

    setWindowTitle( qApp->applicationName() );

    //! @todo TESTING PURPOSES
    //    QFont myFont = _textQuery->font();
    //    myFont.setPixelSize(24);
    //    _textQuery->setFont(myFont);

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
	QPixmap pm = QIcon(":/start.svg").pixmap(QSize(24,24));
	a->setIcon( QIcon(pm) );
	connect( a, SIGNAL( triggered(bool) ), this, SLOT( startQuery() ) );
	ui->toolBar->addAction(a);
}

XQEMainWindow::~XQEMainWindow()
{
    delete ui;

    if (_xmlEditor)
        delete _xmlEditor;
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

void XQEMainWindow::startQuery()
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
    dlg.setWindowTitle( tr("Query Result") );
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

void XQEMainWindow::on_btnViewSource_clicked()
{
    QString fileName = ui->textSourceFile->text();
    if ( fileName.isEmpty() )
        return;

    QFile xmlFile( QDir::cleanPath(fileName) );
    if ( !xmlFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::critical(
                    this, tr("Unable to open XML file"),
                    tr("The source document could not be opened.") );
        return;
    }

    if ( _xmlEditor == 0 )
        _xmlEditor = new XmlEditDialog();

    _xmlEditor->setWindowTitle( tr("XML Source File - %1").arg( QFileInfo(xmlFile).fileName() )  );
    _xmlEditor->setXml( QString::fromUtf8(xmlFile.readAll()) );

    _xmlEditor->show();
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
            setWindowTitle( QString( "%1 - %2" ).arg( qApp->applicationName() ).arg( QFileInfo(dest).fileName() ) );
            _textQuery->setXQText( QString::fromUtf8(dest.readAll()) );
        }
    }
}

void XQEMainWindow::on_actionSave_triggered()
{
    saveQuery();
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
    setWindowTitle( QString("%1 - %2").arg( qApp->applicationName() ).arg( QFileInfo(newFileName).fileName() ) );
    update();
}

void XQEMainWindow::closeEvent(QCloseEvent *e)
{
    bool mayQuit = true;
    if ( _textQuery->modified() && !_textQuery->xqText().isEmpty() )
    {
        QMessageBox message;
        //message.setIconPixmap( pixmapForSvg(":/xqe_resource/question.svg", QSize(32,32)) );
        int btn = message.question( this, tr("Save XQuery?"), tr("Save your query?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        switch ( btn )
        {
        case QMessageBox::Save:
            mayQuit = saveQuery();
            break;

        case QMessageBox::Discard:
            mayQuit = true;
            break;

        default:
            mayQuit = false;
        }
    }

    if (mayQuit)
        e->accept();
    else
        e->ignore();
}

/**
  Shows a dialog to save the current shown query.

  @return True if query was saved, otherwise false.
*/
bool XQEMainWindow::saveQuery()
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
    connect( &fd, SIGNAL( fileSelected(const QString &) ), SLOT(queryFileNameChanged(const QString &)) );

    if ( fd.exec() == 0 )
        return false;

    QFile dest(_queryFileName);

    if ( !dest.open(QIODevice::WriteOnly) )
    {
        // failed to save
        QMessageBox::critical(0, tr("Error"), tr("Unable to save XQuery file at: %1").arg(_queryFileName));
        return false;
    }

    dest.write( _textQuery->xqText().toUtf8() );

    return true;
}
