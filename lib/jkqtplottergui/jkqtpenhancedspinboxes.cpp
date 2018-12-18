/*
Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    

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





/** \file jkqtpenhancedspinboxes.cpp
  * \ingroup jkqtptools
  */

#include "jkqtplottergui/jkqtpenhancedspinboxes.h"





JKQTPEnhancedDoubleSpinBox::JKQTPEnhancedDoubleSpinBox(QWidget *parent):
    QDoubleSpinBox(parent)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

JKQTPEnhancedDoubleSpinBox::~JKQTPEnhancedDoubleSpinBox()
{
    disconnect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

void JKQTPEnhancedDoubleSpinBox::intEditingFinished()
{
    emit editingFinished(value());
}


JKQTPEnhancedSpinBox::JKQTPEnhancedSpinBox(QWidget *parent):
    QSpinBox(parent)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

JKQTPEnhancedSpinBox::~JKQTPEnhancedSpinBox()
{
    disconnect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

void JKQTPEnhancedSpinBox::intEditingFinished()
{
    emit editingFinished(value());
}


