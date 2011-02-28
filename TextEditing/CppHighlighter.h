#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H

#include "AbstractHighlighter.h"

class CppHighlighter : public AbstractHighlighter
{
    Q_OBJECT
public:
    explicit CppHighlighter(QTextDocument *parent = 0);

protected:
    virtual void setupHighlightRules();
    virtual void setupHighlightBlocks();

};

#endif // CPPHIGHLIGHTER_H
