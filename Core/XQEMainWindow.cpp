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

#include "XQEMainWindow.h"

#include "ui_XQEMainWindow.h"

#include "XQEOutput.h"

#include "XmlSource.h"

#include "Query/XQEMessageHandler.h"

#include "TextEditing/XQEditor.h"
#include "TextEditing/XQEdit.h"
#include "TextEditing/XMLEditor.h"
#include "TextEditing/TextSearch.h"

#include "TextEditing/AutoIndent.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include <QtGui/QCloseEvent>

#include <QtCore/QTime>
#include <QtCore/QSettings>


/**
The main window's constructor sets up the main user interface.

@todo At present, all toolbar and menu actions are set here.
This is to be outsourced in seperate functions, maybe an ActionManager class.
*/
XQEMainWindow::XQEMainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::XQEMainWindow )
    , _xmlSource( new XmlSource )
    , _textQuery( new XQEditor )
    , _xmlEditor( new XmlEditor )
{
    ui->setupUi(this);

    setWindowTitle( QString("%1 (%2)").arg( qApp->applicationName() ).arg( qApp->applicationVersion() ) );
    qApp->setWindowIcon( QIcon(":/AppIcon.svg") );

    setCentralWidget( _textQuery );

    ui->toolBar->setIconSize(QSize(21,21));

    _textQueryType = new QComboBox();
    _textQueryType->setToolTip( tr("Select the type of query.") );
    _textQueryType->addItem( "XQuery 1.0", QXmlQuery::XQuery10 );
    _textQueryType->addItem( "XSLT 2.0", QXmlQuery::XSLT20 );
    connect( _textQueryType, SIGNAL(activated(int)), this, SLOT(queryLanguageSelected(int)) );

    ui->toolBar->addWidget(_textQueryType);

    // -- Query menu
    QMenu *m = menuBar()->addMenu( tr("Query") );

    QAction *a = 0;

    a = m->addAction( tr("New"), this, SLOT(actionNewQuery()), QKeySequence( Qt::CTRL + Qt::Key_N ) );
    a->setToolTip( tr("Create a new query.") );
    a = m->addAction( tr("Open"), this, SLOT(actionOpenQuery()), QKeySequence( Qt::CTRL + Qt::Key_O ) );
    a->setToolTip( tr("Open a query file.") );
    a = m->addAction( tr("Save"), this, SLOT(actionSaveQuery()), QKeySequence( Qt::CTRL + Qt::Key_S ) );
    a->setToolTip( tr("Save your query.") );
//    a = m->addAction( tr("Save as..."), this, SLOT(actionSaveQueryAs()), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_S ) );
//    a->setToolTip( tr("Save your query at another location.") );

    m->addSeparator();

    // menu action indent output
    a = m->addAction( QIcon(":/indent.svg"), tr("Indent Output"),
                     this, SLOT(changeFormattedOutput(bool)) );
    a->setCheckable(true);
    a->setChecked( _xqeval.formattedOutput() );
    ui->toolBar->addAction(a);

    a = m->addAction( QIcon(":/start.svg"), tr("Run"), this,
                     SLOT( startQuery() ), QKeySequence( Qt::CTRL + Qt::Key_R ) );

    ui->toolBar->addAction(a);

    m->addSeparator();
    a = m->addAction( tr("Indent Query"), this, SLOT(autoIndent()), QKeySequence( Qt::CTRL + Qt::Key_I ) );

    ui->toolBar->addSeparator();
    ui->toolBar->addWidget( _xmlSource );

    a = new QAction( QIcon(":/eye.svg"), tr("View Source"), 0 );
    connect( a, SIGNAL(triggered()), this, SLOT(actionViewSource()) );
    connect( _xmlSource, SIGNAL(sourceFileAvailable(bool)), a, SLOT(setEnabled(bool)) );
    ui->toolBar->addAction(a);

    a = new QAction( QIcon(":/pen.svg"), tr("Edit Source"), 0 );
    connect( a, SIGNAL(triggered()), this, SLOT(actionEditSource()) );
    connect( _xmlSource, SIGNAL(sourceFileAvailable(bool)), a, SLOT(setEnabled(bool)) );
    ui->toolBar->addAction(a);

    // -- Edit menu
    m = menuBar()->addMenu( tr("Edit") );
    m->addActions( _textQuery->textEdit()->createStandardContextMenu()->actions() );
    m->addAction( tr("Search ..."), this, SLOT(actionSearchText()), QKeySequence(Qt::CTRL + Qt::Key_F) );


    // -- Help menu
    m = menuBar()->addMenu( QString("Help") );
    m->addAction( QString("About"), this, SLOT(about()) );
    m->addAction( QString("About &Qt"), qApp, SLOT(aboutQt()) );

    // read application settings
    readSettings();
}

XQEMainWindow::~XQEMainWindow()
{
    delete ui;

    if (_xmlEditor)
        delete _xmlEditor;
}

/**
The application UI's language changed.
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
Evaluates a query and shows the result in a seperate modal dialog.
*/
void XQEMainWindow::startQuery()
{
    const QString source = loadSourceFile( _xmlSource->sourceFile() );

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
            static_cast<QXmlQuery::QueryLanguage>( _textQueryType->itemData(comboIndex).toInt() );

    _xqeval.setQueryLanguage(ql);
}

/**
The queries file name changed.
*/
void XQEMainWindow::queryFileNameChanged(const QString &newFileName)
{
    _queryFileName = newFileName;

    QString visibleFileName = QFileInfo(_queryFileName).fileName();
    if (visibleFileName.isEmpty())
        visibleFileName = tr("New Query");

    setWindowTitle( QString("%1 (%2) - %3").arg( APP_NAME ).arg( APP_VERSION ).arg( visibleFileName ) );
}


/**
The main window's close event.
*/
void XQEMainWindow::closeEvent(QCloseEvent *e)
{
    if ( queryCanClose() )
    {
        writeSettings();
        e->accept();
    }
    else
        e->ignore();
}

/**
@return When true, it is save to overwrite the existing query.
Called for example when creating a new query or when the applicaiton quits.
*/
bool XQEMainWindow::queryCanClose()
{
    bool mayClose = true;
    if ( _textQuery->modified() && !_textQuery->xqText().isEmpty() )
    {
        QMessageBox message;
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
bool XQEMainWindow::saveQuery(bool saveAs)
{
    QString startPath;
    if ( _queryFileName.isEmpty() || saveAs )
    {
        startPath = QDir::homePath();

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

        QFileDialog fd( 0, tr("Save query file ..."), startPath );
        fd.setAcceptMode(QFileDialog::AcceptSave);
        fd.setDefaultSuffix(ext);
        connect( &fd, SIGNAL( fileSelected(const QString &) ), SLOT(queryFileNameChanged(const QString &)) );

        if ( fd.exec() == 0 )
            return false;
    }

    // write to file
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

/**
The XML source should be shown.
*/
void XQEMainWindow::actionViewSource()
{
    if ( _xmlEditor == 0 )
        return;

    QFile xmlFile( _xmlSource->sourceFile() );
    if ( !xmlFile.open(QIODevice::ReadOnly) || _xmlSource->sourceFile().isEmpty() )
    {
        QMessageBox::critical(
                    this, tr("Unable to open XML file"),
                    tr("The source document could not be opened.") );
        return;
    }

    _xmlEditor->setWindowTitle( tr("XML Source File - %1").arg( QFileInfo(xmlFile).fileName() ) );
    _xmlEditor->setXml( QString::fromUtf8(xmlFile.readAll()) );

    xmlFile.close();

    _xmlEditor->show();
}

/**
Reads settings from the apropriate settings database and applies it to this XQEMainWindow instance.
*/
void XQEMainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup( "MainWindow" );

    move( settings.value("pos", pos()).toPoint() );
    resize( settings.value("size", size()).toSize() ) ;

    if ( settings.value("maximized", false).toBool() )
        setWindowState( windowState() | Qt::WindowMaximized );

    settings.endGroup();

    settings.beginGroup( "Query" );
    const int i = _textQueryType->findText( settings.value( "type" ).toString() );
    if ( i >= 0 )
        _textQueryType->setCurrentIndex( i );

    loadQuery( settings.value( "queryFile" ).toString() );
    settings.endGroup();
}

/**
Writes the current XQEMainWindow's settings to the apropriate settings database.
*/
void XQEMainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup( "MainWindow" );

    settings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() )
    {
        settings.setValue( "pos", pos() );
        settings.setValue( "size", size() );
    }

    settings.endGroup();

    settings.beginGroup( "Query" );
    settings.setValue( "type", _textQueryType->currentText() );
    settings.setValue( "queryFile", _queryFileName );
    settings.endGroup();
}

/**
Action to create a new query. An previous query becomes invalid in this and therefore has to be removed
before the new query can be created.
*/
void XQEMainWindow::actionNewQuery()
{
    if ( queryCanClose() )
    {
        _textQuery->setXQText( QString() );
        queryFileNameChanged( QString() );
    }
}

/**
Opens the source XML document and forwards it to an editor.
*/
void XQEMainWindow::actionEditSource()
{
    _xmlSource->editSource();
}

/**
Starts a string search in the query text.
*/
void XQEMainWindow::actionSearchText()
{
    TextSearch * searchDialog = new TextSearch();

    searchDialog->setTextEdit( _textQuery->textEdit() );

    searchDialog->show();
}

//void XQEMainWindow::actionSaveQueryAs()
//{
//    saveQuery( true );
//}
