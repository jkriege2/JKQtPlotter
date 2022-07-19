/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>) (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate: 2015-06-10 19:19:10 +0200 (Mi, 10 Jun 2015) $  (revision $Rev: 3976 $)

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



#include <cmath>
#include <cstdlib>
#include <iostream>
#include <chrono>

#include "jkqtcommon_imexport.h"

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

#if defined(__WINDOWS__)
  #include<windows.h>
#elif defined(__LINUX__)
  #include <sys/time.h>
#else
  #include <sys/time.h>
#endif



#ifndef JKQTPHIGHRESTIMER_H
#define JKQTPHIGHRESTIMER_H


/**
 * \brief This class implements a high resolution timer capable of measuring time intervals with a resolution
 *        of some microseconds
 * \ingroup jkqtptools_debugging
 *
 * \attention Note that this is a MS Windows specific implementation od a high-resolution timer using some of
 * windows' API methods (namely \c QueryPerformanceCounter() and \c QueryPerformanceFrequency() ). So if you want
 * to use this class on non-win32 systems you will have to find a way to implement it for your system!!!
 *
 * \attention Also note that a standard windows system is NOT a real time OS. So do not expect to get a high accuracy
 * when timing operations using this timer. It gives you an accurate time stamp, but it can not guarantee when
 * the code is executed!
 *
 *
 * The timer works very simple:
 *   - you can start the timer with start() which means you set a time=0
 *   - then you can query the time difference to the last call of start() by using getTime().
 * .
 *
 *
 * \par win32 implementation issues:
 * To implement this timer on windows systems I use two API calls from the windows kernel. They are:
 *   - <a href="http://msdn2.microsoft.com/en-us/library/ms644904.aspx">QueryPerformanceCounter()</a>
 *   - <a href="http://msdn2.microsoft.com/en-us/library/ms644905.aspx">QueryPerformanceFrequency()</a>
 * .
 * \n
 * The first one is used to read times: In the start() method we simply save the current counter value to a variable.
 * In getTime() we can then again use QueryPerformanceCounter() to get the current counter value and then calculate
 * the difference between these two. Using QueryPerformanceFrequency() we can calculate the time difference in usecs
 * from the counter value difference using:
 *   \f[ \Delta t=\frac{N_{\mbox{now}}-N_{\mbox{start}}}{\mbox{\texttt{QueryPerformanceFrequency()}}}\cdot 10^{6} \f]
 *
 */
class JKQTCOMMON_LIB_EXPORT JKQTPHighResTimer {
  protected:
#ifdef __WINDOWS__
        /** \brief internal: time stamp of the last call of start() */
        LARGE_INTEGER last;
#else
        std::chrono::system_clock::time_point last;
#endif
        /** \brief internal: timer frequency */
        double freq;
    public:
        /** \brief class constructor. */
        JKQTPHighResTimer();
        /** \brief class destructor */
        ~JKQTPHighResTimer();
        /** \brief start the timer */
        void start();
        /** \brief get the time since the last call of start() in microseconds */
        double getTime();
};





#endif // JKQTPHIGHRESTIMER_H
