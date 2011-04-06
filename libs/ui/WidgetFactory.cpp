#include "WidgetFactory.h"

#include <QtGui/QWidget>


WidgetFactory::WidgetFactory()
    : _defaultGenerator(0)
{
}

WidgetFactory::~WidgetFactory()
{
    delete _defaultGenerator;
}


QWidget * WidgetFactory::createWidget( const QString &widgetClassName ) const
{
    AbstractWidgetGenerator * generator = _widgetGenerators.value( widgetClassName, 0 );
    if ( generator != 0 )
        return generator->newInstance();

    return defaultWidget();
}

QWidget * WidgetFactory::defaultWidget() const
{
    if (_defaultGenerator != 0)
        return _defaultGenerator->newInstance();

    return 0;
}
