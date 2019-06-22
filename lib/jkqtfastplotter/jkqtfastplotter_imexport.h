#ifndef jkqtfastplotter_IMPORT_H
#define jkqtfastplotter_IMPORT_H


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




/*! \def JKQTFASTPLOTTER_LIB_EXPORT
    \ingroup tools

    This define allows to export functions and classes from the jkqtcommon-library when building a dynamic/shared library.
    Usage is as follows:
    \code
        #include "jkqtfastplotter/jkqtfastplotter_imexport.h"

        class JKQTFASTPLOTTER_LIB_EXPORT exportedClassName {
           ...
        };

        JKQTFASTPLOTTER_LIB_EXPORT void exportedFunctionName();
    \endcode
    
    These macros append the appropriate \c __declspec(dllexport) and \c __declspec(dllimport)
    to the function/class body and thus tell windows compilers to export these sysmbols from
    the shared library, or import them from a shared library.
    
    Note that these attributes are only necessary on windows systems!
    
    These macros are controlled by two other macros:
      - \c JKQTFASTPLOTTER_LIB_IN_DLL declares that the application should link against a shared version of 
                                 JKQTFastPlotter, i.e. \c JKQTFastPlotterSharedLib_XYZ .
                                 This needs to be defined while compiling the library and while compiling 
                                 any application linking against  \c JKQTFastPlotterSharedLib_XYZ. 
      - \c JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY is only defined while compiling JKQTFastPlotter into \c JKQTFastPlotterSharedLib_XYZ
                                 and ensures thet the symbols are exported. If it is not defined (e.g. while 
                                 compiling an application), the symbols are imported
    .  

 */

/*! \def JKQTFASTPLOTTER_LIB_IN_DLL 
    \ingroup tools
    \brief declares that the application should link against a shared version of 
           JKQTFastPlotter, i.e. \c JKQTFastPlotterSharedLib_XYZ .
           This needs to be defined while compiling the library and while compiling 
           any application linking against  \c JKQTFastPlotterSharedLib_XYZ. 
*/

/*! \def JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY 
    \ingroup tools
    \brief is only defined while compiling JKQTFastPlotter into \c JKQTFastPlotterSharedLib_XYZ
           and ensures thet the symbols are exported. If it is not defined (e.g. while 
           compiling an application), the symbols are imported
*/

#  ifndef __WINDOWS__
#    if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#      define __WINDOWS__
#    endif
#  endif

#ifdef __WINDOWS__
#  ifdef JKQTFASTPLOTTER_LIB_IN_DLL
#    ifndef JKQTFASTPLOTTER_LIB_EXPORT
#      ifdef JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY
          /* We are building this library */
#        define JKQTFASTPLOTTER_LIB_EXPORT __declspec(dllexport)
#      else
          /* We are using this library */
#        define JKQTFASTPLOTTER_LIB_EXPORT __declspec(dllimport)
#      endif
#    endif
#  else
#    ifndef JKQTFASTPLOTTER_LIB_EXPORT
#      define JKQTFASTPLOTTER_LIB_EXPORT
#    endif
#  endif
#else
#  define JKQTFASTPLOTTER_LIB_EXPORT
#endif







#endif // jkqtfastplotter_IMPORT_H

