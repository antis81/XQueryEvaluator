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

#ifndef XQEMAINWINDOW_H
#define XQEMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtXmlPatterns/QXmlQuery>

#include "Query/XQEvaluator.h"


namespace Ui {
    class XQEMainWindow;
}

class XmlSource;
class XQEditor;
class XmlEditor;
class QComboBox;

class XQEMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit XQEMainWindow(QWidget *parent = 0);
    ~XQEMainWindow();

public slots:
    void loadQuery(QString fileName);

private slots:
    void autoIndent();
    void startQuery();
    void changeFormattedOutput(bool enabled);

    void actionViewSource();

    void actionOpenQuery();
    void actionSaveQuery();

    void queryLanguageSelected(int comboIndex);

    void queryFileNameChanged(const QString & newFileName);

    void about();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::XQEMainWindow *ui;

    QComboBox *                 _textQueryType;

    QString                     _queryFileName;
    XmlSource *                 _xmlSource;
    XQEditor *                  _textQuery;
    XmlEditor *                 _xmlEditor;

    XQEvaluator                 _xqeval;

    void readSettings();
    void writeSettings();

    QString selectSourceFile();
    QString loadSourceFile(const QString &path) const;

    bool saveQuery();
    bool queryCanClose();
};

#endif // XQEMAINWINDOW_H
