/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>) (DKFZ) & IWR, University of Heidelberg

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


void JKQTPHighResTimer::test(double* mean, double* stddev, unsigned long* histogram, double* histogram_x, unsigned long histogram_size){
  unsigned long runs=1000000;
  double* h=static_cast<double*>(malloc(runs*sizeof(double)));
  *mean = 0;
  *stddev = 0;
  /* time measurement */
  double l=getTime(), n;
  for (unsigned long i=0; i<runs; i++) {
    n=getTime();
    h[i]=n-l;
    l=n;
    //printf("%lf\n",h[i]);
  }
  /* end measurement */

  /* compute mean value */
  double mymin=h[0];
  double mymax=h[0];
  for (unsigned long i=1; i<runs; i++) {
    *mean+=h[i]/(runs-1);
    if (h[i]>mymax) mymax=h[i];
    if (h[i]<mymin) mymin=h[i];
  }
  //printf("%lf   %lf\n", mymin, mymax);
  double binwidth=0;
  if (histogram_size>0) {
    binwidth=(mymax-mymin)/(histogram_size-1);
    for (unsigned int i=0; i<histogram_size; i++) {
      histogram[i]=0;
      histogram_x[i]=mymin+i*binwidth;
    }
  }

  /* compute standard deviation */
  for (unsigned long i=1; i<runs; i++) {
    (*stddev)=(*stddev)+(static_cast<double>(h[i])-*mean)*(static_cast<double>(h[i])-*mean)/static_cast<double>(runs-2);
    if (histogram_size>0) {
      unsigned long bin=(unsigned int)floor((h[i]-mymin)/binwidth);
      if (/*bin>=0 &&*/ bin<histogram_size) histogram[bin]++;
    }
  }
  *stddev=sqrt(*stddev);
  free(h);
};
