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

#ifndef TEXTEDITMETABORDER_H
#define TEXTEDITMETABORDER_H

#include <QtGui/QWidget>

class TextEditBase;


/**
A meta border that can be combined with a QPlainTextEdit to display line numbers.
*/
class TextEditMetaBorder : public QWidget
{
    Q_OBJECT
public:
    explicit TextEditMetaBorder(TextEditBase *editor);
    virtual ~TextEditMetaBorder();

    QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    TextEditBase *      _editor;

};

#endif // TEXTEDITMETABORDER_H
