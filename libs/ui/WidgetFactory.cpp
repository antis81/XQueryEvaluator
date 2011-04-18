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
