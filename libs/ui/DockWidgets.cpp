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

#include "DockWidgets.h"

#include <QtGui/QMainWindow>

#include "AssignedDockWidget.h"


/**
The constructor should set the owner window, that handles QDockWidgets.
*/
DockWidgets::DockWidgets(QMainWindow * ownerWindow)
    : _ownerWindow( ownerWindow )
{
}

DockWidgets::~DockWidgets()
{
}

/**
@return The parent window owns the dock widget areas.
*/
QMainWindow * DockWidgets::ownerWindow() const
{
    return _ownerWindow;
}

/**
Sets the parent window that owns the dock widget areas.
*/
void DockWidgets::setOwnerWindow(QMainWindow *window)
{
    _ownerWindow = window;
}

/**
Convenience method to register a dockWidget without a dynamically assigned content widget type.
The widget can be set by another instance before calling show.
*/
void DockWidgets::registerDockWidget( const QString &key, Qt::DockWidgetArea area )
{
        AssignedDockWidget * dw = _assignedDockWidgets.insert(key, new AssignedDockWidget()).value();

        dw->setArea(area);
        dw->setContentWidget(0);
}

///**
//Registers a dock widget for a fixed area.
//*/
//void DockWidgets::registerDockWidget( const QString &key, const QString &widgetClassName, Qt::DockWidgetArea area )
//{
//    AssignedDockWidget * dw = _assignedDockWidgets.insert(key, new AssignedDockWidget()).value();

//    dw->setArea(area);
//    dw->setContentWidget(0);
//}

/**
Show the assigned dock widget for key.
*/
bool DockWidgets::show(const QString &key, QWidget *content)
{
    if ( _ownerWindow == 0 )
        return false;

    AssignedDockWidget * dw = dockWidgetForKey(key);
    if ( dw == 0 )
        return false;

    // when a content widget is given, set it
    if ( content != 0 )
        dw->setContentWidget(content);

    //! @todo Let the user set the orientation (Qt::Horizontal, Qt::Vertical).
    _ownerWindow->addDockWidget( dw->area(), dw->dockWidget() );
    if ( !dw->dockWidget()->isVisible() )
        dw->dockWidget()->show();

    return true;
}

/**
Hide the assigned dock widget for key.
*/
void DockWidgets::hide(const QString &key)
{
    if ( _ownerWindow == 0 )
        return;

    const AssignedDockWidget * dw = dockWidgetForKey(key);
    if ( dw == 0 )
        return;

    _ownerWindow->removeDockWidget( dw->dockWidget() );
}

AssignedDockWidget * DockWidgets::dockWidgetForKey(const QString &key) const
{
    return _assignedDockWidgets.value(key, 0);
}
