#ifndef XQEDIT_H
#define XQEDIT_H

#include <QTextEdit>

#include "XQueryHighlighter.h"


class QCompleter;

class XQEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit XQEdit(QWidget *parent = 0);
    virtual ~XQEdit();

    QCompleter *completer() const { return _completer; }
    void setCompleter(QCompleter *completer);

    void keyPressEvent(QKeyEvent *e);

    private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleter *        _completer;
    XQueryHighlighter   _xqueryHighlighter;

    QString textUnderCursor() const;
};

#endif // XQEDIT_H
