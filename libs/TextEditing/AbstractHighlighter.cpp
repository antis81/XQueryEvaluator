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

#include "AbstractHighlighter.h"


/**
Called by the inherited class to create a highlighter instance.
*/
AbstractHighlighter::AbstractHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
}

/**
Initializes the highlighter with highlight rules. Should be called within the constructor of the inherited class.

@see setupHighlightRules
@see setupHighlightBlocks
*/
void AbstractHighlighter::init ()
{
    // call the virtual highlight init methods in the right order
    setupHighlightRules();
    setupHighlightBlocks();
}

/**
Adds a highlight rule for a list of regular expression patterns. Sets the rules text format.
*/
void AbstractHighlighter::addHighlightRule(const QStringList &patterns, const QTextCharFormat &format)
{
    // Set the text format ...
    HighlightingRule rule;
    rule.format = format;

    // ... for the following Patterns
    foreach (const QString &pattern, patterns)
    {
        QRegExp rx( pattern );
        rx.setMinimal(true);
        rule.patterns.append(rx);
    }

    // Append to rule list
    _highlightingRules.append(rule);
}

/**
Adds a highlight rule for one regular expression pattern. Sets the rule's text format.
*/
void AbstractHighlighter::addHighlightRule(const QString &pattern, const QTextCharFormat &format)
{
    // Set the text format ...
    HighlightingRule rule;
    rule.format = format;

    QRegExp rx(pattern);
    rx.setMinimal(true);
    rule.patterns.append(rx);

    // Append to rule list
    _highlightingRules.append(rule);
}

/**
Adds a list of regular expression patterns as group of highlight rules using the same specified foreground color.
*/
void AbstractHighlighter::addHighlightRule(const QStringList & patterns, const QColor & color)
{
    QTextCharFormat f;
    f.setForeground(color);
    addHighlightRule(patterns, f);
}

/**
Adds a highlight rule for one regular expression pattern as highlight rule using the same specified color.
*/
void AbstractHighlighter::addHighlightRule(const QString & pattern, const QColor & color)
{
    QTextCharFormat f;
    f.setForeground(color);
    addHighlightRule(pattern, f);

}

/**
Adds a complete highlight block definition.
*/
void AbstractHighlighter::addHighlightBlock(const AbstractHighlighter::HighlightBlock &block)
{
    _blocks.append( block );
}

/**
Called, when Qt wants to highlight a QTextBlock within a QTextDocuement.
*/
void AbstractHighlighter::highlightBlock(const QString &text)
{
    if ( (previousBlockState() < 0) || (currentBlockState() < 0) )
        highlightNormalText(text);

    setCurrentBlockState(-1);
    highlightTextBlock(text);
}

/**
Highlights text outside a text block using previously defined highlight rules.
*/
void AbstractHighlighter::highlightNormalText(const QString &text)
{
    for (int i=0; i < _highlightingRules.count(); ++i)
    {
        const HighlightingRule & rule = _highlightingRules.at(i);

        // Search for matching patterns in current textblock
        for (int j=0; j < rule.patterns.count(); ++j)
        {
            const QRegExp & pattern = rule.patterns.at(j);
            int pos = 0;
            while ( (pos = pattern.indexIn(text, pos)) > -1)
            {
                const int length = pattern.matchedLength();
                setFormat(pos, length, rule.format);
                pos += length;
            }
        }
    }
}

/**
Highlights a text block (e.g. multi line comments) using previously defined HighlightBlock rules.
*/
void AbstractHighlighter::highlightTextBlock(const QString &text)
{
    // highlight text blocks (like comments etc.)
    for ( int i=0; i < _blocks.count(); ++i )
    {
        colorBlock( i, text, _blocks.at(i) );
    }
}


/**
Highlights whole text blocks between start and end tags. E.g. an XML comment looks like: "<!-- ... text ... -->").
*/
void AbstractHighlighter::colorBlock(int blockState, const QString &text, const HighlightBlock &blockFormat)
{
    // Find start position of highlight block
    int start = 0;

    if (previousBlockState() != blockState)
        start = blockFormat.startExp.indexIn(text); // Neuer Block

    // When start pattern found or the current block is within the textblock, highlight it
    while (start > -1)
    {
        int end = blockFormat.endExp.indexIn(text, start);
        int length = 0;

        // Search for the end pattern in the current block
        if (end == -1) {
            // end pattern not found (=> remain in this block state)
            length = text.length() - start;
            setCurrentBlockState(blockState);
        } else {
            // end pattern found (=> free block state)
            length = end - start + blockFormat.endExp.matchedLength();
            setCurrentBlockState(-1);
        }

        setFormat(start, length, blockFormat.textFormat); // einfaerben

        // Search for further textblocks in the current block
        if (end > -1)
            start = blockFormat.startExp.indexIn(text, end + blockFormat.endExp.matchedLength());
        else
            start = -1;
    }
}
