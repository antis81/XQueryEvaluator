#include "CppHighlighter.h"

CppHighlighter::CppHighlighter(QTextDocument *parent)
{
    init();
}

void CppHighlighter::setupHighlightRules()
{
    // declaration keywords
    addHighlightingRule( "\\b(protected|private|public|signals|slots|class)\\b", Qt::darkMagenta );

    // keywords
    addHighlightingRule(
                "\\b(for|if|while|static|const|return|void|virtual|explicit|typedef|emit|Q_OBJECT)\\b"
                , Qt::blue );

    // types
    addHighlightingRule( "\\b(char|int|unsigned|float|double|bool|real|qreal|qint)\\s", Qt::darkBlue );

    // one line comments
    addHighlightingRule( "([\\s,;()]//.*$)", Qt::darkGreen );

    // strings
    addHighlightingRule( "\".*[^\\\\]\"", Qt::darkRed );
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
