/**
**    Copyright 2011 by Nils Fenner
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

#include "MainApplication.h"

#include <QtGui/QFileOpenEvent>


MainApplication::MainApplication(int argc, char ** argv) :
    QApplication(argc, argv)
{
}

bool MainApplication::event(QEvent *ev)
{
    bool eaten;

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
