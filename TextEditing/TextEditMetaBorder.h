#ifndef TEXTEDITMETABORDER_H
#define TEXTEDITMETABORDER_H


#include <QtGui/QTextEdit>
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

    explicit TextEditMetaBorder(QTextEdit *doc, QWidget *parent=0);
    ~TextEditMetaBorder();

//    bool isBreakpoint(int line) { return _breakpoints.contains(line); }
//    QSet<int> breakpoints() const { return _breakpoints; }

//    void setDebugMode(bool bnew) { update(); _bDebugMode=bnew; }
//    bool debugMode() const { return _bDebugMode; }

    void showEvent(QShowEvent *e);
    void paintEvent ( QPaintEvent * event ) ;
    void drawLineNumbers(QPainter & painter);
    void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void breakpointChanged(bool bSet);
private:
//    bool             _bDebugMode;
    int             _iBreakpointWidth;
    QTextEdit *     _document;
    //QSet<int>        _breakpoints;

    struct LineInfo
    {
        int        line;
        QTextBlock textblock;
        QRectF     rect;
    };

    QList<LineInfo> _visibleLineCache;
};

#endif // TEXTEDITMETABORDER_H
