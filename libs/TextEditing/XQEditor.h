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

#ifndef XQEDITOR_H
#define XQEDITOR_H

#include <QtCore/QString>

#include "TextEditBase.h"
#include "XQueryHighlighter.h"
#include "AutoIndent.h"


class QCompleter;
class QAbstractItemModel;

/**
@brief Represents a text editor providing XML/XSLT/XQuery highlighting and autocompletion.
*/
class XQEditor : public TextEditBase
{
    Q_OBJECT
public:
    explicit XQEditor(QWidget *parent = 0);
    virtual ~XQEditor();

    QCompleter *completer() const { return _completer; }
    void setCompleter(QCompleter *completer);

    void autoIndent();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void insertCompletion(const QString &completion);

private:
    XQueryHighlighter   _xqueryHighlighter;
    QCompleter *        _completer;
    QList<int>          _ignoreKeysOnCpl;

    const QString       _eow; //!< End Of Word

    QAbstractItemModel * modelFromFile(QString fileName);

    void setupKeys();

    QString textUnderCursor() const;
};

#endif // XQEDITOR_H
