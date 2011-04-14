#include "DockWidgets.h"

#include <QtGui/QMainWindow>


DockWidgets::DockWidgets()
    : _ownerWindow(0)
{
}

/**
This is a convenience constructor.
*/
DockWidgets::DockWidgets(QMainWindow * ownerWindow)
    : _ownerWindow( ownerWindow )
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
void DockWidgets::registerDockWidget(
    const QString &key, Qt::DockWidgetArea area )
{
    registerDockWidget(key, QString(), area );
}

/**
Registers a dock widget for a fixed area.
*/
void DockWidgets::registerDockWidget( const QString &key, const QString &widgetClassName, Qt::DockWidgetArea area )
{
    AssignedDockWidget * dw = _assignedDockWidgets.insert(key, new AssignedDockWidget()).value();

    dw->setArea(area);
    dw->setWidgetKey(widgetClassName);
}

/**
Show the assigned dock widget for key.
*/
void DockWidgets::show(const QString &key)
{
    if ( _ownerWindow == 0 )
        return;

    const AssignedDockWidget * dw = dockWidgetForKey(key);
    if ( (dw == 0) || dw->dockWidget()->isVisible() )
        return;

    //! @todo Let the user set the orientation (Qt::Horizontal, Qt::Vertical).
    _ownerWindow->addDockWidget( dw->area(), dw->dockWidget() );
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
