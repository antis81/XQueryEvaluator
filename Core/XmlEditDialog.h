/**
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

#ifndef XMLEDITDIALOG_H
#define XMLEDITDIALOG_H

#include <QtGui/QDialog>

class XMLEditor;

namespace Ui {
    class XmlEditDialog;
}

class XmlEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit XmlEditDialog(QWidget *parent = 0);
    ~XmlEditDialog();

    QString xml() const;
    void setXml(const QString &xml);

private:
    Ui::XmlEditDialog *     ui;

    XMLEditor *             _textXml;
};

#endif // XMLEDITDIALOG_H
