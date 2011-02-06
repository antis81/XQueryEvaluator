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

#ifndef TEXTEDITMETABORDER_H
#define TEXTEDITMETABORDER_H


#include <QtGui/QPlainTextEdit>
#include <QtGui/QPaintEvent>
#include <QtCore/QSet>
#include <QtGui/QTextBlock>

class TextEditMetaBorder : public QWidget
{
    Q_OBJECT

public:

    enum eTextBlockOption
    {
        etboBreakPoint= 0x01,
        etboBookmark  = 0x02
    };

    explicit TextEditMetaBorder(QPlainTextEdit *doc, QWidget *parent=0);
    ~TextEditMetaBorder();

//    bool isBreakpoint(int line) { return _breakpoints.contains(line); }
//    QSet<int> breakpoints() const { return _breakpoints; }

//    void setDebugMode(bool bnew) { update(); _bDebugMode=bnew; }
//    bool debugMode() const { return _bDebugMode; }

    void showEvent(QShowEvent *e);
    void paintEvent ( QPaintEvent * event ) ;
    void drawLineNumbers(QPainter & painter);
//    void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void breakpointChanged(bool bSet);

private:
//    bool             _bDebugMode;
//    QSet<int>        _breakpoints;
    QPlainTextEdit *    _document;

//    struct LineInfo
//    {
//        int        line;
//        QTextBlock textblock;
//        QRectF     rect;
//    };

//    QList<LineInfo> _visibleLineCache;
};

#endif // TEXTEDITMETABORDER_H
