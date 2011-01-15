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
