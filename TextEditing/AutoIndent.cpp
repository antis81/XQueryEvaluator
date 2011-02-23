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

#include "AutoIndent.h"

#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QTextLayout>


AutoIndent::AutoIndent()
{
}

void AutoIndent::indentDocument(QTextDocument *doc)
{
    int level = 0;
    QRegExp indentRule( "(\\{|<[^/][^>]*[^/]>)" );
    QRegExp outdentRule( "(\\}|</[^>]*[^/]>)" );

    QString tNew;
    for ( QTextBlock block = doc->firstBlock(); block.isValid(); block = block.next() )
    {
        QString t = block.text().trimmed();
        const bool foundIndent = indentRule.indexIn( t ) != -1;
        const bool foundOutdent = outdentRule.indexIn( t ) != -1;
        if ( foundIndent )
        {
            ++level;
        }

        // outdent on end tag
        if ( foundOutdent )
            --level;

        if (level < 0)
            level = 0;

        if ( !t.isEmpty() ) // don't indent empty lines
        {
            if (foundIndent && !foundOutdent)
                indent(t, level-1); // don't indent start tag (stays on current level)
            else
                indent(t, level); // we are in an indent block
        }

        // line break when not the last line
        if ( block.next().isValid() )
            t += "\n";

        tNew += t; // add to result
    }

    doc->setPlainText(tNew);
}

/**
Indents a single text line.
*/
void AutoIndent::indent(QString &text, int level)
{
    if (level == 0)
        return;

    int spaceCount = level * 4;
    for (int i=0; i < spaceCount; ++i)
        text.prepend(QChar(' '));
}
