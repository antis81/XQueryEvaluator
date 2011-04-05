#include "TextSearch.h"
#include "ui_TextSearch.h"

#include <QtGui/QPlainTextEdit>

/**
Constructor of TextSearch.
*/
TextSearch::TextSearch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextSearch)
    , _textEdit(0)
{
    ui->setupUi(this);
}

/**
Destructor of TextSearch.
*/
TextSearch::~TextSearch()
{
    delete ui;
}

/***/
QPlainTextEdit * TextSearch::textEdit()
{
    return _textEdit;
}

/**
Sets the text edit to which the search relates.
*/
void TextSearch::setTextEdit(QPlainTextEdit *edit)
{
    _textEdit = edit;
}

void TextSearch::on_btnForward_clicked()
{
    find( ui->textSearch->text() );
}

void TextSearch::on_btnBack_clicked()
{
    find( ui->textSearch->text(), false, QTextDocument::FindBackward );
}

void TextSearch::on_btnClose_clicked()
{
    close();
}

/**
Called when search string changes.
*/
void TextSearch::on_textSearch_textChanged()
{
    find( ui->textSearch->text(), true );
}

/**
Searches occurences of a string in the _textEdit.
*/
void TextSearch::find(const QString &search, bool fromStart, QTextDocument::FindFlags options)
{
    if (!_textEdit || search.isEmpty())
        return;

    if ( fromStart )
    {
        QTextCursor crsr = _textEdit->textCursor();
        crsr.setPosition(0);
        _textEdit->setTextCursor(crsr);
    }

    QPalette pal = ui->textSearch->palette();
    if ( _textEdit->find( search, options ) )
    {
        pal.setBrush(QPalette::Base, Qt::white);
        ui->textSearch->setPalette(pal);
    } else {
        pal.setBrush(QPalette::Base, QColor(0xFF,0x80,0x80));
        ui->textSearch->setPalette(pal);
    }
}
