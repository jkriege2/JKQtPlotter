/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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
#include "jkqtcommon/jkqtp_imexport.h"
#include <QString>
#include <QElapsedTimer>


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
class JKQTP_LIB_EXPORT JKQTPAutoOutputTimer : public QElapsedTimer
{
    public:
        explicit JKQTPAutoOutputTimer(const QString& message);
        ~JKQTPAutoOutputTimer();
        void write(const QString& message) const;
    protected:
        QString message;
        QString indent;
        static int global_indent;

};


/** \brief dynamic assertion, throws a \c std::runtime_error exception if \a condition is \c false
 * \ingroup jkqtptools_debugging
 *
 * \param condition the condition to check
 * \param message a user-provided error message
 * \param expression the expression (as a string) that was used to calculate \a condition
 * \param file filename where the exception occured
 * \param line line in file \a file where the exception occured
 *
 * \see JKQTPASSERT_M(), JKQTPASSERT() for macros that use this function and automatically stringify the expression and add file and line
 */
inline void jkqtp_assert(bool condition, const std::string& message, const std::string expression, const std::string& file, int line)
{
    if (!condition) {
        throw std::runtime_error(message+" (expression: "+expression+", file: "+file+":"+std::to_string(line)+")");
    }
}

/** \brief dynamic assertion, throws a \c std::runtime_error exception if \a condition is \c false
 * \ingroup jkqtptools_debugging
 *
 * \param condition the condition to check
 * \param message a user-provided error message
 * \param expression the expression (as a string) that was used to calculate \a condition
 * \param file filename where the exception occured
 * \param line line in file \a file where the exception occured
 * \param function calling function
 *
 * \see JKQTPASSERT_M(), JKQTPASSERT() for macros that use this function and automatically stringify the expression and add file and line
 */
inline void jkqtp_assert(bool condition, const std::string& message, const std::string expression, const std::string& file, int line, const std::string& function)
{
    if (!condition) {
        throw std::runtime_error(message+" (expression: "+expression+", function: "+function+", file: "+file+":"+std::to_string(line)+")");
    }
}

/** \brief dynamic assertion, throws a \c std::runtime_error exception if \a condition is \c false
 * \ingroup jkqtptools_debugging
 *
 * \param condition the condition to check
 * \param expression the expression (as a string) that was used to calculate \a condition
 * \param file filename where the exception occured
 * \param line line in file \a file where the exception occured
 * \param function calling function
 *
 * \see JKQTPASSERT_M(), JKQTPASSERT() for macros that use this function and automatically stringify the expression and add file and line
 */
inline void jkqtp_assert(bool condition, const std::string expression, const std::string& file, int line, const std::string& function)
{
    if (!condition) {
        throw std::runtime_error("assertion failed (expression: "+expression+", function: "+function+", file: "+file+":"+std::to_string(line)+")");
    }
}

/** \brief dynamic assertion, throws a \c std::runtime_error exception if \a condition is \c false
 * \ingroup jkqtptools_debugging
 *
 * \param condition the condition to check
 * \param expression the expression (as a string) that was used to calculate \a condition
 * \param file filename where the exception occured
 * \param line line in file \a file where the exception occured
 *
 * \see JKQTPASSERT_M(), JKQTPASSERT() for macros that use this function and automatically stringify the expression and add file and line
 */
inline void jkqtp_assert(bool condition, const std::string expression, const std::string& file, int line)
{
    if (!condition) {
        throw std::runtime_error("assertion failed (expression: "+expression+", file: "+file+":"+std::to_string(line)+")");
    }
}

/** \brief dynamic assertion, throws a \c std::runtime_error exception if \a condition is \c false
 * \ingroup jkqtptools_debugging
 *
 * \param condition the condition to check
 * \param message a user-provided error message
 *
 * \see JKQTPASSERT_M(), JKQTPASSERT()
 */
inline void jkqtp_assert(bool condition, const std::string& message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

/** \brief dynamic assertion, throws an exception with the given \a message, when the given condition \a condition evaluates to \c false
 * \ingroup jkqtptools_debugging
 */
#define JKQTPASSERT_M(condition, message) jkqtp_assert(condition, message, #condition, __FILE__, __LINE__, __FUNCTION__)
/** \brief dynamic assertion, throws an exception with the given \a message, when the given condition \a condition evaluates to \c false
 * \ingroup jkqtptools_debugging
 */
#define JKQTPASSERT(condition) jkqtp_assert(condition, #condition, __FILE__, __LINE__, __FUNCTION__)

#endif // JKQTPDEBUGGINGTOOLS_H_INCLUDED
