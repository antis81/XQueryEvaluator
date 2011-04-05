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

#include "XmlHighlighter.h"


/**
Creates an XML highlighter instance. The highlighter is used on the parent QTextDocument.
*/
XmlHighlighter::XmlHighlighter(QTextDocument *parent)
	: AbstractHighlighter(parent)
{
	init();
}

void XmlHighlighter::setupHighlightRules()
{
    // XML Elemente (Tags)
    QStringList	tagPatterns;
    tagPatterns
            << "<[^/>\\s]*[\\s>]"
            << "</[^>/]+>"
            << "<[^>/]+/>"
               ;
    QTextCharFormat xmlTagFormat;  xmlTagFormat.setForeground(Qt::darkRed);
    addHighlightRule(tagPatterns, xmlTagFormat);

    // XML Attributes
    QTextCharFormat xmlAttrFormat; xmlAttrFormat.setForeground(Qt::red);
    addHighlightRule( QString( "\\b\\w+\\s*=\\s*[\"']" ), xmlAttrFormat );

    // special tag chars
    QTextCharFormat keySignFormat; keySignFormat.setForeground(Qt::blue);
    addHighlightRule( QString( "[<>]|=[\"']|[\"']\\s*|</|/>" ), keySignFormat );
}

void XmlHighlighter::setupHighlightBlocks()
{
    // XML processing instruction <? ... ?>
    HighlightBlock xmlProcessing;
    xmlProcessing.textFormat.setForeground(QColor(0x40,0x40,0x80));
    xmlProcessing.startExp	= QRegExp("<\\?");
    xmlProcessing.startExp.setMinimal(true);
    xmlProcessing.endExp	= QRegExp("\\?>");
    xmlProcessing.endExp.setMinimal(true);
    addHighlightBlock( xmlProcessing );

    // XML CData <![CDATA[ ... ]]>
    HighlightBlock xmlCData;
    xmlCData.startExp	= QRegExp("<!\\[CDATA\\[");
    xmlCData.startExp.setMinimal(true);
    xmlCData.endExp= QRegExp("\\]\\]>");
    xmlCData.endExp.setMinimal(true);
    xmlCData.textFormat.setForeground(QColor(0xFF,0x80,0x40));
    addHighlightBlock( xmlCData );

    // XML comment <!-- ... -->
    HighlightBlock xmlCommentBlock;
    xmlCommentBlock.startExp = QRegExp("<!--");
    xmlCommentBlock.startExp.setMinimal(true);
    xmlCommentBlock.endExp = QRegExp("-->");
    xmlCommentBlock.endExp.setMinimal(true);
    xmlCommentBlock.textFormat.setForeground(Qt::gray);
    addHighlightBlock( xmlCommentBlock );
}
