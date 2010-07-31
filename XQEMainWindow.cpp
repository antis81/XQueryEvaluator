#include "XQEMainWindow.h"

#include "ui_XQEMainWindow.h"
#include "XQEOutput.h"
#include "XQEMessageHandler.h"
#include "TextEditMetaBorder.h"

#include <QtXmlPatterns/QtXmlPatterns>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QSplitter>


XQEMainWindow::XQEMainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::XQEMainWindow)
{
    ui->setupUi(this);
    //_textXml = new QTextEdit(this);
    _textQuery = new QTextEdit(this);

	// Zeilennummern
	TextEditMetaBorder  *lineNumbers = new TextEditMetaBorder(_textQuery);
	lineNumbers->setFixedWidth(40);
	lineNumbers->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	lineNumbers->setGradientBackground(false);

	//	QVBoxLayout *tabLayout = new QVBoxLayout;
	//	tabLayout->setSpacing(3);
	//	tabLayout->setMargin(0);
	QHBoxLayout *horLayout = new QHBoxLayout;
	horLayout->setSpacing(0);
	horLayout->setMargin(0);
	horLayout->addWidget(lineNumbers);
	horLayout->addWidget(_textQuery);

//	QSplitter * s = new QSplitter();
	QLayout *l = ui->myPlace->layout();
	if (l == 0)
		l = new QGridLayout(ui->myPlace);
//	l->addWidget(s);
	l->setContentsMargins(0,0,0,0);

//	s->setOrientation(Qt::Horizontal);
//	s->addWidget(_textXml);

	//	QWidget *w1 = new QWidget(s);
	QWidget *w2 = new QWidget();
	w2->setLayout(horLayout);
	l->addWidget(w2);
//	s->addWidget(w2);

    //_xmlInHighlighter.setDocument(_textXml->document());
    _xqueryHighlighter.setDocument(_textQuery->document());
}

XQEMainWindow::~XQEMainWindow()
{
    delete ui;
}

void XQEMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void XQEMainWindow::on_btnQuery_clicked()
{
    //    QBuffer input;
    //    input.setData(ui->textSourceXml->toPlainText().toUtf8());
    //    input.open(QIODevice::ReadOnly);

    const QString source = loadSourceFile( ui->textSourceFile->text() ); //_textXml->toPlainText();
    QXmlQuery query;
    XQEMessageHandler msgHandler;
    query.setMessageHandler(&msgHandler);

//    QByteArray inArray(source.toUtf8());
//    QBuffer inBuf(&inArray);
//    inBuf.open(QIODevice::ReadOnly);
//    query.bindVariable("inputDoc", &inBuf);
    query.setFocus(source);

    query.setQuery(_textQuery->toPlainText());
    //    QXmlFormatter serializer(query, );
    QTime stopWatch;
    stopWatch.start();

    //QString         out;
    QBuffer outBuffer;
    outBuffer.open(QIODevice::WriteOnly);

    QXmlSerializer serializer(query, &outBuffer);
    query.evaluateTo(&serializer);

    QString out = QString::fromUtf8(outBuffer.data().constData());

    int duration = stopWatch.elapsed(); // time measurement

    XQEOutput dlg;
    dlg.setDuration(duration);

    if (!msgHandler.errLog().isEmpty())
    {
        dlg.setXml(msgHandler.errLog());
    } else {
        dlg.setXml(out);
    }

    dlg.exec();
}

void XQEMainWindow::on_btnOpenSource_clicked()
{
    ui->textSourceFile->setText(selectSourceFile());
}

void XQEMainWindow::on_actionOpen_triggered()
{
//    QString sourceFile = selectSourceFile();
//    if (!sourceFile.isEmpty())
//    {
//        _textXml->setText( loadSourceFile(sourceFile) );
//    }

    QString destFile = QFileDialog::getOpenFileName(0, tr("Open query file ..."), "", "*.xq");
    if (!destFile.isEmpty())
    {
        QFile dest(destFile);

        if ( dest.open(QIODevice::ReadOnly) )
            _textQuery->setText( QString::fromUtf8(dest.readAll().constData()) );
    }
}

void XQEMainWindow::on_actionSave_triggered()
{
    //    QString selectedFileExt = "*.xml";
    //    QString sourceFile = QFileDialog::getSaveFileName(0, tr("Save XML source file ..."), "", "*.xml", &selectedFileExt);
    //    if (!sourceFile.isEmpty())
    //    {
    //        QFile source(sourceFile);

    //        if ( source.open(QIODevice::WriteOnly) )
    //            source.write(ui->textSourceXml->toPlainText().toUtf8());
    //    }

    //    selectedFileExt = "*.xq";
    QString destFile = QFileDialog::getSaveFileName(0, tr("Save query file ..."), QDir::homePath(), tr("XQuery file (%1)").arg("*.xq"));
    if (!destFile.isEmpty())
    {
        QFile dest(destFile);

        if ( dest.open(QIODevice::WriteOnly) )
            dest.write(_textQuery->toPlainText().toUtf8());
        else
            QMessageBox::critical(0, tr("Error"), tr("Unable to save XQuery file at: %1").arg(destFile));
    }
}

QString XQEMainWindow::selectSourceFile()
{
    return QDir::cleanPath( QFileDialog::getOpenFileName(0, tr("Open XML source file ..."), "", "*.xml") );
}

QString XQEMainWindow::loadSourceFile(const QString &path) const
{
    if (path.isEmpty())
        return QString();

    QFile f(path);
    if (f.open(QIODevice::ReadOnly))
        return f.readAll();

    return QString();
}
