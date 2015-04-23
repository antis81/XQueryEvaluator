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

#ifndef ASSIGNEDDOCKWIDGET_H
#define ASSIGNEDDOCKWIDGET_H

#include <QtGui/QDockWidget>


/**
@brief
Represents an assigned dock widget.
*/
class AssignedDockWidget : public QObject
{
public:
    explicit AssignedDockWidget(QObject * parent = 0);
    virtual ~AssignedDockWidget();

    QWidget * contentWidget() const;
    void setContentWidget(QWidget *w);

    Qt::DockWidgetArea area() const;
    void setArea(Qt::DockWidgetArea area);

    QDockWidget * dockWidget() const;

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    QDockWidget *           _privateDW;

    QString                 _widgetKey; //!< Class name of the contents widget (CASE SENSITIVE!).
    Qt::DockWidgetArea      _area;
};

#endif // ASSIGNEDDOCKWIDGET_H
