#ifndef JKQTPLOTTER_CONFIGMACROS_H
#define JKQTPLOTTER_CONFIGMACROS_H


/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate: 2015-04-02 13:55:22 +0200 (Do, 02 Apr 2015) $  (revision $Rev: 3902 $)

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


/*! \def JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    \ingroup jkqtpplottersupprt
    \brief This is defined if JKQtPlotter shall be compiled without print-support, or print-support is not available.

    This can be used by surrounding code that is requires print-support with
    \code
    #ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    // ...
    // Code that require Print Support
    // ...
    #endif
    \endcode
*/

#if defined(QT_NO_PRINTPREVIEWDIALOG) || defined(QT_NO_PRINTER) || defined(JKQTPLOTTER_HAS_NO_PRINTSUPPORT)
#  define JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#else
#  ifdef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#    undef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#  endif
#endif




#endif // JKQTPLOTTER_CONFIGMACROS_H

