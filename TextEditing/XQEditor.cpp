#include "XQEditor.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QCompleter>
#include <QtGui/QStringListModel>

#include "XQEdit.h"
#include "TextEditMetaBorder.h"


XQEditor::XQEditor(QWidget *parent)
    : QWidget(parent)
    , _textQuery( new XQEdit(this) )
{
	// Zeilennummern
	TextEditMetaBorder  *lineNumbers = new TextEditMetaBorder(_textQuery);
	lineNumbers->setFixedWidth(40);
	lineNumbers->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

	QHBoxLayout *horLayout = new QHBoxLayout;
	horLayout->setSpacing(0);
	horLayout->setMargin(0);
	horLayout->addWidget(lineNumbers);
	horLayout->addWidget(_textQuery);

	setLayout(horLayout);

	QCompleter *completer = new QCompleter();

	//! @todo For playing around only
	QStringList sl;
	sl << "return" << "local" << "lower-case";
	sl << "declare" << "variable" << "function";

	completer->setModel( new QStringListModel(sl) );
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	completer->setCompletionMode(QCompleter::PopupCompletion);

	_textQuery->setCompleter(completer);
}

XQEditor::~XQEditor()
{
}

QString XQEditor::xqText() const
{
	return _textQuery->toPlainText();
}

void XQEditor::setXQText(const QString &text)
{
	_textQuery->setText(text);
}
