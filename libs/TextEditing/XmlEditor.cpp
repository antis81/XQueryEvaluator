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

#include "XmlEditor.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QHBoxLayout>

#include <QtCore/QSettings>

#include "TextEditBase.h"


/**
Creates an XML viewer/editor instance with a highlighter.
*/
XmlEditor::XmlEditor(QWidget *parent)
    : TextEditBase(parent)
{
    _highlighter.setDocument( document() );

    const QFontMetrics &fm = fontMetrics();
    setTabStopWidth( fm.width(QChar(' ')) * 4 );

    //! @todo Prevent writing for the moment.
    setReadOnly(true);
    setLineWrapMode(QPlainTextEdit::NoWrap);

    readSettings();
}

XmlEditor::~XmlEditor()
{
}

/**
Reads settings from the apropriate settings database and applies it to this XmlEditor instance.
*/
void XmlEditor::readSettings()
{
    QSettings settings;

    settings.beginGroup("XmlEditor");

    move( settings.value("pos", pos()).toPoint() );
    resize( settings.value("size", size()).toSize() ) ;

    if ( settings.value("maximized", false).toBool() )
        setWindowState( windowState() | Qt::WindowMaximized );

    settings.endGroup();
}

/**
Writes the current XmlEditor's settings to the apropriate settings database.
*/
void XmlEditor::writeSettings()
{
    QSettings settings;

    settings.beginGroup("XmlEditor");

    settings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() )
    {
        settings.setValue( "pos", pos() );
        settings.setValue( "size", size() );
    }

    settings.endGroup();
}

/**
The XmlEditor becomes invisible.
*/
void XmlEditor::hideEvent(QHideEvent *ev)
{
    writeSettings();
    ev->accept();
}
