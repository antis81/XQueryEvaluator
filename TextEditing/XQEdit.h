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

#ifndef XQEDIT_H
#define XQEDIT_H

#include <QtGui/QPlainTextEdit>
#include <QtCore/QString>

#include "XQueryHighlighter.h"


class QCompleter;

/**
A text edit field providing XML/XSLT/XQuery highlighting and autocompletion.
*/
class XQEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit XQEdit(QWidget *parent = 0);
    virtual ~XQEdit();

    QCompleter *completer() const { return _completer; }
    void setCompleter(QCompleter *completer);

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void insertCompletion(const QString &completion);

private:
    XQueryHighlighter   _xqueryHighlighter;
    QCompleter *        _completer;
    QList<int>          _ignoreKeysOnCpl;

    const QString       _eow; //!< End Of Word

    void setupKeys();

    QString textUnderCursor() const;

    QList<int>      _completionKeys;
};

#endif // XQEDIT_H
