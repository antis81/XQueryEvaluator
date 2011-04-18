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
    void registerDockWidget( const QString &key, const QString &widgetClassName, Qt::DockWidgetArea area );

    bool show(const QString &key, QWidget * content = 0);
    void hide(const QString &key);

    AssignedDockWidget * dockWidgetForKey(const QString &key) const;

private:
    QMainWindow *                           _ownerWindow;
    QMap<QString, AssignedDockWidget*>      _assignedDockWidgets;
};

#endif // DOCKWIDGETS_H
