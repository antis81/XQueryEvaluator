#ifndef TEXTEDITBASE_H
#define TEXTEDITBASE_H

#include <QtGui/QPlainTextEdit>

class TextEditMetaBorder;


class TextEditBase : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextEditBase(QWidget *parent = 0);
    virtual ~TextEditBase();

    void metaBorderPaintEvent(QPaintEvent *e);
    int metaBorderWidth() const;

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateMetaBorderWidth(int newBlockCount);
    void updateMetaBorder(const QRect &, int);

    void highlightCurrentLine();

private:
    TextEditMetaBorder *    _metaBorder;
};

#endif // TEXTEDITBASE_H
