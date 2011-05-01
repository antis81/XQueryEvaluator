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

#include "XQEdit.h"

#include <QtGui/QCompleter>
#include <QtGui/QAbstractItemView>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QScrollBar>


/**
Creates a XQEdit text editor instance to edit an XQuery/XSLT script.
*/
XQEdit::XQEdit(QWidget *parent)
    : TextEditBase(parent)
    , _completer(0)
    , _eow( "~!@#$%^&*()_+{}|:\"<>?,./;'[]\\=" )
{
    _xqueryHighlighter.setDocument( document() );

    const QFontMetrics &fm = fontMetrics();
    setTabStopWidth( fm.width(QChar(' ')) * 4 );
    setLineWrapMode(QPlainTextEdit::NoWrap);

    setupKeys();
}

XQEdit::~XQEdit()
{
}

/**
Setup keyboard keys that are used in eventKeyPressed().
*/
void XQEdit::setupKeys()
{
    // ignore the following keys while the completion dialog is visible
    _ignoreKeysOnCpl
            << Qt::Key_Enter << Qt::Key_Return
            << Qt::Key_Escape
            << Qt::Key_Tab << Qt::Key_Backtab
               ;
}

/**
Finds a word under the current text cursor.

@return The found word is returned.
*/
QString XQEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

/**
Replaces a partly typed word with a certain completion at the current text cursor position.
*/
void XQEdit::insertCompletion(const QString& completion)
{
    if (_completer->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int extra = completion.length() - _completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

/**
Sets a QCompleter to the XQEdit, that cares for text completion.
*/
void XQEdit::setCompleter(QCompleter *completer)
{
    if ( _completer )
        QObject::disconnect(_completer, 0, this, 0);

    _completer = completer;

    if ( _completer == 0 )
        return;

    completer->setWidget( this );
    QObject::connect(_completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

/**
When the user types, keyPressEvent is called and completions are checked.
*/
void XQEdit::keyPressEvent(QKeyEvent *e)
{
    if ( _completer == 0 )
    {
        QPlainTextEdit::keyPressEvent(e);
        return;
    }

    if ( _completer->popup()->isVisible() && _ignoreKeysOnCpl.contains( e->key() ) )
    {
        // let the completer do default behavior
        e->ignore();
        return;
    }

    // ignore the shortcut when completer is already visible
    const bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
    if ( !isShortcut )
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if ( ctrlOrShift && e->text().isEmpty() )
        return;

    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if ( !isShortcut &&
            ( hasModifier || e->text().isEmpty()
             || completionPrefix.length() < 2
             || _eow.contains(e->text().right(1)) ) )
    {
        _completer->popup()->hide();
        return;
    }

    if (completionPrefix != _completer->completionPrefix()) {
        _completer->setCompletionPrefix(completionPrefix);
        _completer->popup()->setCurrentIndex(_completer->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(_completer->popup()->sizeHintForColumn(0)
                + _completer->popup()->verticalScrollBar()->sizeHint().width());

    _completer->complete(cr); // pop it up!
}
