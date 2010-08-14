#include "XQEOutput.h"
#include "ui_XQEOutput.h"

XQEOutput::XQEOutput(QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::XQEOutput)
{
    ui->setupUi(this);
    //_xmlOutHighlighter.setDocument(ui->textOutput->document());
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
    ui->textOutput->setText(xml);
}

void XQEOutput::setHtml(const QString &html)
{
    ui->textOutput->setHtml(html);
}

void XQEOutput::setDuration(int duration)
{
    ui->textDuration->setText(tr("Duration: %1 ms").arg(duration));
}

