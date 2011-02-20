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

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QHideEvent>


XmlSource::XmlSource(QWidget *parent) :
    QWidget(parent)
  ,  ui(new Ui::XmlSource)
  , _editor(0)
{
    ui->setupUi(this);

    QLayout *l = layout();
    if (l == 0)
        l = new QHBoxLayout(this);

    l->setSpacing(0);
    l->setContentsMargins(3,1,3,1);

    connect( ui->textSourceFile, SIGNAL(textChanged(QString)), this, SLOT(setSourceFile(QString)) );

    readSettings();
}

XmlSource::~XmlSource()
{
    delete ui;
}

/**
Open a source XML file.
*/
void XmlSource::on_btnOpenSource_clicked()
{
    const QString &fileName = selectSourceFile();

    if ( !fileName.isEmpty() )
        ui->textSourceFile->setText( fileName );
}

/**
Select a source XML file to run the query on.
*/
QString XmlSource::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), QString(), "*.xml") );
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
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), APP_NAME );

    settings.beginGroup("XmlSource");
    ui->textSourceFile->setText( settings.value("sourceFile", QString()).toString() );
    settings.endGroup();
}

void XmlSource::writeSettings()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), APP_NAME);

    settings.beginGroup("XmlSource");
    settings.setValue( "sourceFile", _sourceFile );
    settings.endGroup();
}
