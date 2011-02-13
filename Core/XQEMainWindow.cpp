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

#include "TextEditing/AutoIndent.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>

#include <QtCore/QTime>


/**
The main window´s constructor sets up the user interface.
*/
XQEMainWindow::XQEMainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::XQEMainWindow )
    , _textQuery( new XQEditor )
    , _xmlEditor(0)
{
    ui->setupUi(this);

    setWindowTitle( QString("%1 (%2)").arg( qApp->applicationName() ).arg( qApp->applicationVersion() ) );
    qApp->setWindowIcon( QIcon(":/AppIcon.svg") );

    //! @todo TESTING PURPOSES
    //    QFont myFont = _textQuery->font();
    //    myFont.setPixelSize(24);
    //    _textQuery->setFont(myFont);

	QLayout *l = ui->myPlace->layout();
	if (l == 0)
		l = new QGridLayout(ui->myPlace);
	l->setContentsMargins(0,0,0,0);

	l->addWidget( _textQuery );

	ui->toolBar->setIconSize(QSize(21,21));

	_combo = new QComboBox();
	_combo->setToolTip( tr("Select the type of query.") );
	_combo->addItem( "XQuery 1.0", QXmlQuery::XQuery10 );
	_combo->addItem( "XSLT 2.0", QXmlQuery::XSLT20 );
	connect( _combo, SIGNAL(activated(int)), this, SLOT(queryLanguageSelected(int)) );

	ui->toolBar->addWidget(_combo);
	ui->toolBar->addSeparator();

	QMenu *m = menuBar()->addMenu( tr("Query") );

	QAction *a = m->addAction( tr("Open"), this, SLOT(actionOpenQuery()), QKeySequence( Qt::CTRL + Qt::Key_O ));
	a->setToolTip( tr("Open a query file.") );
	a = m->addAction( tr("Save"), this, SLOT(actionSaveQuery()), QKeySequence( Qt::CTRL + Qt::Key_S ) );
	a->setToolTip( tr("Save your query.") );

	m->addSeparator();

	// menu action run
	a = m->addAction( QIcon(":/start.svg"), tr("Run"), this,
					 SLOT( startQuery() ), QKeySequence( Qt::CTRL + Qt::Key_R ) );

	ui->toolBar->addAction(a);

	// menu action indent output
	a = m->addAction( QIcon(":/indent.svg"), tr("Indent Output"),
					 this, SLOT(changeFormattedOutput(bool)) );
	a->setCheckable(true);
	a->setChecked( _xqeval.formattedOutput() );
	ui->toolBar->addAction(a);

	m->addSeparator();
	a = m->addAction( tr("Indent Query"), this, SLOT(autoIndent()), QKeySequence( Qt::CTRL + Qt::Key_I ) );

	m = menuBar()->addMenu( tr("Help") );
	m->addAction( tr("About"), this, SLOT(about()) );
	m->addAction( tr("About &Qt"), qApp, SLOT(aboutQt()) );
}

XQEMainWindow::~XQEMainWindow()
{
    delete ui;

    if (_xmlEditor)
        delete _xmlEditor;
}

/**
The application UI´s language changed.
*/
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

/**
Starts evaluations of a query.
*/
void XQEMainWindow::startQuery()
{
    const QString source = loadSourceFile( ui->textSourceFile->text() );

    QTime stopWatch;
    stopWatch.start();

    QString errLog;
    const QString &out = _xqeval.transform( source, _textQuery->xqText(), errLog );

    int duration = stopWatch.elapsed(); // time measurement

    XQEOutput dlg;
    dlg.setWindowTitle( tr("Query Result") );
    dlg.setDuration(duration); // the duration of the query
    dlg.setXml(out); // show resulting XML
    dlg.setErrors(errLog);

    dlg.exec();
}

/**
Open a source XML file.
*/
void XQEMainWindow::on_btnOpenSource_clicked()
{
    ui->textSourceFile->setText(selectSourceFile());
}

/**
The XML source should be shown.
*/
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

/**
Wrapper action to open a query file. The file type depends on the selected query language.
*/
void XQEMainWindow::actionOpenQuery()
{
    // check for modified query first
    if ( !queryCanClose() )
        return;

    QString filter;
    switch (_xqeval.queryLanguage())
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

    loadQuery(destFile);
}

/**
Load a query with the given fileName.
*/
void XQEMainWindow::loadQuery(QString fileName)
{
    if ( fileName.isEmpty() )
        return;

    fileName = QDir::cleanPath(fileName);
    QFile queryFile(fileName);

    if ( queryFile.open(QIODevice::ReadOnly) )
    {
        _queryFileName = fileName; // remember the file name

        _textQuery->setXQText( QString::fromUtf8(queryFile.readAll()) );
        setWindowTitle( QString( "%1 - %2" ).arg( qApp->applicationName() ).arg( QFileInfo(queryFile).fileName() ) );
    }
}

/**
Action wrapper for saving a query.
*/
void XQEMainWindow::actionSaveQuery()
{
    saveQuery();
}

/**
Select a source XML file to run the query on.
*/
QString XQEMainWindow::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), QString(), "*.xml") );
}

/**
Loads a source XML file and assigns it´s content to a string.
*/
QString XQEMainWindow::loadSourceFile(const QString &path) const
{
    if (path.isEmpty())
        return QString();

    QFile f(path);
    if (f.open(QIODevice::ReadOnly))
        return f.readAll();

    return QString();
}

/**
Changed the language type of a query.
*/
void XQEMainWindow::queryLanguageSelected(int comboIndex)
{
    QXmlQuery::QueryLanguage ql =
            static_cast<QXmlQuery::QueryLanguage>( _combo->itemData(comboIndex).toInt() );

    _xqeval.setQueryLanguage(ql);
}

/**
The queries file name changed.
*/
void XQEMainWindow::queryFileNameChanged(const QString &newFileName)
{
    _queryFileName = newFileName;
    setWindowTitle( QString("%1 - %2").arg( qApp->applicationName() ).arg( QFileInfo(newFileName).fileName() ) );
    //update();
}


/**
The main window´s close event.
*/
void XQEMainWindow::closeEvent(QCloseEvent *e)
{
    if ( queryCanClose() )
        e->accept();
    else
        e->ignore();
}

bool XQEMainWindow::queryCanClose()
{
    bool mayClose = true;
    if ( _textQuery->modified() && !_textQuery->xqText().isEmpty() )
    {
        QMessageBox message;
        //message.setIconPixmap( pixmapForSvg(":/xqe_resource/question.svg", QSize(32,32)) );
        int btn = message.question( this, tr("Save XQuery?"), tr("Save your query?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        switch ( btn )
        {
        case QMessageBox::Save:
            mayClose = saveQuery();
            break;

        case QMessageBox::Discard:
            mayClose = true;
            break;

        default:
            mayClose = false;
        }
    }

    return mayClose;
}

/**
  Shows a dialog to save the current shown query.

  @return True if query was saved, otherwise false.
*/
bool XQEMainWindow::saveQuery()
{
    QString ext;
    switch ( _xqeval.queryLanguage() )
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

/**
Decides if the queries output will be indented.
*/
void XQEMainWindow::changeFormattedOutput(bool enabled)
{
    _xqeval.setFormattedOutput(enabled);
}

/**
Auto indents the query text.
*/
void XQEMainWindow::autoIndent()
{
    if (!_textQuery->hasFocus())
        _textQuery->setFocus();
    _textQuery->autoIndent();
}

/**
Shows the applications about dialog.
*/
void XQEMainWindow::about()
{
    QMessageBox::about( 0, tr("About %1").arg( qApp->applicationName() ),
                       tr( "<h1>%1</h1>"
                          "<h2>Version: %2</h2><hr/>"
                          "%1 is a little editor to help writing XQuery scripts."
                          " It features highlighting and basic auto completion and error handling."
                          " Basic XSLT and XPath support is also included." )
                       .arg( qApp->applicationName() )
                       .arg( qApp->applicationVersion() )
                       );
}
