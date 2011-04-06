#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include "ui/AbstractWidgetGenerator.h"

#include <QtCore/QMap>


/**
@brief
Factory to dynamically generate widget by a defined key.
As key, the class name is assumed, but another key can be used if necessary.
*/
class WidgetFactory
{
public:
    WidgetFactory();
    virtual ~WidgetFactory();

    QWidget * createWidget( const QString &widgetClassName ) const;
    QWidget * defaultWidget() const;

    /**
    Registers a widget generator with a new instance using the widget type WT.
    A probably existing generator will be replaced.
    */
    template <typename WT>
    void registerWidgetGenerator(const QString &widgetClassName)
    {
        _widgetGenerators.insert( widgetClassName, new WidgetGeneratorTemplate<WT>() );
    }

    /**
    Sets a default widget generator using the widget type WT.
    A probably existing generator will be replaced.
    */
    template <typename WT>
    void registerDefaultGenerator()
    {
        delete _defaultGenerator;
        _defaultGenerator = 0;

        _defaultGenerator = new WidgetGeneratorTemplate<WT>();
    }

private:
    QMap<QString, AbstractWidgetGenerator*>    _widgetGenerators;
    AbstractWidgetGenerator *                  _defaultGenerator;
};

#endif // WIDGETFACTORY_H
