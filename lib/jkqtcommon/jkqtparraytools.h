/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

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
#include "jkqtcommon/jkqtcommon_imexport.h"


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





/*! \brief malloc() for use herein (aligned on some systems!)
    \ingroup jkqtptools_math_array

*/
inline void* jkqtpArrayMalloc(size_t size) {
//    std::cout<<"statisticsMalloc("<<size<<")\n";
#ifdef STATISTICS_TOOLS_USE_QFTOOLS_H
    return qfMalloc(size);
#else
    if (size<=0) return nullptr;
  #ifdef __LINUX__
    #if  !defined(QF_DONT_USE_ALIGNED_MALLOC)
    return aligned_alloc(JKQTP_ALIGNMENT_BYTES, size);
    #else
    return malloc(size);
    #endif
  #else
    #if  !defined(QF_DONT_USE_ALIGNED_MALLOC)
    return _aligned_malloc(size, JKQTP_ALIGNMENT_BYTES);
    #else
    return malloc(size);
    #endif
  #endif
#endif
}

/*! \brief calloc() for use herein (aligned on some systems!)
    \ingroup jkqtptools_math_array

*/
inline void* jkqtpArrayCalloc(size_t num, size_t size) {
//    std::cout<<"statisticsCalloc("<<num<<", "<<size<<")\n";
#ifdef STATISTICS_TOOLS_USE_QFTOOLS_H
    return qfCalloc(num,size);
#else
    if (size*size<=0) return nullptr;
    void* res=jkqtpArrayMalloc(num*size);
    memset(res, 0, num*size);
    return res;
#endif
}

/*! \brief free() for use herein (aligned on some systems!)
    \ingroup jkqtptools_math_array

*/
inline void jkqtpArrayFree(void* data) {
#ifdef STATISTICS_TOOLS_USE_QFTOOLS_H
    qfFree(data);
#else
    if (!data) return;
    #ifdef __LINUX__
        #if !defined(QF_DONT_USE_ALIGNED_MALLOC)
        free(data);
        #else
        free(data);
        #endif
    #else
        #if !defined(QF_DONT_USE_ALIGNED_MALLOC)
        _aligned_free(data);
        #else
        free(data);
        #endif
    #endif
#endif
}


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

    \note use jkqtpArrayFree() to free the memory!!!
*/
template <class T>
inline T* jkqtpArrayDuplicate(const T* dataIn, long long N) {
//    std::cout<<"statisticsDuplicateArray("<<dataIn<<", "<<N<<")\n";
    if (N<=0 || !dataIn) return nullptr;
    T* out=static_cast<T*>(jkqtpArrayMalloc(N*sizeof(T)));
    if (out) memcpy(out, dataIn, N*sizeof(T));
    return out;
}




/*! \brief this class ensures that the given pointer is jkqtpArrayFreed when the class is destroyed.
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
            if (pntr) jkqtpArrayFree(pntr);
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


