/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplottergui/jkvanishqtoolbar.h"

JKVanishQToolBar::JKVanishQToolBar(const QString &title, QWidget *parent): QToolBar(title, parent) {
    toolbarVanishes=true;
}

JKVanishQToolBar::JKVanishQToolBar(QWidget *parent): QToolBar(parent){
    toolbarVanishes=true;
}

bool JKVanishQToolBar::doesToolbarVanish() const
{
    return this->toolbarVanishes;
}

void JKVanishQToolBar::setToolbarVanishesEnabled(bool __value)
{
    this->toolbarVanishes = __value;
}

void JKVanishQToolBar::leaveEvent(QEvent *) {
    if (toolbarVanishes) hide();
}
