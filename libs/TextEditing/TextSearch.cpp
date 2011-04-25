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

#include "TextSearch.h"
#include "ui_TextSearch.h"

#include <QtGui/QPlainTextEdit>

/**
Constructor of TextSearch.
*/
TextSearch::TextSearch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextSearch)
    , _textEdit(0)
{
    ui->setupUi(this);
}

/**
Destructor of TextSearch.
*/
TextSearch::~TextSearch()
{
    delete ui;
}

/***/
QPlainTextEdit * TextSearch::textEdit()
{
    return _textEdit;
}

/**
Sets the text edit to which the search relates.
*/
void TextSearch::setTextEdit(QPlainTextEdit *edit)
{
    _textEdit = edit;
}

void TextSearch::on_btnForward_clicked()
{
    find( ui->textSearch->text() );
}

void TextSearch::on_btnBack_clicked()
{
    find( ui->textSearch->text(), false, QTextDocument::FindBackward );
}

void TextSearch::on_btnClose_clicked()
{
    close();
}

/**
Called when search string changes.
*/
void TextSearch::on_textSearch_textChanged()
{
    find( ui->textSearch->text(), true );
}

/**
Searches occurences of a string in the _textEdit.
*/
void TextSearch::find(const QString &search, bool fromStart, QTextDocument::FindFlags options)
{
    if (!_textEdit || search.isEmpty())
        return;

    if ( fromStart )
    {
        QTextCursor crsr = _textEdit->textCursor();
        crsr.setPosition(0);
        _textEdit->setTextCursor(crsr);
    }

    _textEdit->find( search, options );
}

void TextSearch::showEvent(QShowEvent *e)
{
    e->accept();
    ui->textSearch->setFocus();

    QWidget::showEvent(e);
}
