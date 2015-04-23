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

#ifndef TEXTEDITBASE_H
#define TEXTEDITBASE_H

#include <QtGui/QPlainTextEdit>

class TextEditMetaBorder;


class TextEditBase : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextEditBase(QWidget *parent = 0);
    virtual ~TextEditBase();

    void metaBorderPaintEvent(QPaintEvent *e);
    int metaBorderWidth() const;

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateMetaBorderWidth(int newBlockCount);
    void updateMetaBorder(const QRect &, int);

    void highlightCurrentLine();

private:
    TextEditMetaBorder *    _metaBorder;
};

#endif // TEXTEDITBASE_H
