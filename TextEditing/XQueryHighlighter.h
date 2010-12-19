#ifndef XQUERYHIGHLIGHTER_H
#define XQUERYHIGHLIGHTER_H

#include <QtCore/QList>
#include <QtGui/QSyntaxHighlighter>

class XQueryHighlighter : public QSyntaxHighlighter
{
public:
    struct HighlightBlock
    {
        QRegExp startExp;
        QRegExp endExp;
        QTextCharFormat textFormat;
    };

    XQueryHighlighter(QTextDocument *parent =0);
    virtual ~XQueryHighlighter();

protected:
    void highlightBlock(const QString &text);
    void colorBlock( int blockState, const QString &text, const HighlightBlock &blockFormat);
    void addHighlightingRule(const QStringList &patterns, const QTextCharFormat &format);

private:
    struct HighlightingRule
        {
        QList<QRegExp*>      patterns;
        QTextCharFormat      format;
        };

    QList<HighlightingRule*>	_highlightingRules;

//    QRegExp            _commentStartExp;
//    QRegExp            _commentEndExp;
//    QTextCharFormat    _commentFormat;

//	QRegExp				_headerStartExp;
//	QRegExp				_headerEndExp;
//	QTextCharFormat		_xmlHeadFormat;

//	QRegExp				_cdataStartExp;
//	QRegExp				_cdataEndExp;
//	QTextCharFormat		_cdataFormat;
	QMap<QString, HighlightBlock>		_blocks;

	QTextCharFormat		_keySignFormat;
	QTextCharFormat		_xmlTagFormat;
	QTextCharFormat		_xmlAttrFormat;

	QTextCharFormat		_keyWordFormat;
	QTextCharFormat		_varNameFormat;
};

#endif // XQUERYHIGHLIGHTER_H
