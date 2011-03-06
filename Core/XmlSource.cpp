/**
**    Copyright 2011 by Nils Fenner
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

#include "XmlSource.h"
#include "ui_XmlSource.h"

#include <QtCore/QSettings>
#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QHideEvent>
#include <QtGui/QLineEdit>


XmlSource::XmlSource(QWidget *parent)
    : QWidget(parent)
    , _editor(0)
{
    QLayout *l = new QHBoxLayout(this);
    l->setSpacing(3);
    l->setContentsMargins(3, 1, 3, 1);

    _textSourceFile = new QComboBox();
    _textSourceFile->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    QLineEdit * le = new QLineEdit();
    //! @todo Workaround for a Qt bug when building with Qt versions prior to 4.7
    le->setProperty( "placeholderText", tr("XML source file") );
    _textSourceFile->setLineEdit(le);
    l->addWidget(_textSourceFile);

    _btnOpenSource = new QToolButton();
    _btnOpenSource->setObjectName("btnOpenSource");
    _btnOpenSource->setText("...");
    l->addWidget(_btnOpenSource);

    connect( _btnOpenSource, SIGNAL(clicked()), this, SLOT(on_btnOpenSource_clicked()) );
    connect( _textSourceFile, SIGNAL(textChanged(QString)), this, SLOT(setSourceFile(QString)) );

    readSettings();
}

XmlSource::~XmlSource()
{
}

/**
Open a source XML file.
*/
void XmlSource::on_btnOpenSource_clicked()
{
    const QString &fileName = selectSourceFile();

    if ( !fileName.isEmpty() )
        _textSourceFile->setEditText( fileName );
}

/**
Select a source XML file to run the query on.
*/
QString XmlSource::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), QString(), "*.xml *.html *.htm") );
}

QString XmlSource::sourceFile() const
{
    return _sourceFile;
}

void XmlSource::setSourceFile(QString sourceFile)
{
    _sourceFile = QDir::cleanPath( sourceFile );

    emit sourceFileAvailable( !_sourceFile.isEmpty() );
}

void XmlSource::hideEvent(QHideEvent *ev)
{
    writeSettings();
    ev->accept();
}

void XmlSource::readSettings()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope
                       , QCoreApplication::organizationName(), QCoreApplication::applicationName() );

    settings.beginGroup("XmlSource");
    _textSourceFile->lineEdit()->setText( settings.value("sourceFile", QString()).toString() );
    _externalEditor = settings.value("externalEditor").toString();
    settings.endGroup();
}

void XmlSource::writeSettings()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope
                       , QCoreApplication::organizationName(), QCoreApplication::applicationName() );

    settings.beginGroup("XmlSource");
    settings.setValue( "sourceFile", _sourceFile );
    settings.setValue( "externalEditor", _externalEditor );
    settings.endGroup();
}


/**
Starts an external editor for editing the XML source.
*/
void XmlSource::editSource()
{
    if ( _sourceFile.isEmpty() || _externalEditor.isEmpty() )
        return;

    QProcess p;

#ifdef Q_WS_MACX
    QString appString( "open -a %1 %2" );
#else
    QString appString( "%1 %2" );
#endif

    appString = appString.arg(_externalEditor).arg( sourceFile() );

    p.startDetached( appString );
}
