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


#ifndef JKQTPSTATISTICSTOOLS_H_INCLUDED
#define JKQTPSTATISTICSTOOLS_H_INCLUDED

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
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtcommon/jkqtplinalgtools.h"
#include "jkqtcommon/jkqtparraytools.h"
#include "jkqtcommon/jkqtptoolsdebugging.h"





/*! \brief calculates the average of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return Average of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    This function implements:
      \f[ \overline{X}=\frac{1}{N}\cdot\sum\limits_{i=1}^{N}X_i \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatAverage(InputIt first, InputIt last, size_t* Noutput=nullptr) {
    double sum=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+v;
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (NN<=0) return JKQTP_DOUBLE_NAN;
    else return sum/static_cast<double>(NN);
}



/*! \brief calculates the weighted average of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam InputWeightIt standard iterator type of \a firstWeight
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param firstWeight iterator pointing to the first item in the weights dataset \f$ w_i \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return weighted average of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    This function implements:
      \f[ \overline{X}=\frac{\sum\limits_{i=1}^{N}w_i\cdot X_i}{\sum\limits_{i=1}^{N}w_i} \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class InputWeightIt>
inline double jkqtpstatWeightedAverage(InputIt first, InputIt last, InputWeightIt firstWeight, size_t* Noutput=nullptr) {
    double sum=0;
    double sumW=0;
    size_t NN=0;
    auto itW=firstWeight;
    for (auto it=first; it!=last; ++it,++itW) {
        const double v=jkqtp_todouble(*it);
        const double w=jkqtp_todouble(*itW);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+v*w;
            sumW=sumW+w;
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (NN<=0) return JKQTP_DOUBLE_NAN;
    else return sum/sumW;
}



/*! \brief calculates the number of valid values in the given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \return number of valid values between \a first and \a last (excluding invalid doubles).

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline size_t jkqtpstatCount(InputIt first, InputIt last) {
    double sum=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+v;
            NN++;
        }
    }
    return NN;
}




/*! \brief calculates the minimum and maximum values in the given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] min receives the minimum element value
    \param[out] max receives the maximum element value
    \param[out] minPos receives the location of the minimum element value
    \param[out] maxPos receives the location of the minimum maximum element value
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline void jkqtpstatMinMax(InputIt first, InputIt last, double& min, double& max, InputIt* minPos=nullptr, InputIt* maxPos=nullptr, size_t* Noutput=nullptr) {
    size_t NN=0;
    bool firstV=true;
    InputIt minp=last;
    InputIt maxp=last;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            if (firstV) {
                min=v;
                max=v;
                minp=it;
                maxp=it;
                firstV=false;
            } else {
                if (v<min) {
                    min=v;
                    minp=it;
                }
                if (v>max) {
                    max=v;
                    maxp=it;
                }
            }
            NN++;
        }
    }
    if (NN<=0) {
        min=JKQTP_DOUBLE_NAN;
        max=JKQTP_DOUBLE_NAN;
    }
    if (Noutput) *Noutput=NN;
    if (minPos) *minPos=minp;
    if (maxPos) *maxPos=maxp;
}





/*! \brief calculates the minimum value in the given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] minPos receives the location of the minimum element value
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the minimum value from the given range

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatMinimum(InputIt first, InputIt last, InputIt* minPos=nullptr, size_t* Noutput=nullptr) {
    size_t NN=0;
    bool firstV=true;
    InputIt minp=last;
    double min=JKQTP_DOUBLE_NAN;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            if (firstV) {
                min=v;
                minp=it;
                firstV=false;
            } else {
                if (v<min) {
                    min=v;
                    minp=it;
                }
            }
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (minPos) *minPos=minp;
    return min;
}


/*! \brief calculates the maximum value in the given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] maxPos receives the location of the maximum element value
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the maximum value from the given range

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatMaximum(InputIt first, InputIt last, InputIt* maxPos=nullptr, size_t* Noutput=nullptr) {
    size_t NN=0;
    bool firstV=true;
    InputIt maxp=last;
    double max=JKQTP_DOUBLE_NAN;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            if (firstV) {
                max=v;
                maxp=it;
                firstV=false;
            } else {
                if (v>max) {
                    max=v;
                    maxp=it;
                }
            }
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (maxPos) *maxPos=maxp;
    return max;
}



/*! \brief calculates the sum of a given data range \a first ... \a last of values,
           modifying each value with a given functor \a modifierFunctor before accumulating
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam FF a functor type
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param modifierFunctor the function to apply to each element in the range before summation (of type \a FF )
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return Sum of modified data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sum(X)=\cdot\sum\limits_{i=1}^{N}\mbox{modifierFunctor}(X_i) \f]

    This function allows to e.g. calculate the sum of squares by calling
    \code
        jkqtpstatModifiedSum(first, last, [](double v) { return v*v; });
        jkqtpstatModifiedSum(first, last, &jkqtp_sqr<double>);
    \endcode

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class FF>
inline double jkqtpstatModifiedSum(InputIt first, InputIt last, FF modifierFunctor, size_t* Noutput=nullptr) {
    double sum=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+modifierFunctor(v);
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (NN<=0) return 0;
    else return sum;
}



/*! \brief calculates the sum of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return Sum of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sum(X)=\cdot\sum\limits_{i=1}^{N}X_i \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatSum(InputIt first, InputIt last, size_t* Noutput=nullptr) {
    return jkqtpstatSum(first, last, &jkqtp_identity<double>, Noutput);
}

/*! \brief calculates the sum of squares of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return Sum of squares of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sum(X)=\cdot\sum\limits_{i=1}^{N}X_i^2 \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatSumSqr(InputIt first, InputIt last, size_t* Noutput=nullptr) {
    return jkqtpstatSum(first, last, &jkqtp_sqr<double>, Noutput);
}


/*! \brief calculates the vector of cummulative (or partial) sums of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] output This iterator is used to store the results, use e.g. a std::back_inserter
    \return vector of cummulative (or partial) sums returned between \a first and \a last (excluding invalid doubles).
            For invalid values, the last sum is re-inserted, so the returned vector has the same number of entries
            as the range \a first ... \a last

    This function implements:
      \f[ \sum(X)_j=\cdot\sum\limits_{i=1}^{j}X_i \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatCumSum(InputIt first, InputIt last, OutputIt output) {
    double sum=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+v;
        }
        *++output=sum;
    }
}


/*! \brief filters the given data range \a first ... \a last for good floats (using JKQTPIsOKFloat() )
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] output This iterator is used to store the results, use e.g. a std::back_inserter
    \return number of elementes put into \a output

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class OutputIt>
inline size_t jkqtpstatFilterGoodFloat(InputIt first, InputIt last, OutputIt output) {
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            *++output=v;
            NN++;
        }
    }
    return NN;
}










/*! \brief calculates the variance of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam InputWeightIt standard iterator type of \a firstWeight
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return Variance of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sigma_X=\text{Var}(X)=\frac{1}{N-1}\cdot\sum\limits_{i=1}^{N}(X_i-\overline{X})^2=\frac{1}{N-1}\cdot\left(\sum_{i=1}^NX_i^2-\frac{1}{N}\cdot\left(\sum_{i=1}^NX_i\right)^2\right) \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatVariance(InputIt first, InputIt last, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    double sum=0;
    double sum2=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+v;
            sum2=sum2+v*v;
            NN++;
        }
    }
    if (averageOut) {
        if (NN<=0) *averageOut=JKQTP_DOUBLE_NAN;
        else *averageOut=sum/static_cast<double>(NN);
    }
    if (Noutput) *Noutput=NN;
    if (NN<=1) return 0;
    else return ( sum2 - sum*sum/static_cast<double>(NN) ) / static_cast<double>(NN-1);
}



/*! \brief calculates the variance of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return standard deviation of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sigma_X=\sqrt{\frac{1}{N-1}\cdot\sum\limits_{i=1}^{N}(X_i-\overline{X})^2}= \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatStdDev(InputIt first, InputIt last, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    return sqrt(jkqtpstatVariance(first, last, averageOut, Noutput));
}




/*! \brief calculates the weighted variance of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam InputWeightIt standard iterator type of \a firstWeight
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param firstWeight iterator pointing to the first item in the weights dataset \f$ w_i \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return weighted standard deviation of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \text{Var}(v)=\frac{\sum\limits_{i=1}^{N}w_i\cdot (v_i-\overline{v})^2}{\sum\limits_{i=1}^{N}w_i} \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class InputWeightIt>
inline double jkqtpstatWeightedVariance(InputIt first, InputIt last, InputWeightIt firstWeight, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    double avg=jkqtpstatWeightedAverage(first, last, firstWeight);
    double sum2=0;
    double sumW=0;
    size_t NN=0;
    auto itW=firstWeight;
    for (auto it=first; it!=last; ++it,++itW) {
        const double v=jkqtp_todouble(*it)-avg;
        const double w=jkqtp_todouble(*itW);
        if (JKQTPIsOKFloat(v)) {
            sum2=sum2+v*v*w;
            sumW=sumW+w;
            NN++;
        }
    }
    if (averageOut) *averageOut=avg;
    if (Noutput) *Noutput=NN;
    if (NN<=0) return 0;
    else return sum2/sumW;
}



/*! \brief calculates the weighted standard deviation of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam InputWeightIt standard iterator type of \a firstWeight
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param firstWeight iterator pointing to the first item in the weights dataset \f$ w_i \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return weighted standard deviation of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \sigma_v=\sqrt{\frac{\sum\limits_{i=1}^{N}w_i\cdot (v_i-\overline{v})^2}{\sum\limits_{i=1}^{N}w_i}} \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt, class InputWeightIt>
inline double jkqtpstatWeightedStdDev(InputIt first, InputIt last, InputWeightIt firstWeight, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    return sqrt(jkqtpstatWeightedVariance(first, last, firstWeight, averageOut, Noutput));
}




/*! \brief calculates the skewness of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return skewness \f$ \gamma_1 \f$ of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
      \f[ \gamma_1=\mathbb{E}\left[\left(\frac{X-\mu}{\sigma}\right)^3\right]= \frac{m_3}{m_2^{3/2}}  = \frac{\frac{1}{n} \sum_{i=1}^n (x_i-\overline{x})^3}{\left(\frac{1}{n} \sum_{i=1}^n (x_i-\overline{x})^2\right)^{3/2}} \f]
    where \f$\mu\f$ is the mean and \f$\sigma\f$ the standard deviation of a random variable \f$X\f$ and \f$\overline{x}\f$ is the average (calculated using jkqtpstatAverage() ) of
    the input dataset \f$ x_i\f$.

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatSkewness(InputIt first, InputIt last, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    double avg=jkqtpstatAverage(first, last);
    double sum3=0;
    double sum2=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it)-avg;
        if (JKQTPIsOKFloat(v)) {
            sum3=sum3+jkqtp_cube(v);
            sum2=sum2+jkqtp_sqr(v);
            NN++;
        }
    }
    if (averageOut) *averageOut=avg;
    if (Noutput) *Noutput=NN;
    if (NN<=0) return 0;
    const double down=jkqtp_cube(sum2/double(NN));
    return sum3/double(NN)/sqrt(down);
}




/*! \brief calculates the given central  moment \f$ \langle (X-\mu)^o\rangle \f$ of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param order oder \f$ o \f$ of the central moment \f$ \langle (X-\mu)^o\rangle \f$
    \param[out] averageOut returns (optionally) the average of the dataset
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the given central  moment \f$ \langle (X-\mu)^o\rangle \f$ of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
        \f[ \langle (X-\mu)^o\rangle= \mathbb{E}\left[\left(X-\mu\right)^o\right] \f]
    where \f$\mu\f$ is the mean of a random variable \f$X\f$ and \f$\overline{x}\f$ is the average (calculated using jkqtpstatAverage() ) of
    the input dataset \f$ x_i\f$.

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatCentralMoment(InputIt first, InputIt last, int order, double* averageOut=nullptr, size_t* Noutput=nullptr) {
    double avg=jkqtpstatAverage(first, last);
    double sum=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it)-avg;
        if (JKQTPIsOKFloat(v)) {
            sum=sum+pow(v, order);
            NN++;
        }
    }
    if (averageOut) *averageOut=avg;
    if (Noutput) *Noutput=NN;
    if (NN<=0) return 0;
    return sum/double(NN);
}




/*! \brief calculates the given (non-central)  moment \f$ \langle X^o\rangle \f$ of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param order oder \f$ o \f$ of the central moment \f$ \langle X^o\rangle \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the given moment \f$ \langle X^o\rangle \f$ of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, 0 is returned

    This function implements:
        \f[ \langle X^n\rangle= \mathbb{E}\left[X^n\right] \f]
    where \f$\mu\f$ is the mean of a random variable \f$X\f$ and \f$\overline{x}\f$ is the average (calculated using jkqtpstatAverage() ) of
    the input dataset \f$ x_i\f$.

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatMoment(InputIt first, InputIt last, int order, size_t* Noutput=nullptr) {
    double sum=0;
    size_t NN=0;
    for (auto it=first; it!=last; ++it) {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            sum=sum+pow(v, order);
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (NN<=0) return 0;
    return sum/double(NN);
}




/*! \brief calculate empirical (Pearson's) correlation coefficient between two given data ranges \a first1 ... \a last1 and  \a first2 ... \a last2
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt1 standard iterator type of \a first1 and \a last1.
    \tparam InputIt2 standard iterator type of \a first2 and \a last2.
    \param first1 iterator pointing to the first item in the first dataset to use \f$ X_1 \f$
    \param last1 iterator pointing behind the last item in the first dataset to use \f$ X_N \f$
    \param first2 iterator pointing to the second item in the first dataset to use \f$ Y_1 \f$
    \param[out] averageOut1 returns (optionally) the average of the first dataset \f$ X_i \f$
    \param[out] averageOut2 returns (optionally) the average of the second dataset \f$ Y_i \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return pearson's correlation coefficient
            If the given range \a first1 ... \a last1 is empty, JKQTP_DOUBLE_NAN is returned

    This function implements:
        \f[ \text{Kor}(x,y)=\frac{\sum\limits_{i=0}^{N-1}(x_i-\overline{x})(y_i-\overline{y})}{\sqrt{\sum\limits_{i=0}^{N-1}(x_i-\overline{x})^2\cdot\sum\limits_{i=0}^{N-1}(y_i-\overline{y})^2}} \f]

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
*/
template <class InputIt1,class InputIt2>
inline double jkqtpstatCorrelationCoefficient(InputIt1 first1, InputIt1 last1, InputIt2 first2, double* averageOut1=nullptr, double* averageOut2=nullptr, size_t* Noutput=nullptr) {
    double xbar=0;
    double ybar=0;
    size_t NN=0;
    auto it2=first2;
    for (auto it=first1; it!=last1; ++it,++it2) {
        const double xm=jkqtp_todouble(*it);
        const double ym=jkqtp_todouble(*it2);
        if (JKQTPIsOKFloat(xm) && JKQTPIsOKFloat(ym)) {
            xbar=xbar+xm;
            ybar=ybar+ym;
            NN++;
        }
    }
    if (Noutput) *Noutput=NN;
    if (averageOut1) {
        if (NN<=0) *averageOut1=JKQTP_DOUBLE_NAN;
        else *averageOut1=xbar/static_cast<double>(NN);
    }
    if (averageOut2) {
        if (NN<=0) *averageOut2=JKQTP_DOUBLE_NAN;
        else *averageOut2=ybar/static_cast<double>(NN);
    }
    if (NN<=0) return JKQTP_DOUBLE_NAN;

    xbar=xbar/NN;
    ybar=ybar/NN;
    double sumxy=0;
    double sumx=0;
    double sumy=0;
    it2=first2;
    for (auto it=first1; it!=last1; ++it,++it2) {
        const double xm=jkqtp_todouble(*it);
        const double ym=jkqtp_todouble(*it2);
        if (JKQTPIsOKFloat(xm) && JKQTPIsOKFloat(ym)) {
            sumxy=sumxy+xm*ym;
            sumx=sumx+xm*xm;
            sumy=sumy+ym*ym;
        }
    }
    return sumxy/sqrt(sumx*sumy);
}




/*! \brief calculates the median of a given sorted (!) data vector
    \ingroup jkqtptools_math_statistics_basic

    \tparam TVector a type, compatible with std::vector (i,e, providing size(), []-element access and iterators)
    \param data a sorted vector with values
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the median of \a data
            If \a data is empty, NAN is returned

*/
template <class TVector>
inline double jkqtpstatMedianOfSortedVector(const TVector& data, size_t* Noutput=nullptr) {
    if (data.size()<=0) {
        if (Noutput) *Noutput=0;
        return JKQTP_DOUBLE_NAN;
    } else {
        if (Noutput) *Noutput=data.size();
        if (data.size()==1) return data[0];
        else if (data.size()%2==0) return (data[(data.size()-1)/2]+data[(data.size()-1)/2+1])/2.0;
        else return data[(data.size()-1)/2];
    }
}





/*! \brief calculates the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range)) of a sorted vector
    \ingroup jkqtptools_math_statistics_basic

    \tparam TVector a type, compatible with std::vector (i,e, providing size(), []-element access and iterators)
    \param data a sorted vector with values
    \param[out] minimum optionally returns the minimum value of the array
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!
    \param[out] median optionally returns the median value of the array
    \param[out] maximum optionally returns the maximum value of the array
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param[out] quantile1 optionally returns the first quantile of the array (specified by \a quantile1Spec )
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param[out] quantile2 optionally returns the second quantile of the array (specified by \a quantile2Spec )
    \param[out] IQR interquartile range, i.e. the range between \a quantile1 and \a quantile2
    \param[out] IQRSignificance significance range of the interquartile range, calculated as \f[ 2\cdot\frac{1.58\cdot \mbox{IQR}}{\sqrt{N}} \f] \see https://en.wikipedia.org/wiki/Box_plot
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstatAddVBoxplotAndOutliers, jkqtpstatAddHBoxplotAndOutliers, jkqtpstatAddVBoxplot, jkqtpstatAddHBoxplot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class TVector>
inline void jkqtpstat5NumberStatisticsOfSortedVector(const TVector& data, double* minimum=nullptr, double minimumQuantile=0, double* median=nullptr, double* maximum=nullptr, double maximumQuantile=1, double* quantile1=nullptr, double quantile1Spec=0.25, double* quantile2=nullptr, double quantile2Spec=0.75, double* IQR=nullptr, double* IQRSignificance=nullptr, size_t* Noutput=nullptr) {
    if (data.size()<=0) {
        if (minimum) *minimum=JKQTP_DOUBLE_NAN;
        if (maximum) *maximum=JKQTP_DOUBLE_NAN;
        if (median)  *median=JKQTP_DOUBLE_NAN;
        if (quantile1)  *quantile1=JKQTP_DOUBLE_NAN;
        if (quantile1)  *quantile1=JKQTP_DOUBLE_NAN;
        if (Noutput) *Noutput=0;
    } else {
        const double qmin=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(minimumQuantile*static_cast<double>(data.size()-1)), data.size()-1)];
        const double qmax=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(maximumQuantile*static_cast<double>(data.size()-1)), data.size()-1)];
        if (minimum) *minimum=qmin;
        if (maximum) *maximum=qmax;
        if (median) {
            *median= jkqtpstatMedianOfSortedVector(data);
        }
        const double q1=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(quantile1Spec*static_cast<double>(data.size()-1)), data.size()-1)];
        const double q2=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(quantile2Spec*static_cast<double>(data.size()-1)), data.size()-1)];
        if (quantile1) {
            *quantile1=q1;
        }
        if (quantile2) {
            *quantile2=q2;
        }
        if (IQR) {
            *IQR=q2-q1;
        }
        if (IQRSignificance) {
            *IQRSignificance=2.0*(1.58*(q2-q1))/sqrt(static_cast<double>(data.size()));
        }
        if (Noutput) *Noutput=data.size();
    }
}

/*! \brief calculates the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range)) of a sorted vector
    \ingroup jkqtptools_math_statistics_basic

    \tparam TVector a type, compatible with std::vector (i,e, providing size(), []-element access and iterators)
    \param data a sorted vector with values
    \param outliersout output iterator that receives the outliers, smaller than minimum and larger than maximum
    \param[out] minimum optionally returns the minimum value of the array
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param[out] median optionally returns the median value of the array
    \param[out] maximum optionally returns the maximum value of the array
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param[out] quantile1 optionally returns the first quantile of the array (specified by \a quantile1Spec )
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param[out] quantile2 optionally returns the second quantile of the array (specified by \a quantile2Spec )
    \param[out] IQR interquartile range, i.e. the range between \a quantile1 and \a quantile2
    \param[out] IQRSignificance significance range of the interquartile range, calculated as \f[ 2\cdot\frac{1.58\cdot \mbox{IQR}}{\sqrt{N}} \f] \see https://en.wikipedia.org/wiki/Box_plot
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstatAddVBoxplotAndOutliers, jkqtpstatAddHBoxplotAndOutliers, jkqtpstatAddVBoxplot, jkqtpstatAddHBoxplot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class TVector, class OutputIt>
inline void jkqtpstat5NumberStatisticsAndOutliersOfSortedVector(const TVector& data, OutputIt outliersout, double* minimum=nullptr, double minimumQuantile=0, double* median=nullptr, double* maximum=nullptr, double maximumQuantile=1, double* quantile1=nullptr, double quantile1Spec=0.25, double* quantile2=nullptr, double quantile2Spec=0.75, double* IQR=nullptr, double* IQRSignificance=nullptr, size_t* Noutput=nullptr) {
    if (data.size()<=0) {
        if (minimum) *minimum=JKQTP_DOUBLE_NAN;
        if (maximum) *maximum=JKQTP_DOUBLE_NAN;
        if (median)  *median=JKQTP_DOUBLE_NAN;
        if (quantile1)  *quantile1=JKQTP_DOUBLE_NAN;
        if (quantile1)  *quantile1=JKQTP_DOUBLE_NAN;
        if (Noutput) *Noutput=0;
    } else {
        const double qmin=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(minimumQuantile*static_cast<double>(data.size()-1)), data.size()-1)];
        const double qmax=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(maximumQuantile*static_cast<double>(data.size()-1)), data.size()-1)];
        if (minimum) *minimum=qmin;
        if (maximum) *maximum=qmax;
        for (auto it=data.begin(); it!=data.end(); ++it) {
            if (*it<qmin || *it>qmax) {
                *++outliersout=*it;
            }
        }
        if (median) {
            *median= jkqtpstatMedianOfSortedVector(data);
        }
        const double q1=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(quantile1Spec*static_cast<double>(data.size()-1)), data.size()-1)];
        const double q2=data[jkqtp_bounded<size_t>(0, static_cast<size_t>(quantile2Spec*static_cast<double>(data.size()-1)), data.size()-1)];
        if (quantile1) {
            *quantile1=q1;
        }
        if (quantile2) {
            *quantile2=q2;
        }
        if (IQR) {
            *IQR=q2-q1;
        }
        if (IQRSignificance) {
            *IQRSignificance=2.0*(1.58*(q2-q1))/sqrt(static_cast<double>(data.size()));
        }
        if (Noutput) *Noutput=data.size();
    }
}



/*! \brief calculates the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range)) of a given data range \a first ... \a last (5-value statistics, e.g. used for boxplots)
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] minimum optionally returns the minimum value of the array
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!
    \param[out] median optionally returns the median value of the array
    \param[out] maximum optionally returns the maximum value of the array
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param[out] quantile1 optionally returns the first quantile of the array (specified by \a quantile1Spec )
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param[out] quantile2 optionally returns the second quantile of the array (specified by \a quantile2Spec )
    \param[out] IQR interquartile range, i.e. the range between \a quantile1 and \a quantile2
    \param[out] IQRSignificance significance range of the interquartile range, calculated as \f[ 2\cdot\frac{1.58\cdot \mbox{IQR}}{\sqrt{N}} \f] \see https://en.wikipedia.org/wiki/Box_plot
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstatAddVBoxplotAndOutliers, jkqtpstatAddHBoxplotAndOutliers, jkqtpstatAddVBoxplot, jkqtpstatAddHBoxplot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt>
inline void jkqtpstat5NumberStatistics(InputIt first, InputIt last, double* minimum=nullptr, double minimumQuantile=0, double* median=nullptr, double* maximum=nullptr, double maximumQuantile=1, double quantile1Spec=0.25, double* quantile1=nullptr, double quantile2Spec=0.75, double* quantile2=nullptr, double* IQR=nullptr, double* IQRSignificance=nullptr, size_t* Noutput=nullptr) {
    std::vector<double> dataFiltered;
    jkqtpstatFilterGoodFloat(first, last, std::back_inserter(dataFiltered));
    std::sort(dataFiltered.begin(), dataFiltered.end());
    jkqtpstat5NumberStatisticsOfSortedVector(dataFiltered, minimum, minimumQuantile,  median,  maximum,  maximumQuantile, quantile1,  quantile1Spec,  quantile2,  quantile2Spec,  IQR,  IQRSignificance, Noutput);
}

/*! \brief calculates the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range)) of a given data range \a first ... \a last (5-value statistics, e.g. used for boxplots)
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a outliersout, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param outliersout output iterator that receives the outliers, smaller than minimum and larger than maximum
    \param[out] minimum optionally returns the minimum value of the array
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param[out] median optionally returns the median value of the array
    \param[out] maximum optionally returns the maximum value of the array
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param[out] quantile1 optionally returns the first quantile of the array (specified by \a quantile1Spec )
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param[out] quantile2 optionally returns the second quantile of the array (specified by \a quantile2Spec )
    \param[out] IQR interquartile range, i.e. the range between \a quantile1 and \a quantile2
    \param[out] IQRSignificance significance range of the interquartile range, calculated as \f[ 2\cdot\frac{1.58\cdot \mbox{IQR}}{\sqrt{N}} \f] \see https://en.wikipedia.org/wiki/Box_plot
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstatAddVBoxplotAndOutliers, jkqtpstatAddHBoxplotAndOutliers, jkqtpstatAddVBoxplot, jkqtpstatAddHBoxplot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstat5NumberStatisticsAndOutliers(InputIt first, InputIt last, OutputIt outliersout, double* minimum=nullptr, double minimumQuantile=0, double* median=nullptr, double* maximum=nullptr, double maximumQuantile=1, double* quantile1=nullptr, double quantile1Spec=0.25, double* quantile2=nullptr, double quantile2Spec=0.75, double* IQR=nullptr, double* IQRSignificance=nullptr, size_t* Noutput=nullptr) {
    std::vector<double> dataFiltered;
    jkqtpstatFilterGoodFloat(first, last, std::back_inserter(dataFiltered));
    std::sort(dataFiltered.begin(), dataFiltered.end());
    jkqtpstat5NumberStatisticsAndOutliersOfSortedVector(dataFiltered, outliersout, minimum, minimumQuantile,  median,  maximum,  maximumQuantile, quantile1,  quantile1Spec,  quantile2,  quantile2Spec,  IQR,  IQRSignificance, Noutput);
}

/*! \brief represents the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range))
    \ingroup jkqtptools_math_statistics_basic
    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstat5NumberStatistics()
*/
struct JKQTPStat5NumberStatistics {
        JKQTPStat5NumberStatistics();

        /** \brief minimum value */
        double minimum;
        /** \brief specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!) */
        double minimumQuantile;
        /** \brief first quantile value (specified by quantile1Spec) */
        double quantile1;
        /** \brief specifies the first quantile (range: 0..1) */
        double quantile1Spec;
        /** \brief median value */
        double median;
        /** \brief second quantile value (specified by quantile1Spec) */
        double quantile2;
        /** \brief specifies the second quantile (range: 0..1) */
        double quantile2Spec;
        /** \brief maximum value */
        double maximum;
        /** \brief specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!) */
        double maximumQuantile;
        /** \brief number of values used to calculate the summary */
        size_t N;
        /** \brief the interquarzile range */
        double IQR() const;
        /** \brief interquartile range, calculated as \f[ 2\cdot\frac{1.58\cdot \mbox{IQR}}{\sqrt{N}} \f] \see https://en.wikipedia.org/wiki/Box_plot */
        double IQRSignificanceEstimate() const;
        /** \brief list with the outlier values < minimum and > maximum */
        std::vector<double> outliers;
};

/*! \brief calculates the Five-Number Statistical Summary (minimum, median, maximum and two user-defined quantiles (as well as derived from these the inter quartile range)) of a given data range \a first ... \a last (5-value statistics, e.g. used for boxplots)
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \return the Five-Number Statistical Summary in a JKQTPStat5NumberStatistics

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Five-number_summary, jkqtpstatAddVBoxplotAndOutliers, jkqtpstatAddHBoxplotAndOutliers, jkqtpstatAddVBoxplot, jkqtpstatAddHBoxplot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt>
inline JKQTPStat5NumberStatistics jkqtpstat5NumberStatistics(InputIt first, InputIt last, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0) {
    JKQTPStat5NumberStatistics res;
    jkqtpstat5NumberStatisticsAndOutliers(first, last, std::back_inserter(res.outliers), &(res.minimum), minimumQuantile, &(res.median), &(res.maximum), maximumQuantile,  &(res.quantile1), quantile1Spec,&(res.quantile2),quantile2Spec, nullptr,nullptr,&(res.N));
    return res;
}


/*! \brief calculates the median of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the median of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatMedian(InputIt first, InputIt last, size_t* Noutput=nullptr) {
    std::vector<double> dataFiltered;
    jkqtpstatFilterGoodFloat(first, last, std::back_inserter(dataFiltered));
    std::sort(dataFiltered.begin(), dataFiltered.end());
    return jkqtpstatMedianOfSortedVector(dataFiltered, Noutput);
}


/*! \brief calculates the \a quantile -th quantile of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param quantile the given quantile, range 0..1 (e.g. 0.25 for the 25% quartile ...)
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the \a quantile -th quantile of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.
*/
template <class InputIt>
inline double jkqtpstatQuantile(InputIt first, InputIt last, double quantile, size_t* Noutput=nullptr) {
    std::vector<double> dataFiltered;
    jkqtpstatFilterGoodFloat(first, last, std::back_inserter(dataFiltered));
    std::sort(dataFiltered.begin(), dataFiltered.end());
    if (dataFiltered.size()<=0) {
        if (Noutput) *Noutput=0;
        return JKQTP_DOUBLE_NAN;
    } else {
        if (Noutput) *Noutput=dataFiltered.size();
        return dataFiltered[jkqtp_bounded<size_t>(0, static_cast<size_t>(quantile*static_cast<double>(dataFiltered.size()-1)), dataFiltered.size()-1)];
    }
}



/*! \brief calculates the median absolute deviation about the median (MAD) of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] median optionally returns the median value in this variable
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the median absolute deviation about the median (MAD) of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    This function calculates
      \f[ \mbox{MAD}(\vec{x})=\mbox{Med}\left\{|\vec{x}-\mbox{Med}(\vec{x})|\right\} \f]


    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Median_absolute_deviation and Ricardo A. Maronna, R. Douglas Martin, Victor J. Yohai: "Robust Statistics: Theory and Methods", Wiley, 2006, ISBN: 978-0-470-01092-1
*/
template <class InputIt>
inline double jkqtpstatMAD(InputIt first, InputIt last, double* median=nullptr, size_t* Noutput=nullptr) {
    std::vector<double> dataFiltered;
    jkqtpstatFilterGoodFloat(first, last, std::back_inserter(dataFiltered));
    std::sort(dataFiltered.begin(), dataFiltered.end());
    if (dataFiltered.size()<=0) {
        if (Noutput) *Noutput=0;
        if (median) *median=JKQTP_DOUBLE_NAN;
        return JKQTP_DOUBLE_NAN;
    } else {
        if (Noutput) *Noutput=dataFiltered.size();
        double med=jkqtpstatMedianOfSortedVector(dataFiltered);
        if (median) *median=med;
        for(double& v: dataFiltered) {
            v=fabs(v-med);
        }
        std::sort(dataFiltered.begin(), dataFiltered.end());
        return jkqtpstatMedianOfSortedVector(dataFiltered);
    }
}



/*! \brief calculates the normalized median absolute deviation about the median (NMAD) of a given data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_basic

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] median optionally returns the median value in this variable
    \param[out] Noutput optionally returns the number of accumulated valid values in this variable
    \return the normalized median absolute deviation about the median (NMAD) of the data returned between \a first and \a last (excluding invalid doubles).
            If the given range \a first ... \a last is empty, NAN is returned

    This function calculates
      \f[ \mbox{NMAD}(\vec{x})=\frac{\mbox{MAD}(\vec{x})}{0.6745}=\frac{\mbox{Med}\left\{|\vec{x}-\mbox{Med}(\vec{x})|\right\}}{0.6745} \f]


    \note This operation implies an internal copy of the data, as well as sorting it!

    \note Each value is the specified range is converted to a double using jkqtp_todouble().
          Entries in the range that are invalid double (using JKQTPIsOKFloat() )
          are ignored when calculating.

    \see https://en.wikipedia.org/wiki/Median_absolute_deviation and Ricardo A. Maronna, R. Douglas Martin, Victor J. Yohai: "Robust Statistics: Theory and Methods", Wiley, 2006, ISBN: 978-0-470-01092-1
*/
template <class InputIt>
inline double jkqtpstatNMAD(InputIt first, InputIt last, double* median=nullptr, size_t* Noutput=nullptr) {
    return jkqtpstatMAD(first, last, median, Noutput)/0.6745;
}




/*! \brief defines where the returned x-coordinates (in histogramXOut) lie inside a histogram bin
    \ingroup jkqtptools_math_statistics_1dhist
    \see jkqtpstatHistogram()
*/
enum class JKQTPStatHistogramBinXMode {
    XIsLeft, /*!< \brief x-location is the left edge of the bin */
    XIsMid, /*!< \brief x-location is the middle of the bin */
    XIsRight /*!< \brief x-location is the right edge of the bin */
};

/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param bins number of bins in the output histogram
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1DAutoranged()
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatHistogram1DAutoranged(InputIt first, InputIt last, OutputIt histogramXOut, OutputIt histogramYOut, int bins=11, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=range/static_cast<double>(bins);

    // initialize the histogram
    for (int i=0; i<bins; i++)  {
        histX.push_back(minV+static_cast<double>(i)*binw);
        histY.push_back(0);
    }

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            size_t b=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((v-minV)/binw)), bins-1);
            histY[b]++;
        }
    }


    // output the histogram
    double xoffset=0;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;

    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *++histogramYOut=h;
    }
}

/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param binWidth width of the bins
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1DAutoranged()
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatHistogram1DAutoranged(InputIt first, InputIt last, OutputIt histogramXOut, OutputIt histogramYOut, double binWidth, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=binWidth;
    const int bins=static_cast<int>(ceil(range/binWidth));

    // initialize the histogram
    for (int i=0; i<bins; i++)  {
        histX.push_back(minV+static_cast<double>(i)*binw);
        histY.push_back(0);
    }

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            size_t b=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((v-minV)/binw)), bins-1);
            histY[b]++;
        }
    }


    // output the histogram
    double xoffset=0;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;

    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *++histogramYOut=h;
    }
}



/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined the range \a binsFirst ... \a binsLast
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of histogram bins
    \param binsLast iterator pointing behind the last item in the set of histogram bins
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1D()
*/
template <class InputIt, class BinsInputIt, class OutputIt>
inline void jkqtpstatHistogram1D(InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, OutputIt histogramXOut, OutputIt histogramYOut, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // initialize the histogram
    for (auto it=binsFirst; it!=binsLast; ++it)  {
        histX.push_back(jkqtp_todouble(*it));
        histY.push_back(0);
    }
    std::sort(histX.begin(), histX.end());

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            auto itb=std::lower_bound(histX.begin(), histX.end(), v);
            size_t bin=jkqtp_bounded<size_t>(0,static_cast<size_t>(abs(std::distance(histX.begin(), itb))), histY.size()-1);
            histY[bin]++;
        }
    }


    // output the histogram
    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        double xoffset=0;
        double binw=1;
        if (binXMode!=JKQTPStatHistogramBinXMode::XIsLeft) {
            if (i==0 && i+1<histX.size()) binw=histX[1]-histX[0];
            else if (i==histX.size()-1 && static_cast<int>(i)-1>0) binw=histX[histX.size()-1]-histX[histX.size()-2];
            else if (i<histX.size() && i+1<histX.size()) binw=histX[i+1]-histX[i];
            if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
            if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;
        }

        *++histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *++histogramYOut=h;
    }
}










/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{\sqrt{2\pi}}\exp \left(-\frac{1}{2}t^2\right) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DGaussian(double t);
/*! \brief a 1D Cauchy kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{\pi(1+t^2)} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DCauchy(double t);

/*! \brief a 1D Picard kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{2}\exp(-|t|) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DPicard(double t);
/*! \brief a 1D Epanechnikov kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{3}{4} ( 1- t^2 ),  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DEpanechnikov(double t);
/*! \brief a 1D uniform kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}0.5,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DUniform(double t);
/*! \brief a 1D Epanechnikov kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}1-|t|,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTriangle(double t);

/*! \brief a 1D quartic kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{15}{16}(1-t^2)^2,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DQuartic(double t);
/*! \brief a 1D triweight kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{35}{32}(1-t^2)^3,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTriweight(double t);

/*! \brief a 1D tricube kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{70}{81}(1-|t|^3)^3,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTricube(double t);
/*! \brief a 1D cosine kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{\pi}{4}\cos\left(\frac{\pi}{2}t\right),  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DCosine(double t);













/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_2dkde_kernels

    \f[ k(t_x, t_y):=\frac{1}{2\pi}\exp \left(-\frac{t_x^2+t_y^2}{2}\right) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel2DGaussian(double tx, double ty);

/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_2dkde_kernels

    \f[ k(t_x, t_y):=\begin{cases}\frac{1}{4},  & \text{if }t_x,t_y\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel2DUniform(double tx, double ty);



















/*! \brief estimates a bandwidth for a Kernel Density Estimator (KDE) of the given data \a first ... \a last
    \ingroup jkqtptools_math_statistics_1dkde

    evaluates \f[ h = \left(\frac{4\hat{\sigma}^5}{3n}\right)^{\frac{1}{5}} \approx 1.06 \hat{\sigma} n^{-1/5} \f]

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \return the estimated bandwidth

*/
template <class InputIt>
inline double jkqtpstatEstimateKDEBandwidth(InputIt first, InputIt last) {
    size_t N=0;
    const double sigma=jkqtpstatStdDev(first, last, nullptr, &N);
    return 1.06*sigma/pow(static_cast<double>(N), 1.0/5.0);
}




/*! \brief evaluates the Kernel Density Estimator (KDE) at a given position
    \ingroup jkqtptools_math_statistics_1dkde

    evaluates \f[ \tilde{f}(t):=\frac{1}{N\cdot\text{bandwidth}}\cdot\sum\limits_{i=0}^{N-1}K\left(\frac{t-x_i}{\text{bandwidth}}\right) \f]

    \tparam InputIt standard iterator type of \a first and \a last.
    \param t where to evaluate the kernel sum
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE

*/
template <class InputIt>
inline double jkqtpstatEvaluateKernelSum(double t, InputIt first, InputIt last, const std::function<double(double)>& kernel, double bandwidth) {
    double res=0;
    size_t cnt=0;
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            const double vx=(t-v)/bandwidth;
            res+=kernel(vx);
            cnt++;
        }
    }
    return res/static_cast<double>(cnt)/bandwidth;
}




/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param Nout number datapoints in the output KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    This function performs <a href="https://en.wikipedia.org/wiki/Kernel_density_estimation">Kernel Density Estimation</a> for a given data array.
    Then the resulting density is evaluated on a regular grid spanning [min(X)...max(X)] with bins datapoints in between.

    \warning this functions is getting very slow for large dataset, as for each point in the resulting histogram N kernel functions have to be evaluated.

    \see en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1DAutoranged(InputIt first, InputIt last, OutputIt KDEXOut, OutputIt KDEYOut, int Nout=100, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=range/static_cast<double>(Nout);

    // calculate the KDE
    for (int i=0; i<Nout; i++)  {
        const double x=minV+static_cast<double>(i)*binw+binw/2.0;
        histX.push_back(x);
        histY.push_back(jkqtpstatEvaluateKernelSum(x, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *++KDEYOut=h;
    }
}


/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param binWidth width of the bins
    \param cummulative if \c true, a cummulative KDE is calculated

    This function performs <a href="https://en.wikipedia.org/wiki/Kernel_density_estimation">Kernel Density Estimation</a> for a given data array.
    Then the resulting density is evaluated on a regular grid spanning [min(X)...max(X)] with bins datapoints in between.

    \warning this functions is getting very slow for large dataset, as for each point in the resulting histogram N kernel functions have to be evaluated.

    \see en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1DAutoranged(InputIt first, InputIt last, OutputIt KDEXOut, OutputIt KDEYOut, double binWidth, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=binWidth;
    const int Nout=static_cast<int>(ceil(range/binWidth));

    // calculate the KDE
    for (int i=0; i<Nout; i++)  {
        histX.push_back(minV+static_cast<double>(i)*binw+binw/2.0);
        histY.push_back(jkqtpstatEvaluateKernelSum(*(histX.end()), first, last, kernel, bandwidth));
    }


    // output the KDE

    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *++KDEYOut=h;
    }

}



/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined the range \a binsFirst ... \a binsLast
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of KDE bins
    \param binsLast iterator pointing behind the last item in the set of KDE bins
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    \see en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class BinsInputIt, class OutputIt>
inline void jkqtpstatKDE1D(InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, OutputIt KDEXOut, OutputIt KDEYOut, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // initialize the KDE
    for (auto it=binsFirst; it!=binsLast; ++it)  {
        histX.push_back(jkqtp_todouble(*it));
    }
    std::sort(histX.begin(), histX.end());

    // calculate the KDE
    for (auto it=histX.begin(); it!=histX.end(); ++it) {
        histY.push_back(jkqtpstatEvaluateKernelSum(*it, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *++KDEYOut=h;
    }

}




/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, evaluation positions are given by the range \a binXLeft ... \a binXRight (in steps of \a binxDelta )
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binXLeft first x-position, where to evaluate the KDE
    \param binXDelta distance between two x-positions at which the KDE is evaluated
    \param binXRight last x-position, where to evaluate the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    \see en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1D(InputIt first, InputIt last, double binXLeft, double binXDelta, double binXRight, OutputIt KDEXOut, OutputIt KDEYOut, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // calculate the KDE
    for (double x=binXLeft; x<=binXRight; x+=binXDelta)  {
        histX.push_back(x);
        histY.push_back(jkqtpstatEvaluateKernelSum(x, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *++KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *++KDEYOut=h;
    }

}







/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is \f$ f(x)=a+b\cdot x \f$
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-(a+b\cdot x_i)\right)^2 \f]
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used

    This function computes internally:
        \f[ a=\overline{y}-b\cdot\overline{x} \f]
        \f[ b=\frac{\sum x_iy_i-N\cdot\overline{x}\cdot\overline{y}}{\sum x_i^2-N\cdot(\overline{x})^2} \f]

    \image html jkqtplotter_simpletest_datastore_regression_lin.png
*/
template <class InputItX, class InputItY>
inline void jkqtpstatLinearRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);

    JKQTPASSERT(Nx>1 && Ny>1);

    double sumx=0, sumy=0, sumxy=0, sumx2=0;
    size_t N=0;
    auto itX=firstX;
    auto itY=firstY;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            sumx=sumx+fit_x;
            sumy=sumy+fit_y;
            sumxy=sumxy+fit_x*fit_y;
            sumx2=sumx2+fit_x*fit_x;
            N++;
        }
    }
    const double NN=static_cast<double>(N);
    JKQTPASSERT_M(NN>1, "too few datapoints");
    if (!fixA && !fixB) {
        coeffB=(double(sumxy)-double(sumx)*double(sumy)/NN)/(double(sumx2)-double(sumx)*double(sumx)/NN);;
        coeffA=double(sumy)/NN-coeffB*double(sumx)/NN;
    } else if (fixA && !fixB) {
        coeffB=(double(sumy)/NN-coeffA)/(double(sumx)/NN);
    } else if (!fixA && fixB) {
        coeffA=double(sumy)/NN-coeffB*double(sumx)/NN;
    }
}


/*! \brief calculate the weighted linear regression coefficients for a given for a given data range \a firstX / \a firstY / \a firstW ... \a lastX / \a lastY / \a lastW  where the model is \f$ f(x)=a+b\cdot x \f$
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_iw_i^2\cdot\left(y_i-(a+b\cdot x_i)\right)^2 \f]
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.


    This function internally computes:
        \f[ a=\frac{\overline{y}-b\cdot\overline{x}}{\overline{w^2}} \f]
        \f[ b=\frac{\overline{w^2}\cdot\overline{x\cdot y}-\overline{x}\cdot\overline{y}}{\overline{x^2}\cdot\overline{w^2}-\overline{x}^2} \f]

    Here the averages are defined in terms of a weight vector \f$ w_i\f$:
        \f[ \overline{x}=\sum\limits_iw_i^2\cdot x_i \f]
        \f[ \overline{y}=\sum\limits_iw_i^2\cdot y_i \f]
        \f[ \overline{x\cdot y}=\sum\limits_iw_i^2\cdot x_i\cdot y_i \f]
        \f[ \overline{x^2}=\sum\limits_iw_i^2\cdot x_i^2 \f]
        \f[ \overline{w^2}=\sum\limits_iw_i^2 \f]

    \image html jkqtplotter_simpletest_datastore_regression_linweight.png

*/
template <class InputItX, class InputItY, class InputItW>
inline void jkqtpstatLinearWeightedRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);
    const int Nw=std::distance(firstW,lastW);

    JKQTPASSERT(Nx>1 && Ny>1 && Nw>1);

    double  sumx=0, sumy=0, sumxy=0, sumx2=0, sumw2=0;
    size_t N=0;
    auto itX=firstX;
    auto itY=firstY;
    auto itW=firstW;
    for (; itX!=lastX && itY!=lastY && itW!=lastW; ++itX, ++itY, ++itW) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        const double fit_w2=jkqtp_sqr(fWeightDataToWi(jkqtp_todouble(*itW)));
        if (JKQTPIsOKFloat(fit_x)&&JKQTPIsOKFloat(fit_y)&&JKQTPIsOKFloat(fit_w2)) {
            sumx=sumx+fit_w2*fit_x;
            sumy=sumy+fit_w2*fit_y;
            sumxy=sumxy+fit_w2*fit_x*fit_y;
            sumx2=sumx2+fit_w2*fit_x*fit_x;
            sumw2=sumw2+fit_w2;
            N++;
        }
    }
    const double NN=static_cast<double>(N);
    JKQTPASSERT_M(NN>1, "too few datapoints");
    if (!fixA && !fixB) {
        coeffB=(double(sumxy)*double(sumw2)-double(sumx)*double(sumy))/(double(sumx2)*double(sumw2)-double(sumx)*double(sumx));
        coeffA=(double(sumy)-coeffB*double(sumx))/double(sumw2);
    } else if (fixA && !fixB) {
        coeffB=(double(sumy)-coeffA*double(sumw2))/double(sumx);
    } else if (!fixA && fixB) {
        coeffA=(double(sumy)-coeffB*double(sumx))/double(sumw2);
    }
}



/*! \brief calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters of the model \f$ f(x)=a+b\cdot x \f$
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
           So this function finds an outlier-robust solution to the optimization problem:
           \f[ (a^\ast,b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^p \f]
    \ingroup jkqtptools_math_statistics_regression

    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    This is a simple form of the IRLS algorithm to estimate the parameters a and b in a linear model \f$ f(x)=a+b\cdot x \f$.
    This algorithm solves the optimization problem for a \f$ L_p\f$-norm:
      \f[ (a^\ast,b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^p \f]
    by iteratively optimization weights \f$ \vec{w} \f$ and solving a weighted least squares problem in each iteration:
      \f[ (a_n,b_n)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^{(p-2)}\cdot|a+b\cdot x_i-y_i|^2 \f]


    The IRLS-algorithm works as follows:
      - calculate initial \f$ a_0\f$ and \f$ b_0\f$ with unweighted regression from x and y
      - perform a number of iterations (parameter \a iterations ). In each iteration \f$ n\f$:
          - calculate the error vector \f$\vec{e}\f$: \f[ e_i = a+b\cdot x_i -y_i \f]
          - estimate new weights \f$\vec{w}\f$: \f[ w_i=|e_i|^{(p-2)/2} \f]
          - calculate new estimates \f$ a_n\f$ and \f$ b_n\f$ with weighted regression from \f$ \vec{x}\f$ and \f$ \vec{y}\f$ and \f$ \vec{w}\f$
        .
      - return the last estimates \f$ a_n\f$ and \f$ b_n\f$
    .

    \image html irls.png

    \image html jkqtplotter_simpletest_datastore_regression_linrobust_p.png

    \see https://en.wikipedia.org/wiki/Iteratively_reweighted_least_squares, C. Sidney Burrus: "Iterative Reweighted Least Squares", <a href="http://cnx.org/content/m45285/latest/">http://cnx.org/content/m45285/latest/</a>
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRobustIRLSLinearRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);
    const int N=std::min(Nx,Ny);

    JKQTPASSERT(Nx>1 && Ny>1);

    std::vector<double> weights;
    std::fill_n(std::back_inserter(weights), N, 1.0);

    double alast=coeffA, blast=coeffB;
    jkqtpstatLinearWeightedRegression(firstX, lastX, firstY, lastY, weights.begin(), weights.end(), alast, blast, fixA, fixB, &jkqtp_identity<double>);
    for (int it=0; it<iterations-1; it++) {
        // calculate weights
        auto itX=firstX;
        auto itY=firstY;
        for (double& w: weights) {
            const double fit_x=*itX;
            const double fit_y=*itY;
            const double e=alast+blast*fit_x-fit_y;
            w=pow(std::max<double>(JKQTP_EPSILON*100.0, fabs(e)), (p-2.0)/2.0);
            ++itX;
            ++itY;
        }
        // solve weighted linear least squares
        jkqtpstatLinearWeightedRegression(firstX, lastX, firstY, lastY, weights.begin(), weights.end(), alast, blast, fixA, fixB, &jkqtp_identity<double>);
    }
    coeffA=alast;
    coeffB=blast;
}







/*! \brief when performing linear regression, different target functions can be fitted, if the input data is transformed accordingly. This library provides the options in this enum by default.
    \ingroup jkqtptools_math_statistics_regression
    */
enum class JKQTPStatRegressionModelType {
    Linear,       /*!< \brief linear model \f$ f(x)=a+b\cdot x \f$ */
    PowerLaw,     /*!< \brief power law model \f$ f(x)=a\cdot x^b \f$ */
    Exponential,  /*!< \brief exponential model \f$ f(x)=a\cdot \exp(b\cdot x) \f$ */
    Logarithm,   /*!< \brief exponential model \f$ f(x)=a+b\cdot \ln(x) \f$ */
};


/*! \brief Generates functors \c f(x,a,b) for the models from JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTP_LIB_EXPORT std::function<double(double, double, double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type);

/*! \brief Generates a LaTeX string for the models from JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTP_LIB_EXPORT QString jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType type, double a, double b);

/*! \brief Generates functors \c f(x) for the models from JKQTPStatRegressionModelType in \a type and binds the parameter values \a and \a b to the returned function
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTP_LIB_EXPORT std::function<double(double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type, double a, double b);

/*! \brief Generates the transformation function for x-data (\c result.first ) and y-data (\c result.second ) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTP_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateTransformation(JKQTPStatRegressionModelType type);


/*! \brief Generates the transformation function for a-parameter (offset, \c result.first : transform, \c result.second : back-transform) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTP_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateParameterATransformation(JKQTPStatRegressionModelType type);


/*! \brief Generates the transformation function for b-parameter (slope, \c result.first : transform, \c result.second : back-transform) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTP_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateParameterBTransformation(JKQTPStatRegressionModelType type);


/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-f_{\text{type}}(x_i,a,b)\right)^2 \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatLinearRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatLinearRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatLinearRegression(x.begin(), x.end(), y.begin(), y.end(), a, b, fixA, fixB);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}



/*! \brief calculate the robust linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the Lp-norm optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-f_{\text{type}}(x_i,a,b)\right)^p \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatRobustIRLSLinearRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatRobustIRLSLinearRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRobustIRLSRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatRobustIRLSLinearRegression(x.begin(), x.end(), y.begin(), y.end(), a, b, fixA, fixB, p, iterations);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}




/*! \brief calculate the robust linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the Lp-norm optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-f_{\text{type}}(x_i,a,b)\right)^p \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatLinearWeightedRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatLinearWeightedRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY, class InputItW>
inline void jkqtpstatWeightedRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatLinearWeightedRegression(x.begin(), x.end(), y.begin(), y.end(), firstW, lastW, a, b, fixA, fixB, fWeightDataToWi);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}




/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$
    \ingroup jkqtptools_math_statistics_poly

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam OutputItP output iterator for the polynomial coefficients
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param P degree of the polynomial (P>=N !!!)
    \param[out] firstRes Iterator (of type \a OutputItP ), which receives the (P+1)-entry vector with the polynomial coefficients \f$ p_i \f$

    This function uses jkqtpstatLinSolve() to solve the system of equations
      \f[ \begin{bmatrix} y_1\\ y_2\\ y_3 \\ \vdots \\ y_n \end{bmatrix}= \begin{bmatrix} 1 & x_1 & x_1^2 & \dots & x_1^P \\ 1 & x_2 & x_2^2 & \dots & x_2^P\\ 1 & x_3 & x_3^2 & \dots & x_3^P \\ \vdots & \vdots & \vdots & & \vdots \\ 1 & x_n & x_n^2 & \dots & x_n^P \end{bmatrix} \begin{bmatrix} p_0\\ p_1\\ p_2\\ \vdots \\ p_P \end{bmatrix}  \f]
      \f[ \vec{y}=V\vec{p}\ \ \ \ \ \Rightarrow\ \ \ \ \ \vec{p}=(V^TV)^{-1}V^T\vec{y} \f]

    \image html jkqtplotter_simpletest_datastore_regression_polynom.png

    \see https://en.wikipedia.org/wiki/Polynomial_regression
*/
template <class InputItX, class InputItY, class OutputItP>
inline void jkqtpstatPolyFit(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t P, OutputItP firstRes) {
    {
        const int Nx=std::distance(firstX,lastX);
        const int Ny=std::distance(firstY,lastY);
        JKQTPASSERT(Nx>1 && Ny>1);
    }

    size_t N=0;

    std::vector<double> X,Y;
    auto itX=firstX;
    auto itY=firstY;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            X.push_back(fit_x);
            Y.push_back(fit_y);
            N++;
        }
    }

    // build Vandermonde matrix V
    std::vector<double> V;
    V.resize(N*(P+1));
    for (size_t l=0; l<N; l++) {
        V[jkqtplinalgMatIndex(l,0,P+1)]=1.0;
        double x=X[l];
        const double xx=x;
        for (size_t c=1; c<P+1; c++) {
            V[jkqtplinalgMatIndex(l,c,P+1)]=x;
            x=x*xx;
        }
    }
#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V = \n";
    jkqtplinalgPrintMatrix(V.data(),N,P+1);
    std::cout<<"\n";
#endif

    // calculate V^T
   std::vector<double> VT=V;
    jkqtplinalgTransposeMatrix(VT.data(), static_cast<long>(N), static_cast<long>(P+1));

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T = \n";
    jkqtplinalgPrintMatrix(VT.data(),P+1,N);
    std::cout<<"\n";
#endif

    // calculate V^T*V
    std::vector<double> VTV;
    VTV.resize((P+1)*(P+1));
    jkqtplinalgMatrixProduct(VT.data(), static_cast<long>(P+1), static_cast<long>(N), V.data(), static_cast<long>(N), static_cast<long>(P+1), VTV.data());

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T*V = \n";
    jkqtplinalgPrintMatrix(VTV.data(),P+1,P+1);
    std::cout<<"\n";
#endif

    // calculate V^T*y
    std::vector<double> VTY;
    VTY.resize(P+1);
    jkqtplinalgMatrixProduct(VT.data(), static_cast<long>(P+1), static_cast<long>(N), Y.data(), static_cast<long>(N), 1, VTY.data());

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T*y = \n";
    jkqtplinalgPrintMatrix(VTY.data(),P+1,1);
    std::cout<<"\n";
#endif

    // solve V^T*y = V^T*V*p
    const bool ok=jkqtplinalgLinSolve(VTV.data(), VTY.data(), static_cast<long>(P+1));

    if (ok) {
        auto itR=firstRes;
        for (size_t p=0; p<P+1; p++) {
            *++itR=VTY[p];
        }
    } else {
        throw std::runtime_error("jkqtplinalgLinSolve() didn't return a result!");
    }

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"result_out = \n";
    jkqtplinalgPrintMatrix(result_out,P+1,1);
    std::cout<<"\n";
#endif

}

/*! \brief evaluate a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ with \f$ p_i \f$ taken from the range \a firstP ... \a lastP
    \ingroup jkqtptools_math_statistics_poly

    \tparam PolyItP iterator for the polynomial coefficients
    \param x where to evaluate
    \param firstP points to the first polynomial coefficient \f$ p_1 \f$ (i.e. the offset with \f$ x^0 \f$ )
    \param lastP points behind the last polynomial coefficient  \f$ p_P \f$
    \return value of polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ at location \a x

*/
template <class PolyItP>
inline double jkqtpstatPolyEval(double x, PolyItP firstP, PolyItP lastP) {
    double v=0.0;
    double xx=1.0;
    for (auto itP=firstP; itP!=lastP; ++itP) {
        v=v+(*itP)*xx;
        xx=xx*x;
    }
    return v;
}

/*! \brief a C++-functor, which evaluates a polynomial
    \ingroup jkqtptools_math_statistics_poly
*/
struct JKQTPStatPolynomialFunctor {
        std::vector<double> P;
        template <class PolyItP>
        inline JKQTPStatPolynomialFunctor(PolyItP firstP, PolyItP lastP) {
            for (auto itP=firstP; itP!=lastP; ++itP) {
                P.push_back(*itP);
            }
        }
        inline double operator()(double x) const { return jkqtpstatPolyEval(x, P.begin(), P.end()); }

};

/*! \brief returns a C++-functor, which evaluates a polynomial
    \ingroup jkqtptools_math_statistics_poly

    \tparam PolyItP iterator for the polynomial coefficients
    \param firstP points to the first polynomial coefficient \f$ p_1 \f$ (i.e. the offset with \f$ x^0 \f$ )
    \param lastP points behind the last polynomial coefficient  \f$ p_P \f$
*/
template <class PolyItP>
inline std::function<double(double)> jkqtpstatGeneratePolynomialModel(PolyItP firstP, PolyItP lastP) {
    return JKQTPStatPolynomialFunctor(firstP, lastP);
}

/*! \brief Generates a LaTeX string for the polynomial model with the coefficients \a firstP ... \a lastP
    \ingroup jkqtptools_math_statistics_regression

    \tparam PolyItP iterator for the polynomial coefficients
    \param firstP points to the first polynomial coefficient \f$ p_1 \f$ (i.e. the offset with \f$ x^0 \f$ )
    \param lastP points behind the last polynomial coefficient  \f$ p_P \f$
    */
template <class PolyItP>
QString jkqtpstatPolynomialModel2Latex(PolyItP firstP, PolyItP lastP) {
    QString str="f(x)=";
    size_t p=0;
    for (auto itP=firstP; itP!=lastP; ++itP) {
        if (p==0) str+=jkqtp_floattolatexqstr(*itP, 3);
        else {
            if (*itP>=0) str+="+";
            str+=QString("%2{\\cdot}x^{%1}").arg(p).arg(jkqtp_floattolatexqstr(*itP, 3));
        }
        p++;
    }
    return str;
}





/*! \brief calculates the coefficient of determination \f$ R^2 \f$ for a set of measurements \f$ (x_i,y_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_poly

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \return coeffcicient of determination \f[ R^2=1-\frac{\sum_i\bigl[y_i-f(x_i)\bigr]^2}{\sum_i\bigl[y_i-\overline{y}\bigr]^2} \f] where \f[ \overline{y}=\frac{1}{N}\cdot\sum_iy_i \f]



    \see https://en.wikipedia.org/wiki/Coefficient_of_determination
*/
template <class InputItX, class InputItY>
inline double jkqtpstatCoefficientOfDetermination(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, std::function<double(double)> f) {

    auto itX=firstX;
    auto itY=firstY;

    const double yMean=jkqtpstatAverage(firstX,lastX);
    double SSres=0;
    double SStot=0;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            SStot+=jkqtp_sqr(fit_y-yMean);
            SSres+=jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return 1.0-SSres/SStot;
}




/*! \brief calculates the sum of deviations \f$ \chi^2 \f$ for a set of measurements \f$ (x_i,y_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_poly

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \return sum of deviations  \f[ \chi^2=\sum_i\bigl[y_i-f(x_i)\bigr]^2 \f]



    \see https://en.wikipedia.org/wiki/Coefficient_of_determination
*/
template <class InputItX, class InputItY>
inline double jkqtpstatSumOfDeviations(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, std::function<double(double)> f) {

    auto itX=firstX;
    auto itY=firstY;

    double SSres=0;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            SSres+=jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return SSres;
}




/*! \brief calculates the weighted sum of deviations \f$ \chi^2 \f$ for a set of measurements \f$ (x_i,y_i,w_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_poly

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.
    \return weighted sum of deviations  \f[ \chi^2=\sum_iw_i^2\cdot\bigl[y_i-f(x_i)\bigr]^2 \f]


    \see https://en.wikipedia.org/wiki/Reduced_chi-squared_statistic
*/
template <class InputItX, class InputItY, class InputItW>
inline double jkqtpstatWeightedSumOfDeviations(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, std::function<double(double)> f, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {

    auto itX=firstX;
    auto itY=firstY;
    auto itW=firstW;

    double SSres=0;
    for (; itX!=lastX && itY!=lastY && itW!=lastW; ++itX, ++itY, ++itW) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        const double fit_w2=jkqtp_sqr(fWeightDataToWi(jkqtp_todouble(*itW)));
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y) && JKQTPIsOKFloat(fit_w2)) {
            SSres+=fit_w2*jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return SSres;
}






#endif // JKQTPSTATISTICSTOOLS_H_INCLUDED


