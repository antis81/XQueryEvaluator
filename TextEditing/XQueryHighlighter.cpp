/**
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

#include "XQueryHighlighter.h"

XQueryHighlighter::XQueryHighlighter(QTextDocument *parent)
    : XmlHighlighter(parent)
{
    HighlightingRule rule;

    // textformat for each highlight case
    QString keyWordPattern( "\\b(declare|function|if|then|else|for|at|in|let|where|return)\\b" );
    QTextCharFormat keyWordFormat; keyWordFormat.setForeground(Qt::blue);
    addHighlightingRule( keyWordPattern, keyWordFormat );

    QTextCharFormat varNameFormat; varNameFormat.setForeground(Qt::darkMagenta);
    addHighlightingRule( QString( "\\b?\\$([A-Za-z0-9_]+)\\b" ), varNameFormat );

    setupHighlightBlocks();
}

/**
Setup highlight blocks (e.g. comment blocks).
*/
void XQueryHighlighter::setupHighlightBlocks()
{
    // XQuery comment (: ... :)
    HighlightBlock xqComment;
    xqComment.startExp = QRegExp("\\(:");
    xqComment.startExp.setMinimal(true);
    xqComment.endExp = QRegExp(":\\)");
    xqComment.endExp.setMinimal(true);
    xqComment.textFormat.setForeground(Qt::darkGreen);
    _blocks.append( xqComment );
}
