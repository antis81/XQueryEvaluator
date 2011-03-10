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

#ifndef XQEDITOR_H
#define XQEDITOR_H

#include <QtGui/QWidget>

#include "AutoIndent.h"


class XQEdit;
class QAbstractItemModel;

/**
Editor widget to edit text in an XQEdit. Adds line numbers.
*/
class XQEditor : public QWidget
{
    Q_OBJECT

public:
    explicit XQEditor(QWidget *parent = 0);
    ~XQEditor();

    QString xqText() const;
    void setXQText(const QString &text);

    bool modified() const;
    void setModified(bool modified);

    void autoIndent();

    XQEdit * textEdit() const;

private slots:
    void documentModified(bool modified);

private:
    XQEdit *    _textQuery;
    bool        _modified;

    QAbstractItemModel * modelFromFile(QString fileName);
};

#endif // XQEDITOR_H
