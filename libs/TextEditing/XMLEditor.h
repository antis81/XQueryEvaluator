/*
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

#ifndef XMLEDITOR_H
#define XMLEDITOR_H

#include <QtGui/QWidget>

#include "XmlHighlighter.h"

class QPlainTextEdit;

/**
An XML editor showing highlighted XML text and line numbers. Currently read only.
*/
class XmlEditor : public QWidget
{
    Q_OBJECT
public:
    explicit XmlEditor(QWidget *parent = 0);
    ~XmlEditor();

    QString xml() const;
    void setXml(const QString & xml);

protected:
    virtual void hideEvent(QHideEvent * ev);

private:
    QPlainTextEdit *    _textXml;
    XmlHighlighter      _highlighter;

    void readSettings();
    void writeSettings();
};

#endif // XMLEDITOR_H