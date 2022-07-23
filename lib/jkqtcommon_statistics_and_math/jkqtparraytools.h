/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate$  (revision $Rev$)

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


#ifndef JKQTPARRAYTOOLS_H_INCLUDED
#define JKQTPARRAYTOOLS_H_INCLUDED

#include <stdint.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <limits>
#include <vector>
#include <utility>
#include <cfloat>
#include <ostream>
#include <iomanip>
#include <sstream>
#include "jkqtcommon_statistics_and_math/jkqtcommon_statistics_and_math_imexport.h"


#ifdef _OPENMP
# include <omp.h>
#endif

#ifndef __LINUX__
# if defined(linux)
#  define __LINUX__
# endif
#endif

#ifdef __LINUX__
#include <malloc.h>
#include <stdlib.h>
#endif

#ifndef JKQTP_ALIGNMENT_BYTES
#define JKQTP_ALIGNMENT_BYTES 32
#endif




/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools_math_array

*/
template <class T>
inline void jkqtpArraySwap(T* a, long long l, long long r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}

/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools_math_array

*/
template <class T>
inline void jkqtpArraySwapV(std::vector<T>& a, long long l, long long r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}



/*! \brief duplicate an array of data
    \ingroup jkqtptools_math_array

    \note use free() to free the memory!!!
*/
template <class T>
inline T* jkqtpArrayDuplicate(const T* dataIn, long long N) {
//    std::cout<<"statisticsDuplicateArray("<<dataIn<<", "<<N<<")\n";
    if (N<=0 || !dataIn) return nullptr;
    T* out=static_cast<T*>(malloc(N*sizeof(T)));
    if (out) memcpy(out, dataIn, N*sizeof(T));
    return out;
}




/*! \brief this class ensures that the given pointer is freed when the class is destroyed.
    \ingroup jkqtptools_math_array

*/
template<typename T>
class JKQTPArrayScopedPointer {
    public:
        inline explicit JKQTPArrayScopedPointer(T* pnt) {
            pntr=pnt;
        }
        inline explicit JKQTPArrayScopedPointer() {
            pntr=nullptr;
        }
        inline explicit JKQTPArrayScopedPointer(const JKQTPArrayScopedPointer& other) {
            pntr=other.pntr;
        }


        ~JKQTPArrayScopedPointer() {
            if (pntr) free(pntr);
        }


        inline  JKQTPArrayScopedPointer<T>& operator=(T* p) {
             pntr=p;
             return *this;
         }

        inline  JKQTPArrayScopedPointer<T>& operator=(const JKQTPArrayScopedPointer<T>& p) {
             pntr=p.pntr;
             return *this;
         }

        inline T& operator*() const { return *pntr; }
        inline T* operator->() const { return pntr; }
        inline T* data() const { return pntr; }
        inline T& operator[](long long i) {
            return pntr[i];
        }
        inline const T& operator[](long long i) const {
            return pntr[i];
        }



    private:
        T* pntr;
};

#endif // JKQTPARRAYTOOLS_H_INCLUDED


