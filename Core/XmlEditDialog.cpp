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

#include "XmlEditDialog.h"
#include "ui_XmlEditDialog.h"

#include "TextEditing/XMLEditor.h"

#include <QtGui/QLayout>


XmlEditDialog::XmlEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::XmlEditDialog)
    , _textXml(new XMLEditor())
{
    ui->setupUi(this);

	QLayout *l = ui->myPlace->layout();
	if (l == 0)
		l = new QGridLayout(ui->myPlace);
	l->setContentsMargins(0,0,0,0);

	l->addWidget( _textXml );
}

XmlEditDialog::~XmlEditDialog()
{
	delete ui;
}

QString XmlEditDialog::xml() const
{
	return _textXml->xml();
}

void XmlEditDialog::setXml(const QString &xml)
{
	_textXml->setXml(xml);
}
