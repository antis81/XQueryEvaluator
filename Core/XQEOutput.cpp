/*
 * XQueryEvaluator - Edit and run XQuery / XSLT scripts.
 * Copyright (C) 2011-2015 by Nils Fenner <nils@macgitver.org>
 *
 * This file is part of XQueryEvaluator.
 *
 * XQueryEvaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XQueryEvaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "XQEOutput.h"
#include "ui_XQEOutput.h"


/**
Constructs an output window to show a query result.
*/
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

/**
Sets an error log's output. Expects HTML formatted output.
*/
void XQEOutput::setErrors(const QString &html)
{
    ui->textErrors->setHtml(html);
}

/**
Sets the output text.
*/
void XQEOutput::setXml(const QString &xml)
{
    ui->textOutput->setPlainText(xml);
}

/**
Sets the duration in ms.
*/
void XQEOutput::setDuration(int duration)
{
    ui->textDuration->setText(tr("Duration: %1 ms").arg(duration));
}
