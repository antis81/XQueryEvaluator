#ifndef XQEDITOR_H
#define XQEDITOR_H

#include <QtGui/QWidget>

class XQEdit;
class QAbstractItemModel;


class XQEditor : public QWidget
{
    Q_OBJECT

public:
    explicit XQEditor(QWidget *parent = 0);
    ~XQEditor();

    QString xqText() const;
    void setXQText(const QString &text);

private:
    XQEdit *     _textQuery;

    QAbstractItemModel * modelFromFile(QString fileName);
};

#endif // XQEDITOR_H
