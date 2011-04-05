#include "DockWidgets.h"

#include <QtGui/QMainWindow>


DockWidgets::DockWidgets()
    : _parentWindow(0)
{
}

/**
@return The parent window owns the dock widget areas.
*/
QMainWindow * DockWidgets::parentWindow()
{
    return _parentWindow;
}

/**
Sets the parent window that owns the dock widget areas.
*/
void DockWidgets::setParentWindow(QMainWindow *window)
{
    _parentWindow = window;
}
