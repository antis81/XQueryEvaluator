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

#include "MainApplication.h"

#include <QtGui/QFileOpenEvent>

/**
Constructs a platform independent QApplication variant.
*/
MainApplication::MainApplication(int &argc, char **argv, bool GUIenabled) :
    QApplication(argc, argv, GUIenabled)
{
}

/**
Handles events that are sent to the main application object.
E.g. on OSX it is used to handle associated file extensions correctly.
*/
bool MainApplication::event(QEvent *ev)
{
    bool eaten = false;

    switch (ev->type())
    {
    case QEvent::FileOpen:
        emit ( fileOpened(static_cast<QFileOpenEvent *>(ev)->file()) );
        eaten = true;
        break;

    default:
        eaten = QApplication::event(ev);
        break;
    }

    return eaten;
}
