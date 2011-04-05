#ifndef DOCKWIDGETS_H
#define DOCKWIDGETS_H

class QMainWindow;


/**
@brief
Controls a set of QDockWidgets in a parent QMainWindow
by registering assigning them to a fixed QDockWidgetArea.
*/
class DockWidgets
{
public:
    /**
    @brief
    Represents an assigned dock widget.
    */
    struct AssignedDockWidget
    {
        Qt::DockWidgetArea      area;
        bool                    closeOnHide;
        bool                    resizable;
        QDockWidget *           dockWidget;

        AssignedDockWidget()
            : area(Qt::NoDockWidgetArea), closeOnHide(true), dockWidget(0) {}
    };

    DockWidgets();

    QMainWindow * parentWindow();
    void setParentWindow(QMainWindow * window);

    registerDockWidget( const QString &id, Qt::DockWidgetArea area );

private:
    QMainWindow *               _parentWindow;
    QList<AssignedDockWidget>   _assignedDockWidgets;
};

#endif // DOCKWIDGETS_H
