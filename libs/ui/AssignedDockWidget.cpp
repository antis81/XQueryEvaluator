#include "AssignedDockWidget.h"

#include <QtGui/QLayout>


AssignedDockWidget::AssignedDockWidget()
    : _privateDW( new QDockWidget() )
    , _area(Qt::NoDockWidgetArea)
    , _options( CloseOnHide | Resizable )
{
    _privateDW->setFeatures( QDockWidget::NoDockWidgetFeatures );
    _privateDW->setTitleBarWidget( new QWidget() );

    QLayout * l = _privateDW->layout();
    if (l != 0)
        l->setContentsMargins(0,0,0,0);
    else
        _privateDW->setContentsMargins(0,0,0,0);
}

AssignedDockWidget::~AssignedDockWidget()
{
    delete _privateDW;
}

const QString & AssignedDockWidget::widgetKey() const
{
    return _widgetKey;
}

void AssignedDockWidget::setWidgetKey(const QString &widgetClassName)
{
    _widgetKey = widgetClassName;
}

Qt::DockWidgetArea AssignedDockWidget::area() const
{
    return _area;
}

void AssignedDockWidget::setArea(Qt::DockWidgetArea area)
{
    _area = area;
}

AssignedDockWidget::Options AssignedDockWidget::options() const
{
    return _options;
}

void AssignedDockWidget::setOptions(Options options)
{
    _options = options;
}

QDockWidget * AssignedDockWidget::dockWidget() const
{
    return _privateDW;
}
