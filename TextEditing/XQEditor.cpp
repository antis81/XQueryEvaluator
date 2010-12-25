#include "XQEditor.h"

#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCompleter>
#include <QtGui/QStringListModel>

#include <QtCore/QFile>
#include <QtCore/QDir>

#include "XQEdit.h"
#include "TextEditMetaBorder.h"


XQEditor::XQEditor(QWidget *parent)
    : QWidget(parent)
    , _textQuery( new XQEdit(this) )
{
    Q_INIT_RESOURCE(TextEditing);

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

	completer->setModel( modelFromFile(":/CompletionModels/XQuery.cpl") );
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	completer->setCompletionMode(QCompleter::PopupCompletion);

	_textQuery->setCompleter(completer);
}

XQEditor::~XQEditor()
{
	Q_CLEANUP_RESOURCE(TextEditing);
}

QString XQEditor::xqText() const
{
	return _textQuery->toPlainText();
}

void XQEditor::setXQText(const QString &text)
{
	_textQuery->setText(text);
}

QAbstractItemModel * XQEditor::modelFromFile(QString fileName)
{
	QFile modelFile( QDir::cleanPath(fileName) );

	if ( !modelFile.open(QIODevice::ReadOnly) )
		return 0;

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList sl;

    while (!modelFile.atEnd())
    {
        QString line = QString::fromUtf8( modelFile.readLine() ).trimmed();
        if (!line.isEmpty())
            sl << line;
    }

    modelFile.close();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return new QStringListModel(sl);
}
