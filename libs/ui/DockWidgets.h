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

#ifndef DOCKWIDGETS_H
#define DOCKWIDGETS_H

#include <QtCore/QObject>
#include <QtCore/QMap>

class QMainWindow;
class AssignedDockWidget;


/**
@brief
Controls a set of QDockWidgets in a parent QMainWindow
by registering assigning them to a fixed QDockWidgetArea.
*/
class DockWidgets
{
public:
    explicit DockWidgets( QMainWindow * ownerWindow = 0);
    virtual ~DockWidgets();

    QMainWindow * ownerWindow() const;
    void setOwnerWindow(QMainWindow * window);

    void registerDockWidget( const QString &key, Qt::DockWidgetArea area );
    //void registerDockWidget( const QString &key, const QString &widgetClassName, Qt::DockWidgetArea area );

    bool show(const QString &key, QWidget * content = 0);
    void hide(const QString &key);

    AssignedDockWidget * dockWidgetForKey(const QString &key) const;

private:
    QMainWindow *                           _ownerWindow;
    QMap<QString, AssignedDockWidget*>      _assignedDockWidgets;
};

#endif // DOCKWIDGETS_H
