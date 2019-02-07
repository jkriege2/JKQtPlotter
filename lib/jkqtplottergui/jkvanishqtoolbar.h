/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)



    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef jkvanishqtoolbar_H
#define jkvanishqtoolbar_H
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QToolBar>

/** \brief a modified <a href="http://doc.qt.io/qt-5/qtoolbar.html">QToolBar</a> which vanishes when the mouse leaves the toolbar.
 * \ingroup jkqtptools_qtwidgets
 *
 * \image html jkqtplotter_toolbar_hidden.png "Hidden Toolbar"
 *
 * \image html jkqtplotter_toolbar_shown.png "Shown Toolbar"
 *
 * \image html jkqtvanishtoolbar.gif
 *
 */
class JKQTP_LIB_EXPORT JKVanishQToolBar: public QToolBar {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKVanishQToolBar(const QString& title, QWidget* parent=nullptr);
        /** \brief class constructor */
        JKVanishQToolBar(QWidget* parent=nullptr);
        /** \brief is the auto-vanishing feature of this toolbar activated? */
        bool doesToolbarVanish() const;
    public slots:
        /** \brief enable or disable the auto-vanishing feature of this toolbar  */
        void setToolbarVanishesEnabled(bool __value);
    protected:
        /** \brief is the auto-vanishing feature of this toolbar activated? */
        bool toolbarVanishes;
        /** \brief this event triggers the vanishing of the toolbar */
        void leaveEvent ( QEvent * /*event*/ );
};
#endif // jkvanishqtoolbar_H
