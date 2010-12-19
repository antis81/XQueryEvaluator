#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <QtGui/QTextCharFormat>
#include <QtXml/QDomDocument>

class QTextDocument;

class XmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    XmlHighlighter(QTextDocument *parent = 0);
    virtual ~XmlHighlighter();

protected:
    void highlightBlock(const QString &text);
    void colorBlock( int blockState, const QString &text, const QRegExp &startExp, 
                     const QRegExp &endExp, const QTextCharFormat &fmt);
    void addHighlightingRule(const QStringList &patterns, const QTextCharFormat &format);

private:
    struct HighlightingRule
        {
        QList<QRegExp*>      patterns;
        QTextCharFormat      format;
        };
    
	QList<HighlightingRule*>	_highlightingRules;

    QRegExp                 _commentStartExp;
    QRegExp                 _commentEndExp;
	QTextCharFormat		_commentFormat;
	
	QRegExp					_headerStartExp;
	QRegExp					_headerEndExp;
	QTextCharFormat		_xmlHeadFormat;
	
	QRegExp					_cdataStartExp;
	QRegExp					_cdataEndExp;
	QTextCharFormat		_cdataFormat;
	
	QTextCharFormat		_keySignFormat;
	QTextCharFormat		_xmlTagFormat;
	QTextCharFormat		_xmlAttrFormat;
};

#endif // LBXMLHIGHLIGHTER_H
