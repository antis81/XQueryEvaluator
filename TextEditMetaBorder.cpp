#include "TextEditMetaBorder.h"

#include <QPainter>
#include <QScrollbar>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>

TextEditMetaBorder::TextEditMetaBorder(QTextEdit *parent)
: QWidget(parent)
, _parent(parent)
, _bDebugMode(false)
{
    const QFontMetrics fm = fontMetrics();

    _iBreakpointWidth=fm.maxWidth()*1.5;

    // 4 Zeilennummern ein space und einmap breakpoint
    setMinimumWidth(fm.maxWidth()*6);
    //QSizePolicy policy=sizePolicy();
    //policy.setVerticalPolicy(QSizePolicy::Fixed);

    //connect(_parent,SIGNAL(textChanged()),this,SLOT(repaint()));
    connect( parent->document()->documentLayout(), SIGNAL( update(const QRectF &) ),
        this, SLOT( update() ) );
    connect( parent->verticalScrollBar(), SIGNAL(valueChanged(int) ),
        this, SLOT( update() ) );
}

TextEditMetaBorder::~TextEditMetaBorder()
{
}


/**
 * (De-)aktiviert das Zeichnen des Hintergrunds mit Farbverlauf.
 */
void TextEditMetaBorder::setGradientBackground(bool enable)
{
    _gradientBG = enable;
}

void TextEditMetaBorder::paintEvent ( QPaintEvent * event )
{
    QPainter p(this);
    setFont( _parent->font() );

    drawBackground(p);
    drawLineNumbers(p);
}


void TextEditMetaBorder::drawBackground(QPainter & painter)
{
    QColor background=_parent->palette().window().color();
    QColor textBackground=_parent->palette().base().color();

    if (_gradientBG) {
        QLinearGradient linearGrad(QPointF(0, 0), QPointF(rect().width()/2, 0));
        linearGrad.setColorAt(0, background);
        linearGrad.setColorAt(1, textBackground);

        painter.setBrush( linearGrad );
    }

    painter.setPen(Qt::NoPen);
    painter.drawRect( rect() );
}

void TextEditMetaBorder::drawLineNumbers(QPainter & painter)
{
    _visibleLineCache.clear();
    painter.setPen(QPen(_parent->palette().text().color()));
    QAbstractTextDocumentLayout *layout = _parent->document()->documentLayout();
    int contentsY = _parent->verticalScrollBar()->value();
    qreal pageBottom = contentsY + _parent->viewport()->height();
    const QFontMetrics fm = fontMetrics();
    const int ascent = fontMetrics().ascent() + 1; // height = ascent + descent + 1
    int lineCount = 1;

    for ( QTextBlock block = _parent->document()->begin();
        block.isValid(); block = block.next(), ++lineCount )
    {

        const QRectF boundingRect = layout->blockBoundingRect( block );

        QPointF position = boundingRect.topLeft();
        // zeilen vor view -> nicht sichtbar
        if ( position.y() + boundingRect.height() < contentsY )
            continue;
        // zeilen nach view auch nicht mehr sichtbar
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
                painter.setPen(Qt::NoPen);
                QColor col=_bDebugMode?Qt::red:Qt::darkRed;
                col.setAlpha(150);

                QBrush b(col);
                painter.setBrush(b);
                x.rect.setWidth(_iBreakpointWidth);
                painter.drawRect(x.rect);
                painter.setPen(QPen(_parent->palette().text().color()));
            }
            if ( block.userState() & etboBookmark )
            {
                painter.setPen(Qt::NoPen);
                QColor col=Qt::darkBlue;
                col.setAlpha(150);

                QBrush b(col);
                painter.setBrush(b);
                x.rect.setWidth(_iBreakpointWidth);
                painter.drawRect(x.rect);
                painter.setPen(QPen(_parent->palette().text().color()));
            }
        }

        const QString txt = QString("%1").arg( lineCount );
        painter.drawText( width() - fm.width(txt) -3 , qRound( position.y() ) - contentsY + ascent, txt );
    }
}

void TextEditMetaBorder::mousePressEvent ( QMouseEvent * event )
{
    if ( _bDebugMode )
        return;

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
