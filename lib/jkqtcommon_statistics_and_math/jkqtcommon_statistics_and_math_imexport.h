#ifndef jkqtcommon_statistics_and_math_IMPORT_H
#define jkqtcommon_statistics_and_math_IMPORT_H

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




/*! \def JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT
    \ingroup tools

    This define allows to export functions and classes from the jkqtcommon_statistics_and_math-library when building a dynamic/shared library.
    Usage is as follows:
    \code
        #include "jkqtcommon_statistics_and_math_imexport/jkqtcommon_statistics_and_math_imexport.h"

        class JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT exportedClassName {
           ...
        };

        JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT void exportedFunctionName();
    \endcode
    
    These macros append the appropriate \c Q_DECL_EXPORT and \c Q_DECL_IMPORT
    to the function/class body and thus tell windows compilers to export these sysmbols from
    the shared library, or import them from a shared library.
    
    Note that these attributes are only necessary on windows systems!
    
    These macros are controlled by two other macros:
      - \c JKQTCOMMON_STATISTICS_AND_MATH_LIB_IN_DLL declares that the application should link against a shared version of 
                                 JKQTCommonStatisticsAndMath, i.e. \c JKQTCommonStatisticsAndMathSharedLib_XYZ .
                                 This needs to be defined while compiling the library and while compiling 
                                 any application linking against  \c JKQTCommonStatisticsAndMathSharedLib_XYZ. 
      - \c JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT_LIBRARY is only defined while compiling JKQTCommonStatisticsAndMath into \c JKQTCommonStatisticsAndMathSharedLib_XYZ
                                 and ensures thet the symbols are exported. If it is not defined (e.g. while 
                                 compiling an application), the symbols are imported
    .  

 */

/*! \def JKQTCOMMON_STATISTICS_AND_MATH_LIB_IN_DLL 
    \ingroup tools
    \brief declares that the application should link against a shared version of 
           JKQTCommonStatisticsAndMath, i.e. \c JKQTCommonStatisticsAndMathSharedLib_XYZ .
           This needs to be defined while compiling the library and while compiling 
           any application linking against  \c JKQTCommonStatisticsAndMathSharedLib_XYZ. 
*/

/*! \def JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT_LIBRARY 
    \ingroup tools
    \brief is only defined while compiling JKQTCommonStatisticsAndMath into \c JKQTCommonStatisticsAndMathSharedLib_XYZ
           and ensures thet the symbols are exported. If it is not defined (e.g. while 
           compiling an application), the symbols are imported
*/

#include <QtCore/QtGlobal>
#  ifdef JKQTCOMMON_STATISTICS_AND_MATH_LIB_IN_DLL
#    ifndef JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT
#      ifdef JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT_LIBRARY
          /* We are building this library */
#        define JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT Q_DECL_EXPORT
#      else
          /* We are using this library */
#        define JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT Q_DECL_IMPORT
#      endif
#    endif
#  else
#    ifndef JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT
#      define JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT
#    endif
#  endif



#endif // jkqtcommon_statistics_and_math_IMPORT_H

