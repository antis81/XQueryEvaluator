/**
**    Copyright (c) 2011 by Nils Fenner
**
**    This file is part of XQueryEvaluator.
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
**    along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "XQEOutput.h"
#include "ui_XQEOutput.h"

XQEOutput::XQEOutput(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::XQEOutput)
{
    ui->setupUi(this);
    //_xmlOutHighlighter.setDocument(ui->textOutput->document());

    const QFontMetrics &fm = ui->textOutput->fontMetrics();
    ui->textOutput->setTabStopWidth( fm.width(QChar(' ')) * 4 );
}

XQEOutput::~XQEOutput()
{
    delete ui;
}

void XQEOutput::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void XQEOutput::setErrors(const QString &html)
{
    ui->textErrors->setHtml(html);
}

void XQEOutput::setXml(const QString &xml)
{
    ui->textOutput->setPlainText(xml);
}

void XQEOutput::setDuration(int duration)
{
    ui->textDuration->setText(tr("Duration: %1 ms").arg(duration));
}
