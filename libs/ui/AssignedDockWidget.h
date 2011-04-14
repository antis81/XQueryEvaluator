#ifndef ASSIGNEDDOCKWIDGET_H
#define ASSIGNEDDOCKWIDGET_H

#include <QtCore/QFlags>
#include <QtGui/QDockWidget>


/**
@brief
Represents an assigned dock widget.
*/
class AssignedDockWidget
        Q_FLAGS( Options )
{
public:
    explicit AssignedDockWidget();
    virtual ~AssignedDockWidget();

    const QString& widgetKey() const;
    void setWidgetKey(const QString &widgetClassName);

    Qt::DockWidgetArea area() const;
    void setArea(Qt::DockWidgetArea area);

    QDockWidget * dockWidget() const;

private:
    QDockWidget *           _privateDW;

    QString                 _widgetKey; //!< Class name of the contents widget (CASE SENSITIVE!).
    Qt::DockWidgetArea      _area;
};

#endif // ASSIGNEDDOCKWIDGET_H
