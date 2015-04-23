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

#ifndef XQE_XMLEDITOR_H
#define XQE_XMLEDITOR_H

#include <QtWidgets/QWidget>

#include "XmlHighlighter.h"
#include "TextEditBase.h"


/**
* @class    XmlEditor
*
* @brief    An XML editor showing highlighted XML text and line numbers.
*
* @todo     Currently XML docs can only be viewed, not edited.
*/
class XmlEditor : public TextEditBase
{
//    Q_OBJECT
public:
    explicit XmlEditor(QWidget *parent = 0);
    virtual ~XmlEditor();

protected:
    virtual void hideEvent(QHideEvent * ev);

private:
    XmlHighlighter    _highlighter;

    void readSettings();
    void writeSettings();
};

#endif
