/*
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
#include "ui/DockWidgets.h"


namespace Ui {
    class XQEMainWindow;
}

class XmlSource;
class XQEditor;
class XmlEditor;
class QComboBox;


/**
The main user interface of XQueryEvaluator.
*/
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
    void actionOutputToFile(bool yes);

    void actionViewSource(bool activate);
    void actionEditSource();

    void actionNewQuery();
    void actionOpenQuery();
    void actionSaveQuery();
    void actionSaveQueryAs();

    void actionSearchText();

    void queryLanguageSelected(int comboIndex);
    void queryFileNameChanged(const QString & newFileName);

    void about();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::XQEMainWindow *ui;

    QComboBox *                 _textQueryType;

    bool                        _outputToFile;
    QString                     _queryFileName;
    QString                     _outputFilePath;
    XmlSource *                 _xmlSource;
    XQEditor *                  _textQuery;

    XQEvaluator                 _xqeval;

    DockWidgets                 _fixedDockWidgets;

    void readSettings();
    void writeSettings();

    QString selectSourceFile();
    QString loadSourceFile(const QString &path) const;

    bool saveQuery(bool saveAs = false);
    bool queryCanClose();

    void saveOutputToFile( const QString &content );
};

#endif // XQEMAINWINDOW_H
