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

/**
Automatically indents the QTextDocument doc by using indent/outdent regular expressions.

@todo The expressions are currently hardcoded and used to indent XQuery, XML and XSLT documents.
*/
void AutoIndent::indentDocument(QTextDocument *doc)
{
    int level = 0;
    QList<QRegExp> indentRules;
    indentRules.append( QRegExp( "<\\w+[^>]*[^/]>" ) );
    indentRules.append( QRegExp( "\\{" ) );
    //indentRules.append( QRegExp( "(for|return)" ) );
    QRegExp outdentRule( "(\\}|</[^>])" );

    QString tNew;
    for ( QTextBlock block = doc->firstBlock(); block.isValid(); block = block.next() )
    {
        QString t = block.text().trimmed();

        const int indentCount = matchCount( indentRules, t );
        level += indentCount;

        const int outdentCount = matchCount( outdentRule, t );
        level -= outdentCount;

        if (level < 0)
            level = 0;

        if ( !t.isEmpty() ) // don't indent empty lines
        {
            const bool foundIndent = (indentCount > 0);
            // const bool foundOutdent = (outdentCount > 0);

            if ( foundIndent )
                indent(t, level - indentCount + outdentCount); // don't indent start tag (stays on current level)
            else
                indent(t, level); // we are in an indent block
        }

        // line break when not the last line
        if ( block.next().isValid() )
            t += "\n";

        // add to result
        tNew += t;
        //tNew.prepend( QString("[%1 , %2] : ").arg(indentCount, 2).arg(outdentCount, 2);
    }

    doc->setPlainText(tNew);
}

/**
Counts the occurence of regExp match in text.

@return The matched count is returned.
*/
int AutoIndent::matchCount( const QRegExp &regExp, const QString &text ) const
{
    int count = 0;

    int pos = 0;
    while ( (pos = regExp.indexIn(text, pos)) > -1)
    {
        ++count;

        const int length = regExp.matchedLength();
        pos += length;
    }

    return count;
}

/**
Counts the occurences of a list of regular expressions. Each expression is evaluated independent of other expressions.

@return The total match count is returned.
*/
int AutoIndent::matchCount( const QList<QRegExp> &expressions, const QString &text ) const
{
    int count = 0;

    for (int i=0; i < expressions.count(); ++i)
    {
        count += matchCount(expressions.at(i), text);
    }

    return count;
}

/**
Indents a single text line by the given level.
*/
void AutoIndent::indent(QString &text, int level)
{
    if (level == 0)
        return;

    int spaceCount = level * 4;
    for (int i=0; i < spaceCount; ++i)
        text.prepend(QChar(' '));
}
