/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef jkqtpenhancedspinboxes_H_INCLUDED
#define jkqtpenhancedspinboxes_H_INCLUDED
#include "jkqtplotter/jkqtplotter_imexport.h"

#include <QSpinBox>
#include <QDoubleSpinBox>

/*! \brief enhanced QDoubleSpinBox
    \ingroup jkqtptools_qtwidgets

    This class adds an event editingFinished() to QDoubleSpinBox.
*/
class JKQTPLOTTER_LIB_EXPORT JKQTPEnhancedDoubleSpinBox : public QDoubleSpinBox {
        Q_OBJECT
    public:
        JKQTPEnhancedDoubleSpinBox(QWidget* parent=nullptr);
        ~JKQTPEnhancedDoubleSpinBox();
    signals:
        void editingFinished(double value);
    protected slots:
        void intEditingFinished();
};



/*! \brief enhanced QDoubleSpinBox
    \ingroup jkqtptools_qtwidgets

    This class adds an event editingFinished() to QDoubleSpinBox.
*/
class JKQTPLOTTER_LIB_EXPORT JKQTPEnhancedSpinBox : public QSpinBox {
        Q_OBJECT
    public:
        JKQTPEnhancedSpinBox(QWidget* parent=nullptr);
        ~JKQTPEnhancedSpinBox();
    signals:
        void editingFinished(int value);
    protected slots:
        void intEditingFinished();
};

#endif // jkqtpenhancedspinboxes_H_INCLUDED
