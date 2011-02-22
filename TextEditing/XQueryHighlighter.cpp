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
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // textformat for each highlight case
    QTextCharFormat keySignFormat; keySignFormat.setForeground(Qt::blue);
    QTextCharFormat keyWordFormat; keyWordFormat.setForeground(Qt::blue);
    QTextCharFormat xmlTagFormat; xmlTagFormat.setForeground(Qt::darkRed);
    QTextCharFormat xmlAttrFormat; xmlAttrFormat.setForeground(Qt::red);
    QTextCharFormat varNameFormat; varNameFormat.setForeground(Qt::darkMagenta);

    // XML Elemente (Tags)
    QString	tagPattern(
                "<[^/>\\s]*[\\s>]"
                "|</[^>/]+>"
                "|<[^>/]+/>"
                );
    addHighlightingRule(tagPattern, xmlTagFormat);

    QString keyWordPattern(
                "\\bdeclare\\b"
                "|\\bfunction\\b"
                "|\\bif\\b"
                "|\\bthen\\b"
                "|\\belse\\b"
                "|\\bfor\\b"
                "|\\bat\\b"
                "|\\bin\\b"
                "|\\blet\\b"
                "|\\bwhere\\b"
                "|\\breturn\\b"
                );
    addHighlightingRule( keyWordPattern, keyWordFormat );

    // XML Attribute
    addHighlightingRule( QString( "\\b\\w+\\s*=\\s*[\"']" ), xmlAttrFormat );

    addHighlightingRule( QString( "\\b?\\$([A-Za-z0-9_]+)\\b" ), varNameFormat );

    // Tag Anfangs-/Endezeichen
    addHighlightingRule( QString( "[<>]|=[\"']|[\"']\\s*|</|/>" ), keySignFormat );

    setupHighlightBlocks();
}

/**
Setup highlight blocks (e.g. comment blocks).
*/
void XQueryHighlighter::setupHighlightBlocks()
{
    // XML processing instruction <? ... ?>
    HighlightBlock xmlProcessing;
    xmlProcessing.textFormat.setForeground(QColor(0x40,0x40,0x80));
    xmlProcessing.startExp = QRegExp("<\\?");
    xmlProcessing.startExp.setMinimal(true);
    xmlProcessing.endExp	= QRegExp("\\?>");
    xmlProcessing.endExp.setMinimal(true);
    _blocks.insert("XMLProcessingInstruction", xmlProcessing);

    // XML CData <![CDATA[ ... ]]>
    HighlightBlock xmlCData;
    xmlCData.startExp	= QRegExp("<!\\[CDATA\\[");
    xmlCData.startExp.setMinimal(true);
    xmlCData.endExp= QRegExp("\\]\\]>");
    xmlCData.endExp.setMinimal(true);
    xmlCData.textFormat.setForeground(QColor(0xFF,0x80,0x40));
    _blocks.insert("XMLCData", xmlCData);

    // XML comment <!-- ... -->
    HighlightBlock xmlCommentBlock;
    xmlCommentBlock.startExp = QRegExp("<!--");
    xmlCommentBlock.startExp.setMinimal(true);
    xmlCommentBlock.endExp = QRegExp("-->");
    xmlCommentBlock.endExp.setMinimal(true);
    xmlCommentBlock.textFormat.setForeground(Qt::gray);
    _blocks.insert("XMLComment", xmlCommentBlock);

    // XQuery comment (: ... :)
    HighlightBlock xqComment;
    xqComment.startExp = QRegExp("\\(:");
    xqComment.startExp.setMinimal(true);
    xqComment.endExp = QRegExp(":\\)");
    xqComment.endExp.setMinimal(true);
    xqComment.textFormat.setForeground(Qt::darkGreen);
    _blocks.insert("XQueryComment", xqComment);
}


XQueryHighlighter::~XQueryHighlighter()
{
}


/**
 * Adds a highlighting rule for multiple patterns (must be RegExp's).
 */
void XQueryHighlighter::addHighlightingRule(const QStringList &patterns, const QTextCharFormat &format)
{
    // Set the text format ...
    HighlightingRule rule;
    rule.format = format;

    // ... for the following Patterns
    for (int i=0; i < patterns.count(); ++i)
    {
        QRegExp rx(patterns[i]);
        rx.setMinimal(true);
        rule.patterns.append(rx);
    }

    // Append to rule list
    _highlightingRules.append(rule);
}

/**
 * Adds a highlighting rule for one pattern (must be RegExp).
 */
void XQueryHighlighter::addHighlightingRule(const QString &pattern, const QTextCharFormat &format)
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

void XQueryHighlighter::highlightBlock(const QString &text)
{
    //qDebug() << "BEFORE - prev: " << previousBlockState() << "; curr: " << currentBlockState();

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

    setCurrentBlockState(0);

    colorBlock(1, text, _blocks.value("XMLProcessingInstruction"));	// XML processing instruction
    colorBlock(2, text, _blocks.value("XMLCData"));         // XML CDATA
    colorBlock(3, text, _blocks.value("XMLComment"));       // XML comment
    colorBlock(4, text, _blocks.value("XQueryComment"));    // XQuery comment
}


/**
 * Färbt Textbloecke zwischen zwei Steuerzeichen ein (z.B. ein Kommentar: "<!-- ... Text ... -->").
 */
void XQueryHighlighter::colorBlock(int blockState, const QString &text, const HighlightBlock &blockFormat)
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
