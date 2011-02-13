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

#include "XmlHighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	//_highlightBlockCalls = 0;


	HighlightingRule rule;

	// Textformate fuer die einzelnen Highlightfaelle
	_xmlHeadFormat.setForeground(QColor(0x40,0x40,0x80));
	_keySignFormat.setForeground(Qt::blue);
	_commentFormat.setForeground(Qt::gray);
	_cdataFormat.setForeground(QColor(0xFF,0x80,0x40));
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

	// XML Attribute
	QStringList	attrPatterns;
	attrPatterns	<< " [^=</>]*(=[\"'])";
	addHighlightingRule(attrPatterns, _xmlAttrFormat);

	// Tag Anfangs-/Endezeichen
	QStringList charPatterns;
	charPatterns	<< "[<>]"
					<< "=[\"']"
					<< "[\"']\\s*"
					<< "</" << "/>"
					;
	addHighlightingRule(charPatterns, _keySignFormat);

	// XML Headerstring <? ... ?>
	_headerStartExp	= QRegExp("<\\?");
	_headerStartExp.setMinimal(true);
	_headerEndExp	= QRegExp("\\?>");
	_headerEndExp.setMinimal(true);

	// CDATA STRING <![CDATA[ ]]>
	_cdataStartExp	= QRegExp("<!\\[CDATA\\[");
	_cdataStartExp.setMinimal(true);
	_cdataEndExp	= QRegExp("\\]\\]>");
	_headerEndExp.setMinimal(true);

	// XML Kommentare <!-- ... -->
	_commentStartExp = QRegExp("<!--");
	_commentStartExp.setMinimal(true);
	_commentEndExp = QRegExp("-->");
	_commentEndExp.setMinimal(true);

	//qDebug() << "Number of highlighting rules: " << _highlightingRules.count();
}

XmlHighlighter::~XmlHighlighter()
{
    qDeleteAll(_highlightingRules);
    _highlightingRules.clear();
}


/**
 * Adds a highlighting rule for multiple patterns (must be RegExp's).
 */
void XmlHighlighter::addHighlightingRule(const QStringList &patterns, const QTextCharFormat &format)
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


void XmlHighlighter::highlightBlock(const QString &text)
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

    colorBlock(1, text, _headerStartExp, _headerEndExp, _xmlHeadFormat);	// XML Header
    colorBlock(2, text, _cdataStartExp, _cdataEndExp, _cdataFormat);	    // CDATA einfaerben
    colorBlock(3, text, _commentStartExp, _commentEndExp, _commentFormat);	// Kommentare einfaerben
}


/**
 * Colors text blocks between start and end tags. E.g. a comment like "<!-- ... text ... -->").
 */
void XmlHighlighter::colorBlock(int blockState, const QString &text, const QRegExp &startExp,
                                  const QRegExp &endExp, const QTextCharFormat &fmt)
{
	// Find start position of highlight block
	int start = 0;
	if (previousBlockState() != blockState)
		start = startExp.indexIn(text); // Neuer Block

	// When start pattern found or the current block is within the textblock, highlight it
	while (start > -1)
	{
		int end = endExp.indexIn(text, start);
		int length = 0;

		// Search for the end pattern in the current block
		if (end == -1) {
			// end pattern not found (=> remain in this block state)
			length = text.length() - start;
			setCurrentBlockState(blockState);
		} else {
			// end pattern found (=> free block state)
			length = end - start + endExp.matchedLength();
			setCurrentBlockState(-1);
		}

		setFormat(start, length, fmt); // einfaerben

        // Search for further textblocks in the current block
        if (end > -1)
            start = startExp.indexIn(text, end + endExp.matchedLength());
        else
            start = -1;
    }
}
