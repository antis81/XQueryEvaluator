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

class XQEditor;

namespace Ui {
    class XQEMainWindow;
}

class QComboBox;

class XQEMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit XQEMainWindow(QWidget *parent = 0);
    ~XQEMainWindow();

private slots:
    void on_btnQuery_clicked();
    void on_btnOpenSource_clicked();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void queryLanguageSelected(int comboIndex);

    void queryFileNameChanged(const QString & newFileName);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::XQEMainWindow *ui;

    QComboBox *                 _combo; //!< @todo Outsource this and connect to an action manager or something.

    QString                     _queryFileName;
    XQEditor *                  _textQuery;
    QXmlQuery::QueryLanguage    _queryLanguage;

    QString selectSourceFile();
    QString loadSourceFile(const QString &path) const;
};

#endif // XQEMAINWINDOW_H
