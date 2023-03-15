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


#ifndef JKQTPSTATBASICS_H_INCLUDED
#define JKQTPSTATBASICS_H_INCLUDED

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
#include "jkqtmath/jkqtmath_imexport.h"
#include "jkqtmath/jkqtplinalgtools.h"
#include "jkqtmath/jkqtparraytools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"


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
    if (NN==0) return JKQTP_DOUBLE_NAN;
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
    if (NN==0) return JKQTP_DOUBLE_NAN;
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
    if (NN==0) {
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
    if (NN==0) return 0;
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
                       Output is then a vector of cummulative (or partial) sums returned between
                       \a first and \a last (excluding invalid doubles).
                       For invalid values, the last sum is re-inserted, so the returned vector has
                       the same number of entries as the range \a first ... \a last

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
        *output=sum;
        ++output;
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
            *output=v;
            ++output;
            NN++;
        }
    }
    return NN;
}










/*! \brief calculates the variance \f$ \sigma_X^2=\mbox{Var}(X) \f$ of a given data range \a first ... \a last
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
      \f[ \sigma_X^2=\text{Var}(X)=\frac{1}{N-1}\cdot\sum\limits_{i=1}^{N}(X_i-\overline{X})^2=\frac{1}{N-1}\cdot\left(\sum_{i=1}^NX_i^2-\frac{1}{N}\cdot\left(\sum_{i=1}^NX_i\right)^2\right) \f]

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
        if (NN==0) *averageOut=JKQTP_DOUBLE_NAN;
        else *averageOut=sum/static_cast<double>(NN);
    }
    if (Noutput) *Noutput=NN;
    if (NN<=1) return 0;
    else return ( sum2 - sum*sum/static_cast<double>(NN) ) / static_cast<double>(NN-1);
}



/*! \brief calculates the standard deviation \f$ \sigma_X=\sqrt{\mbox{Var}(X)} \f$ of a given data range \a first ... \a last
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




/*! \brief calculates the weighted variance \f$ \sigma_X^2=\mbox{Var}(X) \f$ of a given data range \a first ... \a last
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
      \f[ \sigma_v^2=\text{Var}(v)=\frac{\sum\limits_{i=1}^{N}w_i\cdot (v_i-\overline{v})^2}{\sum\limits_{i=1}^{N}w_i} \f]

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
    if (NN==0) return 0;
    else return sum2/sumW;
}



/*! \brief calculates the weighted standard deviation \f$ \sigma_X=\sqrt{\mbox{Var}(X)} \f$ of a given data range \a first ... \a last
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




/*! \brief calculates the skewness \f$ \gamma_1=\mathbb{E}\left[\left(\frac{X-\mu}{\sigma}\right)^3\right] \f$ of a given data range \a first ... \a last
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
    if (NN==0) return 0;
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
    if (NN==0) return 0;
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
    if (NN==0) return 0;
    return sum/double(NN);
}




/*! \brief calculate empirical (Pearson's) correlation coefficient \f$ \rho_{x,y} \f$ between two given data ranges \a first1 ... \a last1 and  \a first2 ... \a last2
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
        \f[ \rho_{x,y}=\text{CorCoeff}_{\text{Pearson}}(x,y)=\frac{\sum\limits_{i=0}^{N-1}(x_i-\overline{x})(y_i-\overline{y})}{\sqrt{\sum\limits_{i=0}^{N-1}(x_i-\overline{x})^2\cdot\sum\limits_{i=0}^{N-1}(y_i-\overline{y})^2}} \f]

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
        if (NN==0) *averageOut1=JKQTP_DOUBLE_NAN;
        else *averageOut1=xbar/static_cast<double>(NN);
    }
    if (averageOut2) {
        if (NN==0) *averageOut2=JKQTP_DOUBLE_NAN;
        else *averageOut2=ybar/static_cast<double>(NN);
    }
    if (NN==0) return JKQTP_DOUBLE_NAN;

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
                *outliersout=*it;
                ++outliersout;
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
inline void jkqtpstat5NumberStatistics(InputIt first, InputIt last, double* minimum, double minimumQuantile=0, double* median=nullptr, double* maximum=nullptr, double maximumQuantile=1, double quantile1Spec=0.25, double* quantile1=nullptr, double quantile2Spec=0.75, double* quantile2=nullptr, double* IQR=nullptr, double* IQRSignificance=nullptr, size_t* Noutput=nullptr) {
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
struct jkqtmath_LIB_EXPORT JKQTPStat5NumberStatistics {
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




#endif // JKQTPSTATBASICS_H_INCLUDED


