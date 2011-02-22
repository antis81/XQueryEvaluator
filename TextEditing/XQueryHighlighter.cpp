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

    // Textformate fuer die einzelnen Highlightfaelle
    _keySignFormat.setForeground(Qt::blue);
    _xmlTagFormat.setForeground(Qt::darkRed);
    _xmlAttrFormat.setForeground(Qt::red);

    // XML Elemente (Tags)
    QStringList	tagPatterns;
    tagPatterns
            << "<[^/>\\s]*[\\s>]"
            << "</[^>/]+>"
            << "<[^>/]+/>"
               ;
    addHighlightingRule(tagPatterns, _xmlTagFormat);

    _keyWordFormat.setForeground(Qt::blue);
    QStringList keyWordPatterns;
    keyWordPatterns
            << "\\bdeclare\\b"
            << "\\bfunction\\b"
            << "\\bif\\b"
            << "\\bthen\\b"
            << "\\belse\\b"
            << "\\bfor\\b"
            << "\\bin\\b"
            << "\\blet\\b"
            << "\\bwhere\\b"
            << "\\breturn\\b"
               ;

    addHighlightingRule(keyWordPatterns, _keyWordFormat);

    // XML Attribute
    QStringList	attrPatterns;
    attrPatterns
            << "\\b\\w+\\s*=\\s*[\"']"
               ;
    addHighlightingRule(attrPatterns, _xmlAttrFormat);

    _varNameFormat.setForeground(Qt::darkMagenta);
    QStringList varNamePatterns;
    varNamePatterns
            << "\\b?\\$([A-Za-z0-9_]+)\\b"
               ;
    addHighlightingRule(varNamePatterns, _varNameFormat);

    // Tag Anfangs-/Endezeichen
    QStringList charPatterns;
    charPatterns	<< "[<>]"
			<< "=[\"']"
			<< "[\"']\\s*"
			<< "</" << "/>"
                           ;					;
    addHighlightingRule(charPatterns, _keySignFormat);

    // XML processing instruction <? ... ?>
    HighlightBlock xmlProcessing;
    xmlProcessing.textFormat.setForeground(QColor(0x40,0x40,0x80));
    xmlProcessing.startExp	= QRegExp("<\\?");
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
    qDeleteAll(_highlightingRules);
    _highlightingRules.clear();
}


/**
 * Adds a highlighting rule for multiple patterns (must be RegExp's).
 */
void XQueryHighlighter::addHighlightingRule(const QStringList &patterns, const QTextCharFormat &format)
{
    // Set the text format ...
    HighlightingRule *rule = new HighlightingRule;
    rule->format = format;

    // ... for the following Patterns
    for (int i=0; i < patterns.count(); i++)
    {
        //QString pattern = patterns[i];
        QRegExp *rx = new QRegExp(patterns[i]);
        rx->setMinimal(true);
        rule->patterns.append(rx);
    }

    // Append to rule list
    _highlightingRules.append(rule);
}


void XQueryHighlighter::highlightBlock(const QString &text)
{
    //qDebug() << "BEFORE - prev: " << previousBlockState() << "; curr: " << currentBlockState();

    //if ( (previousBlockState() == -1) || (previousBlockState() == 0) )
    //{
    for (int i=0; i < _highlightingRules.count(); i++)
    {
        const HighlightingRule *rule = _highlightingRules[i];

        // Search for matching patterns in current textblock
        for (int j=0; j < rule->patterns.count(); j++)
        {
            const QRegExp *pattern = rule->patterns[j];
            //int pos = searchPos;
            int pos = 0;
            while ( (pos = pattern->indexIn(text, pos)) > -1)
            {
                const int length = pattern->matchedLength();
                setFormat(pos, length, rule->format);
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
