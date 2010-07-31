#ifndef XQEMAINWINDOW_H
#define XQEMAINWINDOW_H

#include <QtGui/QMainWindow>

#include "XmlHighlighter.h"
#include "XQueryHighlighter.h"

namespace Ui {
    class XQEMainWindow;
}

class XQEMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit XQEMainWindow(QWidget *parent = 0);
    ~XQEMainWindow();

private slots:
    void on_btnQuery_clicked();
    void on_btnOpenSource_clicked();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::XQEMainWindow *ui;

    //QTextEdit *             _textXml;
    QTextEdit *             _textQuery;

    XmlHighlighter          _xmlInHighlighter;
    XQueryHighlighter       _xqueryHighlighter;

    QString selectSourceFile();
    QString loadSourceFile(const QString &path) const;
};

#endif // XQEMAINWINDOW_H
