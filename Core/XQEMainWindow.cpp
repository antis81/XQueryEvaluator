/*
 * XQueryEvaluator - Edit and run XQuery / XSLT scripts.
 * Copyright (C) 2011-2015 by Nils Fenner <nils@macgitver.org>
 *
 * This file is part of XQueryEvaluator.
 *
 * XQueryEvaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XQueryEvaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "XQEMainWindow.h"

#include "XQEOutput.h"

#include "XmlSource.h"

#include "Query/XQEMessageHandler.h"

#include "TextEditing/XQEditor.h"
#include "TextEditing/XmlEditor.h"
#include "TextEditing/TextSearch.h"

#include "TextEditing/AutoIndent.h"

#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolBar>

/**
 * @brief       The main window's constructor sets up the main user interface.
 *
 * @todo        Move initialization of menu actions to an ActionManager class.
 */
XQEMainWindow::XQEMainWindow(QWidget *parent)
    : QMainWindow( parent )
    , _modified( false )
    , _outputToFile(false)
    , _xmlSource( new XmlSource )
    , _textQuery( new XQEditor )
{
    setupUi(this);

    connect( _textQuery, SIGNAL( modificationChanged(bool) ), this, SLOT( documentModified(bool) ) );

    setDockOptions( QMainWindow::AnimatedDocks );

    // setup docking areas
    _fixedDockWidgets.setOwnerWindow(this);

    _fixedDockWidgets.registerDockWidget( "search", Qt::TopDockWidgetArea );
    _fixedDockWidgets.registerDockWidget( "xml-edit", Qt::RightDockWidgetArea );

    setWindowTitle( QString("%1 (%2)").arg( qApp->applicationName() ).arg( qApp->applicationVersion() ) );
    qApp->setWindowIcon( QIcon(":/AppIcon.svg") );

    setCentralWidget( _textQuery );

    toolBar->setIconSize(QSize(21,21));

    _textQueryType = new QComboBox();
    _textQueryType->setToolTip( tr("Select the type of query.") );
    _textQueryType->addItem( "XQuery 1.0", QXmlQuery::XQuery10 );
    _textQueryType->addItem( "XSLT 2.0", QXmlQuery::XSLT20 );
    connect( _textQueryType, SIGNAL(activated(int)), this, SLOT(queryLanguageSelected(int)) );

    toolBar->addWidget( _textQueryType );

    // -- Query menu
    QMenu *m = appMenu->addMenu( tr("Quer&y") );

    QAction *a = 0;

    a = m->addAction( tr("&New"), this, SLOT(actionNewQuery()),
                      QKeySequence( Qt::CTRL + Qt::Key_N ) );
    a->setToolTip( tr("Create a new query.") );
    a = m->addAction( tr("&Open"), this, SLOT(actionOpenQuery()),
                      QKeySequence( Qt::CTRL + Qt::Key_O ) );
    a->setToolTip( tr("Open a query file.") );
    a = m->addAction( tr("&Save"), this, SLOT(actionSaveQuery()),
                      QKeySequence( Qt::CTRL + Qt::Key_S ) );
    a->setToolTip( tr("Save your query.") );
    a = m->addAction( tr("Save &as..."), this, SLOT(actionSaveQueryAs()),
                      QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_S ) );
    a->setToolTip( tr("Save your query at another location.") );

    m->addSeparator();

    // menu action indent output
    a = m->addAction( QIcon(":/indent.svg"), tr("Indent Ou&tput"),
                     this, SLOT(changeFormattedOutput(bool)) );
    a->setCheckable(true);
    a->setChecked( _xqeval.formattedOutput() );
    toolBar->addAction(a);

    a = m->addAction( QIcon(":/toFile.svg"), tr("Output to &file ..."),
                      this, SLOT(actionOutputToFile(bool)) );
    a->setCheckable(true);
    toolBar->addAction(a);

    m->addSeparator();

    a = m->addAction( QIcon(":/legacy.svg"), tr("&Legacy Mode"),
                      this, SLOT(actionLegacyMode(bool)) );
    a->setCheckable(true);
    a->setToolTip( tr("Write queries for Qt 4.4. Those queries need to declare "
                      "declare an external variable called $inputDocument. "
                      "Here's how you declare it:<br/>"
                      "<b>declare variable $inputDocument external;</b>") );
    toolBar->addAction(a);

    m->addSeparator();

    a = m->addAction( QIcon(":/start.svg"), tr("&Run"), this,
                      SLOT(startQuery()), QKeySequence(Qt::CTRL + Qt::Key_R) );

    toolBar->addAction(a);

    m->addSeparator();
    a = m->addAction( tr("&Indent Query"), this, SLOT(autoIndent()),
                      QKeySequence( Qt::CTRL + Qt::Key_I ) );

    m->addSeparator();
    a = m->addAction( tr("&Quit %1").arg(qApp->applicationName()),
                      this, SLOT(close()), QKeySequence(Qt::CTRL + Qt::Key_Q) );

    toolBar->addSeparator();
    toolBar->addWidget( _xmlSource );

    a = new QAction( QIcon(":/eye.svg"), tr("View Source"), 0 );
    a->setCheckable(true);
    connect( a, SIGNAL(triggered(bool)), this, SLOT(actionViewSource(bool)) );
    connect( _xmlSource, SIGNAL(sourceFileAvailable(bool)), a, SLOT(setEnabled(bool)) );
    toolBar->addAction(a);

    a = new QAction( QIcon(":/pen.svg"), tr("Edit Source"), 0 );
    connect( a, SIGNAL(triggered()), this, SLOT(actionEditSource()) );
    connect( _xmlSource, SIGNAL(sourceFileAvailable(bool)), a, SLOT(setEnabled(bool)) );
    toolBar->addAction(a);

    // -- Edit menu
    m = appMenu->addMenu( tr("&Edit") );
    m->addActions( _textQuery->createStandardContextMenu()->actions() );
    m->addAction( tr("Search ..."), this, SLOT(actionSearchText()), QKeySequence(Qt::CTRL + Qt::Key_F) );

    // -- Help menu
    m = appMenu->addMenu( QStringLiteral("&Help") );
    m->addAction( QStringLiteral("&About"), this, SLOT(about()) );
    m->addAction( QStringLiteral("About &Qt"), qApp, SLOT(aboutQt()) );

    // read application settings
    readSettings();
}

/**
 * @brief       The application UI's language changed.
 */
void XQEMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * @brief       Evaluates a query and displays the result.
 *
 * When @ref outputToFile() is true, the displayed result is written to the
 * specified file in addition.
 */
void XQEMainWindow::startQuery()
{
    const QString source = loadSourceFile( _xmlSource->sourceFile() );

    QTime stopWatch;
    stopWatch.start();

    QString errLog;
    const QString &out = _xqeval.transform( source, _textQuery->toPlainText(), errLog );

    int duration = stopWatch.elapsed(); // time measurement

    if ( _outputToFile )
        saveOutputToFile(out);

    XQEOutput dlg;
    dlg.setWindowTitle( tr("Query Result") );
    dlg.setDuration(duration); // the duration of the query
    dlg.setXml(out); // show resulting XML
    dlg.setErrors(errLog);

    dlg.exec();
}

/**
 * @brief   Show a dialog to open a query file.
 *
 * The file type depends on the selected query language.
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
 * @brief       Load a query with the given fileName.
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

        _textQuery->setPlainText( QString::fromLocal8Bit( queryFile.readAll()) );
        setWindowTitle( QString( "%1 - %2" ).arg( qApp->applicationName() ).arg( QFileInfo(queryFile).fileName() ) );
    }
}

void XQEMainWindow::actionSaveQuery()
{
    if ( saveQuery() )
        _modified = false;
}

/**
 * @brief       Load a source XML file.
 *
 * @param[in]   path    path to XML file
 */
QString XQEMainWindow::loadSourceFile(const QString &path) const
{
    if (path.isEmpty())
        return QString();

    QFile f(path);
    if (f.open(QIODevice::ReadOnly))
        return QString::fromLocal8Bit( f.readAll() );

    QMessageBox::critical(
                0, tr("Unable to open XML file"),
                tr("The source document could not be opened.") );

    return QString();
}

/**
 * @brief       Change the language type of a query.
 *
 * @param       comboIndex  the language index from the combo box
 */
void XQEMainWindow::queryLanguageSelected(int comboIndex)
{
    QXmlQuery::QueryLanguage ql =
            static_cast<QXmlQuery::QueryLanguage>( _textQueryType->itemData(comboIndex).toInt() );

    _xqeval.setQueryLanguage(ql);
}

/**
 * @brief       The loaded query file changed.
 *
 * @param[in]   newFileName the new file name to show in the main window title
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
 * @brief       The main window's close event.
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
 * @brief       Check, if the currently opened query file can be closed.
 *
 * @return      When true, the active query can be closed.
 */
bool XQEMainWindow::queryCanClose()
{
    bool mayClose = true;
    if ( _modified && !_textQuery->toPlainText().isEmpty() )
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
 * @brief       Shows a dialog to save the current shown query.
 *
 * @return      True if query was saved; false otherwise.
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

    const QString &content = _textQuery->toPlainText();
    qint64 bytesWritten = dest.write( content.toUtf8() );
    if ( bytesWritten == content.toUtf8().size() )
        return true;

    QMessageBox::critical( this, tr("Error on writing file"), tr("Error while writing query file %1.").arg(_queryFileName) );

    return false;
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
void XQEMainWindow::actionViewSource(bool activate)
{
    if (!activate)
    {
        _fixedDockWidgets.hide("xml-edit");
        return;
    }

    XmlEditor * xmlEditor = new XmlEditor();

    xmlEditor->setWindowTitle( tr("XML Source File - %1").arg( QFileInfo(_xmlSource->sourceFile()).fileName() ) );
    xmlEditor->setPlainText( loadSourceFile( _xmlSource->sourceFile() ) );

    if ( !_fixedDockWidgets.show("xml-edit", xmlEditor) )
    {
        // show xml editor as seperate window
        xmlEditor->show();
    }
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

    _outputFilePath = settings.value( "outputFilePath" ).toString();

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
    settings.setValue( "outputFilePath", _outputFilePath );
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
        _textQuery->setPlainText( QString() );
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
    searchDialog->setTextEdit( _textQuery );

    if ( !_fixedDockWidgets.show("search", searchDialog) )
    {
        // show as seperate window
        searchDialog->show();
    }
}

void XQEMainWindow::actionSaveQueryAs()
{
    if ( saveQuery(true) )
        documentModified(false);
}

/**
Sets an output file to which the query result will be written on executing.
*/
void XQEMainWindow::actionOutputToFile(bool yes)
{
    _outputToFile = yes;
    if ( !_outputToFile )
        return;

    // set a file name
    QString path = _outputFilePath;
    if ( path.isEmpty() )
        path = QDir::homePath();

    path = QFileDialog::getSaveFileName( this, tr("Select an output file."), QFileInfo(path).absoluteFilePath() );

    if ( !path.isEmpty() )
        _outputFilePath = QFileInfo(path).absoluteFilePath();
}

void XQEMainWindow::saveOutputToFile(const QString &content)
{
    if ( _outputFilePath.isEmpty() || content.isEmpty() )
        return;


    QFile file( _outputFilePath );
    qint64 bytesWritten = 0;
    if ( file.open(QIODevice::WriteOnly) )
    {
        bytesWritten = file.write( content.toUtf8() );
        file.close();
    } else {
        QMessageBox::critical( this, tr("Unable to open output file"),
                               tr("Unable to create or open the output file %1.").arg(_outputFilePath) );
    }

    if ( bytesWritten != content.toUtf8().size() )
        QMessageBox::critical( this, tr("Write error"), tr("Error while writng the output file %1.").arg(_outputFilePath) );
}

void XQEMainWindow::documentModified(bool modified)
{
    _modified = modified;
}

/**
Sets the legacy mode to develop queries for Qt Versions 4.4.x.
*/
void XQEMainWindow::actionLegacyMode(bool yes)
{
    _xqeval.setLegacyMode(yes);
}
