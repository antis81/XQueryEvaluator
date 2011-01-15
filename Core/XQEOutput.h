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

#ifndef XQEOUTPUT_H
#define XQEOUTPUT_H

#include <QDialog>

#include "TextEditing/XmlHighlighter.h"

namespace Ui {
    class XQEOutput;
}

class XQEOutput : public QDialog
{
    Q_OBJECT

public:
    explicit XQEOutput(QWidget *parent = 0);
    ~XQEOutput();

    void setErrors(const QString &html);
    void setXml(const QString &xml);
    void setHtml(const QString &html);

    void setDuration(int duration);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::XQEOutput *ui;

    XmlHighlighter          _xmlOutHighlighter;
};

#endif // XQEOUTPUT_H
