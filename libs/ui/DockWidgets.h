#ifndef DOCKWIDGETS_H
#define DOCKWIDGETS_H

#include "ui/WidgetFactory.h"
#include "ui/AssignedDockWidget.h"

class QMainWindow;
class AbstractWidgetGenerator;


/**
@brief
Controls a set of QDockWidgets in a parent QMainWindow
by registering assigning them to a fixed QDockWidgetArea.
*/
class DockWidgets : public WidgetFactory
{
public:
    DockWidgets();
    DockWidgets( QMainWindow * ownerWindow );

    QMainWindow * ownerWindow() const;
    void setOwnerWindow(QMainWindow * window);

    void registerDockWidget( const QString &key, Qt::DockWidgetArea area );
    void registerDockWidget( const QString &key, const QString &widgetClassName, Qt::DockWidgetArea area );

    void show(const QString &key);
    void hide(const QString &key);

    AssignedDockWidget * dockWidgetForKey(const QString &key) const;

private:
    QMainWindow *                           _ownerWindow;
    QMap<QString, AssignedDockWidget*>      _assignedDockWidgets;
};

#endif // DOCKWIDGETS_H
