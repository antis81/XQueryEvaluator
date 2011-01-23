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

#include "TextEditMetaBorder.h"

#include <QtGui/QPainter>
#include <QtGui/QScrollBar>
#include <QtGui/QTextBlock>
#include <QtGui/QAbstractTextDocumentLayout>


TextEditMetaBorder::TextEditMetaBorder(QPlainTextEdit *doc, QWidget *parent)
    : QWidget(parent)
    , _document(doc)
//, _bDebugMode(false)
{
    const QFontMetrics &fm = fontMetrics();

    _iBreakpointWidth = fm.maxWidth() * 1.5;

    // 4 line numbers + space + breakpoint
    setMinimumWidth(fm.maxWidth()*6);

    connect( _document->document()->documentLayout(), SIGNAL( update(const QRectF &) ),
        this, SLOT( update() ) );
    connect( _document->verticalScrollBar(), SIGNAL(valueChanged(int) ),
        this, SLOT( update() ) );
}

TextEditMetaBorder::~TextEditMetaBorder()
{
}


void TextEditMetaBorder::paintEvent ( QPaintEvent * event )
{
    QPainter p(this);
    p.save();

    drawLineNumbers(p);

    p.restore();
}

void TextEditMetaBorder::drawLineNumbers(QPainter & painter)
{
    const int scrollOffset = _document->verticalScrollBar()->value();
    const qreal visibleBottom = _document->viewport()->height() + scrollOffset;
    const QFontMetrics &fm = _document->fontMetrics();

    for ( QTextBlock block = _document->document()->begin();
        block.isValid(); block = block.next() )
    {

        const int count = block.blockNumber() +1;

        // don't draw invisible lines
        if ( count-1 < scrollOffset )
            continue;

        int linePos = fm.height() * (count - scrollOffset);
        if ( count > visibleBottom )
            break;

        const QString txt = QString("%1").arg( count );
        painter.drawText( width() - fm.width(txt) -3 , linePos, txt );
    }
}

void TextEditMetaBorder::mousePressEvent ( QMouseEvent * event )
{
    if (event->button() & Qt::LeftButton)
    {
        for ( int i = 0 ; i < _visibleLineCache.count() ; i++ )
        {
            if ( _visibleLineCache[i].rect.contains( event->pos() ) )
            {
                QTextBlock block=_visibleLineCache[i].textblock;

                if ( block.userState() == -1 )
                    block.setUserState(0);

                block.setUserState(block.userState() ^ etboBookmark );
                /*int line=_visibleLineCache[i].line ;
                if ( isBreakpoint( line ) )
                {
                _breakpoints.remove(line);
                }else
                {
                _breakpoints.insert(line);
                }
                */
                update();
            }
        } // for
    }
}

void TextEditMetaBorder::showEvent(QShowEvent *e)
{
    e->accept();

    if (_document->font() != font())
        setFont(_document->font());
}
