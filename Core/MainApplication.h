/*
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

/**
The MainApplication class enhances the QApplication class by platform independent functionality.
*/

#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QtGui/QApplication>

/**
Enhances a QApplication class with additional platform independent functionality.

At present this is used associate file types under OSX.
*/
class MainApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MainApplication(int &argc, char **argv, bool GUIenabled=true);

protected:
    bool event(QEvent *ev);

signals:
    void fileOpened(QString fileName);

};

#endif // MAINAPPLICATION_H
