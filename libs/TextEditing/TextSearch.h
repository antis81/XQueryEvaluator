#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H

#include <QtGui/QWidget>
#include <QtGui/QTextDocument>

class QPlainTextEdit;

namespace Ui {
    class TextSearch;
}


/**
@brief
Represents a generic text search dialog.
*/
class TextSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TextSearch(QWidget *parent = 0);
    ~TextSearch();

    QPlainTextEdit * textEdit();
    void setTextEdit( QPlainTextEdit * edit );

private slots:
    void on_btnForward_clicked();
    void on_btnBack_clicked();
    void on_btnClose_clicked();

    void on_textSearch_textChanged();

private:
    Ui::TextSearch *    ui;

    QPlainTextEdit *    _textEdit;

    void find( const QString &search, bool fromStart =false, QTextDocument::FindFlags options =0 );
};

#endif // TEXTSEARCH_H
