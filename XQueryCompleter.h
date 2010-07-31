#ifndef XQUERYCOMPLETER_H
#define XQUERYCOMPLETER_H

#include <QCompleter>

class XQueryCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit XQueryCompleter(QObject *parent = 0);

signals:

public slots:

};

#endif // XQUERYCOMPLETER_H
