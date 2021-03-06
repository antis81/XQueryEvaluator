/*
 * XQueryEvaluator - Edit and run XQuery / XSLT scripts.
 * Copyright (C) 2011-2015 by Nils Fenner <nils@macgitver.org>
 *
 * This file is part of XQueryEvaluator.
 *
 * XQueryEvaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XQueryEvaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XMLSOURCE_H
#define XMLSOURCE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>

#include <QtCore/QDir>

/**
Provides functionality for managing XML source documents.
*/
class XmlSource : public QWidget
{
    Q_OBJECT

public:
    explicit XmlSource(QWidget *parent = 0);
    ~XmlSource();

    QString sourceFile() const;

    QWidget * editor() const { return _editor; }
    void setEditor(QWidget *w) { _editor = w; }

    void editSource();

signals:
    void sourceFileAvailable(bool yes);

private slots:
    void on_btnOpenSource_clicked();

    void setSourceFile(QString sourceFile);

protected:
    void hideEvent(QHideEvent * ev);

private:
    QComboBox *     _textSourceFile;
    QString         _sourceFile;

    QToolButton *   _btnOpenSource;

    QWidget *       _editor;
    QString         _externalEditor;

    QString selectSourceFile();

    void readSettings();
    void writeSettings();

};

#endif // XMLSOURCE_H
