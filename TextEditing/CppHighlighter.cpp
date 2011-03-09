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

#include "CppHighlighter.h"


/**
Creates a C++ highlighter. The highlighter is used on the parent QTextDocument.
*/
CppHighlighter::CppHighlighter(QTextDocument *parent)
    : AbstractHighlighter(parent)
{
    init();
}

void CppHighlighter::setupHighlightRules()
{
    // declaration keywords
    addHighlightRule( "\\b(protected|private|public|signals|slots|class)\\b", Qt::darkMagenta );

    // keywords
    addHighlightRule(
                "\\b(for|if|while|static|const|return|void|virtual|explicit|typedef|emit|Q_OBJECT)\\b"
                , Qt::blue );

    // types
    addHighlightRule( "\\b(char|int|unsigned|float|double|bool|real|qreal|qint)\\s", Qt::darkBlue );

    // one line comments
    addHighlightRule( "([\\s,;()]//.*$)", Qt::darkGreen );

    // strings
    addHighlightRule( "\".*[^\\\\]\"", Qt::darkRed );
}

void CppHighlighter::setupHighlightBlocks()
{
    AbstractHighlighter::HighlightBlock block;
    block.startExp = QRegExp( "/\\*" );
    block.startExp.setMinimal(true);
    block.endExp = QRegExp( "\\*/" );
    block.endExp.setMinimal(true);
    block.textFormat.setForeground(Qt::darkGreen);
    addHighlightBlock( block );
}
