#ifndef jkqtp_IMPORT_H
#define jkqtp_IMPORT_H


/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

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




/*! \file jkqtp_imexport.h
    \ingroup tools

    This file allows to export functions and classes from this directory in a dynamic/shared lib and to append the
    correct __declspec() declaration on windows. If you want to export one, make sure you have set
    \code
    #include "jkqtplottertools/jkqtp_imexport.h"

    class JKQTP_LIB_EXPORT exportedClassName {
       ...
    };

    JKQTP_LIB_EXPORT void exportedFunctionName();
    \endcode

    Then when compiling define the preprocessor symbol \c JKQTP_LIB_IN_DLL when exporting AND using the classes/functions.
    When compiling the classes for export, also define the preprocessor symbol \c JKQTP_LIB_EXPORT_LIBRARY .
 */



#  ifndef __WINDOWS__
#    if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#      define __WINDOWS__
#    endif
#  endif

#ifdef __WINDOWS__
#  ifdef JKQTP_LIB_IN_DLL
#    if defined(JKQTP_LIB_EXPORT_LIBRARY)
#      define JKQTP_LIB_EXPORT __declspec(dllexport)
#    else
#      define JKQTP_LIB_EXPORT __declspec(dllimport)
#    endif
#  else
#    define JKQTP_LIB_EXPORT
#  endif
#else
#  define JKQTP_LIB_EXPORT
#endif








#endif // jkqtp_IMPORT_H

