/**
**    Copyright 2011 by Nils Fenner
**
**    This file is part of XQueryEvaluator
**
**    XQueryEvaluator is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    XQueryEvaluator is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with XQueryEvaluator. If not, see <http://www.gnu.org/licenses/>.
*/

#include "XMLEditor.h"

#include <QtGui/QPlainTextEdit>
#include <QtGui/QHBoxLayout>

#include "TextEditMetaBorder.h"


XMLEditor::XMLEditor(QWidget *parent)
	: QWidget(parent)
	, _textXml(new QPlainTextEdit(this))
{
	_highlighter.setDocument( _textXml->document() );

    const QFontMetrics &fm = fontMetrics();
    _textXml->setTabStopWidth( fm.width(QChar(' ')) * 4 );

	// line numbers
	TextEditMetaBorder *lineNumbers = new TextEditMetaBorder(_textXml);
	lineNumbers->setFixedWidth(40);
	lineNumbers->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

	QHBoxLayout *horLayout = new QHBoxLayout();
	horLayout->setSpacing(0);
	horLayout->setMargin(0);
	horLayout->addWidget(lineNumbers);
	horLayout->addWidget(_textXml);

	setLayout(horLayout);

	//! @todo Prevent writing for the moment.
	_textXml->setReadOnly(true);
}

XMLEditor::~XMLEditor()
{
}

QString XMLEditor::xml() const
{
	return _textXml->toPlainText();
}

void XMLEditor::setXml(const QString &xml)
{
	_textXml->setPlainText(xml);
}
