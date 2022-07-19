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



/*
  Name: jkqtphighrestimer.cpp
  Copyright: (c) 2007
  Author: Jan krieger <jan@jkrieger.de>, http://www.jkrieger.de/
*/

#include "jkqtphighrestimer.h" // class's header file


JKQTPHighResTimer::JKQTPHighResTimer(): last(), freq(0) {
  start();
}

JKQTPHighResTimer::~JKQTPHighResTimer() = default;

void JKQTPHighResTimer::start(){
  #ifdef __LINUX__
  #endif
  #ifdef __WINDOWS__
      LARGE_INTEGER fr;
      QueryPerformanceFrequency(&fr);
      freq=static_cast<double>(fr.QuadPart);
      QueryPerformanceCounter(&last);
  #else
      last=std::chrono::system_clock::now();
  #endif
};

double JKQTPHighResTimer::getTime(){
  #ifdef __LINUX__
  #endif
  #ifdef __WINDOWS__
      LARGE_INTEGER now;
      QueryPerformanceCounter(&now);
      return (static_cast<double>(now.QuadPart-last.QuadPart)/freq)*1e6;
  #else
    auto no=std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(no-last).count();


  #endif
};


