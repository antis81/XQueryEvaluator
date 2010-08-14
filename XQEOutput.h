#ifndef XQEOUTPUT_H
#define XQEOUTPUT_H

#include <QDialog>

#include "XmlHighlighter.h"

namespace Ui {
    class XQEOutput;
}

class XQEOutput : public QDialog
{
    Q_OBJECT

public:
    explicit XQEOutput(QWidget *parent = 0);
    ~XQEOutput();

    void setErrors(const QString &html);
    void setXml(const QString &xml);
    void setHtml(const QString &html);

    void setDuration(int duration);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::XQEOutput *ui;

    XmlHighlighter          _xmlOutHighlighter;
};

#endif // XQEOUTPUT_H
