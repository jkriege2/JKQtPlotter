/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef JKQTPDEBUGGINGTOOLS_H_INCLUDED
#define JKQTPDEBUGGINGTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QString>
#include <QElapsedTimer>
#include <stdexcept>


#ifndef __WINDOWS__
# if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#  define __WINDOWS__
# endif
#endif

#ifndef __LINUX__
# if defined(linux)
#  define __LINUX__
# endif
#endif

#undef JKTOOLS_TIMER_USE_TIME
#if defined(__WINDOWS__)
  #include<windows.h>
#elif defined(__LINUX__)
  #include <sys/time.h>
#else
  #define JKTOOLS_TIMER_USE_TIME
#endif



/** \brief RAII construct that times its lifetime, outputting properly indented qDebug()-message
 * \ingroup jkqtptools_debugging
 */
class JKQTCOMMON_LIB_EXPORT JKQTPAutoOutputTimer : public QElapsedTimer
{
    public:
        explicit JKQTPAutoOutputTimer(const QString& message);
        ~JKQTPAutoOutputTimer();
        void write(const QString& message) const;
    protected:
        QString message;
        QString indent;


};


/** \brief dynamic assertion, throws an exception with the given \a message, when the given condition \a condition evaluates to \c false
 * \ingroup jkqtptools_debugging
 */
#define JKQTPASSERT_M(condition, message) { if (!(condition)) throw std::runtime_error(std::string(message)+" (expression: "+std::string(#condition)+", function: "+std::string(__FUNCTION__)+", file: "+std::string(__FILE__)+":"+std::to_string(__LINE__)+")"); }
/** \brief dynamic assertion, throws an exception with the given \a message, when the given condition \a condition evaluates to \c false
 * \ingroup jkqtptools_debugging
 */
#define JKQTPASSERT(condition) { if (!(condition)) throw std::runtime_error("assertion failed (expression: "+std::string(#condition)+", function: "+std::string(__FUNCTION__)+", file: "+std::string(__FILE__)+":"+std::to_string(__LINE__)+")"); }

#endif // JKQTPDEBUGGINGTOOLS_H_INCLUDED
