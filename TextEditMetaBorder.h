#ifndef TEXTEDITMETABORDER_H
#define TEXTEDITMETABORDER_H


#include <QTextEdit>
#include <QPaintEvent>
#include <QSet>
#include <QTextBlock>

class TextEditMetaBorder : public QWidget
{
    Q_OBJECT

public:

    enum eTextBlockOption
    {
        etboBreakPoint= 0x01,
        etboBookmark  = 0x02
    };

    TextEditMetaBorder(QTextEdit *parent);
    ~TextEditMetaBorder();

    void setGradientBackground(bool enable);

    //bool isBreakpoint(int line) { return _breakpoints.contains(line); }
    //QSet<int> breakpoints() const { return _breakpoints; }

//    void setDebugMode(bool bnew) { update(); _bDebugMode=bnew; }
//    bool debugMode() const { return _bDebugMode; }

protected:
    virtual void paintEvent ( QPaintEvent * event ) ;
    virtual void drawBackground(QPainter & painter);
    virtual void drawLineNumbers(QPainter & painter);
    virtual void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void breakpointChanged(bool bSet);
private:
    bool             _gradientBG;
//    bool             _bDebugMode;
    int              _iBreakpointWidth;
    QTextEdit *      _parent;
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
