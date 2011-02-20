/**
**    Copyright 2011 by Nils Fenner
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

#ifndef XMLSOURCE_H
#define XMLSOURCE_H

#include <QtGui/QWidget>

#include <QtCore/QDir>

namespace Ui {
    class XmlSource;
}

class XmlSource : public QWidget
{
    Q_OBJECT

public:
    explicit XmlSource(QWidget *parent = 0);
    ~XmlSource();

    QString sourceFile() const;

    QWidget * editor() const { return _editor; }
    void setEditor(QWidget *w) { _editor = w; }

signals:
    void sourceFileAvailable(bool yes);

private slots:
    void on_btnOpenSource_clicked();

    void setSourceFile(QString sourceFile);

protected:
    void hideEvent(QHideEvent * ev);

private:
    Ui::XmlSource *ui;

    QString         _sourceFile;
    QWidget *       _editor;

    QString selectSourceFile();

    void readSettings();
    void writeSettings();

};

#endif // XMLSOURCE_H
