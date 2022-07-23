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



#ifndef JKQTPALGORITHMS_H_INCLUDED
#define JKQTPALGORITHMS_H_INCLUDED
#include "jkqtcommon_statistics_and_math/jkqtcommon_statistics_and_math_imexport.h"




/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools_algorithms

*/
template <class T>
inline void jkqtpSwap(T* a, int l, int r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}



/*! \brief QuickSort (recursive implementation)
    \ingroup jkqtptools_algorithms
    \internal

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T>
inline void jkqtpQuicksort(T* a, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap<T>(a, i, j);
        }
        jkqtpSwap<T>(a, i, r);

        jkqtpQuicksort(a, l, i-1);
        jkqtpQuicksort(a, i+1, r);
    }
}

/*! \brief QuickSort (recursive implementation), sorts \a a2 alongside \a a, using \a a as sort criterion
    \ingroup jkqtptools_algorithms
    \internal

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T, class T2>
inline void jkqtpQuicksortDual(T* a, T2* a2, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap(a, i, j);
            jkqtpSwap(a2, i, j);
        }
        jkqtpSwap(a, i, r);
        jkqtpSwap(a2, i, r);

        jkqtpQuicksortDual(a, a2, l, i-1);
        jkqtpQuicksortDual(a, a2, i+1, r);
    }
}



/*! \brief sort the given array and return the result in \a output (implies a copy!!!)
    \ingroup jkqtptools_algorithms

    \param input array to be sorted
    \param N size of the array input
    \param output if \c !=nullptr data is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.

 */
template <class T>
inline void jkqtpQuicksort(const T* input, long long N, T* output) {
    if ((!input)) return ;
    if (N<=0) return;
    T* data=output;
    memcpy(output, input, N*sizeof(T));
    jkqtpQuicksort(data, 0, N-1);
}




/*! \brief sort the given arrays, using \a input as sort criterion
    \ingroup jkqtptools_algorithms

    \param input array to be sorted (the sort criterion)
    \param input2 array to be sorted (sorted alongside \a input )
    \param N size of the array input

 */
template <class T, class T2>
inline void jkqtpQuicksortDual(T* input, T2* input2, int N) {
    if ((!input)) return ;
    if (N<=0) return;
    jkqtpQuicksortDual(input, input2, 0, N-1);
}

/*! \brief sort the given arrays, using \a input as sort criterion, returns the results in \a output and \a output2 (implied copies of the data!)
    \ingroup jkqtptools_algorithms

    \param input array to be sorted (the sort criterion)
    \param input2 array to be sorted (sorted alongside \a input )
    \param N size of the array input
    \param output result is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.
    \param output2 result is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.

 */
template <class T, class T2>
inline void jkqtpQuicksortDual(const T* input, const T2* input2, int N, T* output, T2* output2) {
    if ((!input)) return ;
    if (N<=0) return;
    T* data=output;
    memcpy(output, input, N*sizeof(T));
    T2* data2=output2;
    memcpy(output2, input2, N*sizeof(T2));
    jkqtpQuicksortDual(data, data2, 0, N-1);
}


#endif // JKQTPALGORITHMS_H_INCLUDED
