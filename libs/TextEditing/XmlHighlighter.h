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

#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include "AbstractHighlighter.h"

#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <QtGui/QTextCharFormat>
#include <QtXml/QDomDocument>

class QTextDocument;

/**
An XML text highlighter for QTextDocuments.
*/
class XmlHighlighter : public AbstractHighlighter
{
    Q_OBJECT
public:
    explicit XmlHighlighter(QTextDocument *parent = 0);

protected:
    void setupHighlightRules();
    void setupHighlightBlocks();
};

#endif // LBXMLHIGHLIGHTER_H
