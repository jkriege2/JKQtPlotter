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



/** \file jkvanishqtoolbar.h
  * \ingroup jkqtpbaseplotter
  */

#ifndef jkvanishqtoolbar_H
#define jkvanishqtoolbar_H
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QToolBar>

/** \brief a modified a href="http://doc.trolltech.com/4.5/qtoolbar.html">QToolBar</a> which vanishes when the mouse leaves the toolbar.
 * \ingroup jkqtptools
 */
class LIB_EXPORT JKVanishQToolBar: public QToolBar {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKVanishQToolBar(const QString& title, QWidget* parent=nullptr);
        /** \brief class constructor */
        JKVanishQToolBar(QWidget* parent=nullptr);
        JKQTPGET_SET_MACRO(bool, toolbarVanishes)
    protected:
        bool toolbarVanishes;
        /** \brief this event triggers the vanishing of the toolbar */
        void leaveEvent ( QEvent * /*event*/ );
};
#endif // jkvanishqtoolbar_H
