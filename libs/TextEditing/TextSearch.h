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

#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H

#include <QtWidgets/QWidget>
#include <QtGui/QTextDocument>

class QPlainTextEdit;

namespace Ui {
    class TextSearch;
}


/**
@brief
Represents a generic text search dialog.
*/
class TextSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TextSearch(QWidget *parent = 0);
    ~TextSearch();

    QPlainTextEdit * textEdit();
    void setTextEdit( QPlainTextEdit * edit );

protected:
    virtual void showEvent(QShowEvent *);

private slots:
    void on_btnForward_clicked();
    void on_btnBack_clicked();
    void on_btnClose_clicked();

    void on_textSearch_textChanged();

private:
    Ui::TextSearch *    ui;

    QPlainTextEdit *    _textEdit;

    void find( const QString &search, bool fromStart =false, QTextDocument::FindFlags options =0 );
};

#endif // TEXTSEARCH_H
