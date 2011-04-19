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

#ifndef XQUERYHIGHLIGHTER_H
#define XQUERYHIGHLIGHTER_H

#include "XmlHighlighter.h"

#include <QtCore/QList>


/**
Highlights combined XQuery/XSLT and XML QTextDocuments.
*/
class XQueryHighlighter : public XmlHighlighter
{
public:
    explicit XQueryHighlighter(QTextDocument *parent =0);

private:
    void setupHighlightRules();
    void setupHighlightBlocks();

};

#endif // XQUERYHIGHLIGHTER_H