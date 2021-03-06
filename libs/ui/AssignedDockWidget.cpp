/*
 * XQueryEvaluator - Edit and run XQuery / XSLT scripts.
 * Copyright (C) 2011-2015 by Nils Fenner <nils@macgitver.org>
 *
 * This file is part of XQueryEvaluator.
 *
 * XQueryEvaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XQueryEvaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XQueryEvaluator.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AssignedDockWidget.h"

#include <QtCore/QEvent>
#include <QtWidgets/QLayout>


AssignedDockWidget::AssignedDockWidget(QObject * parent)
    : QObject(parent)
    , _privateDW( new QDockWidget() )
    , _area(Qt::NoDockWidgetArea)
{
    _privateDW->setContentsMargins(0,0,0,0);
    _privateDW->setAttribute(Qt::WA_DeleteOnClose, false);
    _privateDW->setFeatures( QDockWidget::NoDockWidgetFeatures );
    _privateDW->setTitleBarWidget( new QWidget() );
}

AssignedDockWidget::~AssignedDockWidget()
{
    delete _privateDW;
}

QWidget * AssignedDockWidget::contentWidget() const
{
    return _privateDW->widget();
}

void AssignedDockWidget::setContentWidget(QWidget *w)
{
    // setup a layout, when the widget has none doesn't have one
    if ( w->layout() == 0 )
    {
        QWidget *current = w; // remember the original pointer!
        w = new QWidget();
        QLayout * l = new QGridLayout(w);
        l->setContentsMargins(0,0,0,0);
        l->addWidget(current);
    }

    _privateDW->setWidget(w);

    if ( w != 0 )
        w->installEventFilter(this);
}

Qt::DockWidgetArea AssignedDockWidget::area() const
{
    return _area;
}

void AssignedDockWidget::setArea(Qt::DockWidgetArea area)
{
    _area = area;
}

QDockWidget * AssignedDockWidget::dockWidget() const
{
    return _privateDW;
}

bool AssignedDockWidget::eventFilter(QObject *, QEvent *e)
{
    e->accept();

    if ( e->type() == QEvent::Close )
        _privateDW->close();

    return true;
}
