#include "CppHighlighter.h"

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
