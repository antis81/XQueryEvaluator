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
    _visibleLineCache.clear();

    QAbstractTextDocumentLayout *layout = _document->document()->documentLayout();
    int contentsY = _document->verticalScrollBar()->value();
    qreal pageBottom = contentsY + _document->viewport()->height();
    const QFontMetrics &fm = fontMetrics();
    const int ascent = fm.ascent() + 1; // height = ascent + descent + 1
    int lineCount = 1;

    for ( QTextBlock block = _document->document()->begin();
        block.isValid(); block = block.next(), ++lineCount )
    {

        const QRectF boundingRect = layout->blockBoundingRect( block );

        QPointF position = boundingRect.topLeft();

        // don't draw invisible lines
        if ( position.y() + boundingRect.height() < contentsY )
            continue;

        if ( position.y() > pageBottom )
            break;

        LineInfo x;
        x.line     = lineCount;
        x.textblock= block;
        x.rect     = boundingRect;

        x.rect.setX(0);
        x.rect.setWidth(width()-3);
        x.rect.translate(1, - contentsY);
        x.rect.setHeight(x.rect.height()-2);

        _visibleLineCache.append(x);

        if (block.userState()>=0)
        {
            if ( block.userState() & etboBreakPoint )
            {
                QColor col=Qt::darkRed; //_bDebugMode ? Qt::red : Qt::darkRed;
                col.setAlpha(150);

                x.rect.setWidth(_iBreakpointWidth);
                painter.fillRect(x.rect, col);
            }

            else if ( block.userState() & etboBookmark )
            {
                QColor col=Qt::darkBlue;
                col.setAlpha(150);

                x.rect.setWidth(_iBreakpointWidth);
                painter.fillRect(x.rect, col);
            }
        }

        const QString txt = QString("%1").arg( lineCount );
        painter.drawText( width() - fm.width(txt) -3 , qRound( position.y() ) - contentsY + ascent, txt );
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
