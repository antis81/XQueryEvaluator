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

#include "TextEditMetaBorder.h"

#include "TextEditBase.h"


/**
Creates a meta border instance that takes care of the doc's line numbers.
The parent widget is used like any QWidget's parent.
*/
TextEditMetaBorder::TextEditMetaBorder( TextEditBase * editor )
    : QWidget(editor)
    , _editor(editor)
{
}

TextEditMetaBorder::~TextEditMetaBorder()
{
}

QSize TextEditMetaBorder::sizeHint() const
{
    return QSize( _editor->metaBorderWidth(), 0 );
}

void TextEditMetaBorder::paintEvent(QPaintEvent *e)
{
    _editor->metaBorderPaintEvent( e );
}
