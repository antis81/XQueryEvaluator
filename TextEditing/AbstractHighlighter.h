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

#ifndef ABSTRACTHIGHLIGHTER_H
#define ABSTRACTHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

class AbstractHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:

    struct HighlightBlock
    {
        QRegExp startExp;
        QRegExp endExp;
        QTextCharFormat textFormat;
    };

    struct HighlightingRule
    {
        QList<QRegExp>      patterns;
        QTextCharFormat     format;
    };

    explicit AbstractHighlighter(QTextDocument *parent = 0);

protected:
    void addHighlightRule(const QStringList & patterns, const QTextCharFormat & format);
    void addHighlightRule(const QString & pattern, const QTextCharFormat & format);
    void addHighlightRule(const QStringList & patterns, const QColor & color);
    void addHighlightRule(const QString & pattern, const QColor & color);

    void addHighlightBlock(const AbstractHighlighter::HighlightBlock &block);

    void init();

    virtual void setupHighlightRules() = 0;
    virtual void setupHighlightBlocks() = 0;

private:
    QList<HighlightingRule>     _highlightingRules;
    QList<HighlightBlock>       _blocks;

    void highlightBlock(const QString &text);
    void highlightNormalText(const QString &text);
    void highlightTextBlock(const QString &text);
    void colorBlock(int blockState, const QString &text, const HighlightBlock &blockFormat);
};

#endif // ABSTRACTHIGHLIGHTER_H
